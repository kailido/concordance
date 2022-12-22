#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_WORD_LEN 20
#define MAX_WORDS    100
#define MAX_LINE_LEN 70
#define MAX_LINES    100
/*
 * Global variables
 */
int num_words = 0;
int num_lines = 0;
char lines[MAX_LINES][MAX_LINE_LEN];
char words[MAX_WORDS][MAX_WORD_LEN];


char bad_words[MAX_WORDS][MAX_WORD_LEN];
int bad_count = 0;
int indexed_count = 0;
int line_num = 0;

typedef struct {
    
    int index;
    int word_count;
    char word[MAX_WORD_LEN];
    char sentence[MAX_LINE_LEN];
    char capped[MAX_WORD_LEN];
    
} indexedWord;


int in = 0;

void tokenize_line (char *input_line) {
    char *t;

    t = strtok (input_line, " ,");
    while (t && num_words < MAX_WORDS) {
        strncpy (words[num_words], t, MAX_WORD_LEN);
        num_words++;
        t = strtok (NULL, " ,");
    }
}

int compare_words(const void *wa, const void *wb) {
    char *p_wa = (char *)wa;
    char *p_wb = (char *)wb;

    return strcmp(p_wa, p_wb);
}

// got code from https://www.scaler.com/topics/string-length-in-c/
int str_length(char str[]) {
    // initializing count variable (stores the length of the string)
    int count; 
    
    // incrementing the count till the end of the string
    for (count = 0; str[count] != '\0'; ++count);
    
    // returning the character count of the string
    return count; 
}



void print_order(char word_order[MAX_LINES][MAX_LINE_LEN], indexedWord indexed_words[MAX_WORDS], int index_count) {

    int i = 0;
    int j = 0;
    int k = 0;
    char l[MAX_LINES][MAX_LINE_LEN];
    while (i < index_count) {
        while (j < index_count) {
            if (strncmp(word_order[i], indexed_words[j].word, MAX_WORD_LEN) == 0){      // if the index_word is the same as the word order
                
                num_words = 0;
                strncpy(l[j], indexed_words[j].sentence, MAX_LINE_LEN);
                int len = strlen(l[i]);
                if (len > 0 && l[i][len-1] == '\n') {
                    l[i][len-1] = '\0';
                }
                tokenize_line(l[j]);
                k = 0;
                while (k < num_words) {
                    if (strncmp(words[k], indexed_words[j].word, MAX_WORD_LEN) == 0) { // if the word in the sentence is the index word, capitilize it
                        printf("%s", indexed_words[j].capped);
                        strncpy(indexed_words[j].word, indexed_words[j].capped, MAX_WORD_LEN);
                    } else {
                        printf("%s", words[k]);
                    }
                    k++;
                    if (k < num_words) {                                                // prints a space after each word except the last word
                        printf(" ");
                    }
                }
                printf("\n");
                j = 0;
                break;
            }
            j++;
        }
        j = 0;
        i++;
    }
}

void capitilize_word(indexedWord indexed_words[MAX_WORDS], int indexed_count) {

    int i = 0;
    char l[MAX_LINES][MAX_LINE_LEN];
    while (i < indexed_count) {
        strncpy(l[i], indexed_words[i].sentence, MAX_LINE_LEN);
        int len = strlen(l[i]);                                                 //removes \n from each line
        if (len > 0 && l[i][len-1] == '\n') {                   
            l[i][len-1] = '\0';
        }
        num_words = 0;
        tokenize_line(l[i]);
        int j = 0;
        while (j < num_words){
            if (strncmp(words[j], indexed_words[i].word, MAX_WORD_LEN) == 0 ){  //if the word is the index word, capitilizes it
                strncpy(indexed_words[i].capped, words[j], MAX_WORD_LEN);
                int k = 0;
                while( k < strlen(words[j])) {
                    words[j][k] = toupper(words[j][k]);
                    k++;
                }
                
                strncpy(indexed_words[i].capped, words[j], MAX_WORD_LEN);       //copies capitilized word to indexed_words capped location      
            }
            j++;
        }
        i++;
    }
}






int main(int argc, char *argv[]) {


    int i = 0;
    char l[MAX_LINES][MAX_LINE_LEN];
    char word_order[MAX_LINES][MAX_LINE_LEN];
    indexedWord indexed_words[MAX_WORDS];


    int count1 = 0;
    int bad_count = 0;
    while (fgets(lines[count1], MAX_WORDS, stdin) != NULL) {                         
        if(lines[count1][0] == '\'') {          
            count1++;
            break;
        }
        count1++;
     }
    count1 = 0;
    
    while (fgets(lines[count1], MAX_WORDS, stdin) != NULL){             // adds exclusion words to bad_words array                      
        
        int length = strlen(lines[count1]);
        if (length > 0 && lines[count1][length-1] == '\n') {
            lines[count1][length-1] = '\0';
        }

        if (lines[count1][0] == '\"'){                                   // exits loop once """" is reached
            break;;
        }
        else{
            strncpy(bad_words[bad_count],lines[count1], MAX_WORD_LEN);   //copies c elements to bad_words, EXCLUDING """"
            bad_count++;
        }
        count1++;
    }
    
        while (fgets(lines[i], MAX_LINES, stdin) != NULL){               //loops through remaining lines until NULL is reached
            num_words = 0;

            int len = strlen(lines[i]);                                 // removes \n from the end of eac line
            if (len > 0 && lines[i][len-1] == '\n') {
                lines[i][len-1] = '\0';
            }
            strncpy(l[i], lines[i], MAX_LINE_LEN);                      // copies line[i] into l[i]

            num_words = 0;
            tokenize_line(l[i]);
            int j = 0;
            int cur_count = 0;
            
            j = 0;
            while (j < num_words){
                cur_count = 0;
                while(cur_count <= bad_count){       //compares words of each line with each bad word 
                    in = 0;
                    if (strncmp(words[j], bad_words[cur_count], str_length(words[j])) != 0) {
                        //printf("words being compared: %s, %s\n", words[j], bad_words[cur_count]);
                        cur_count++;
                    } else {
                        in = 1;
                        break;
                    }
                }
                if (in == 0) {                                                              // if word is not in bad_words, add to indexed words
                    
                    
                    strncpy(indexed_words[indexed_count].word, words[j], MAX_WORD_LEN);     //copies word into indexed_words
                    strncpy(indexed_words[indexed_count].sentence, lines[i], MAX_LINE_LEN); //copies entire sentence into indexed words sentence
                    strncpy(word_order[indexed_count], words[j], MAX_WORD_LEN);
                    indexed_words[indexed_count].word_count = num_words;
                    
                    indexed_count++;
                    
                } 
                j++;
            }
            i++;
     }
        

                                                                                            // this section organizes word_order in alphabetical order
    char temp[MAX_WORD_LEN];
    for(i=0;i<=indexed_count;i++) {

        for(i=0;i<=indexed_count;i++) {

            for(int j=i+1;j<indexed_count;j++){

                if(strcmp(word_order[i],word_order[j])>0){
                    strcpy(temp,word_order[i]);
                    strcpy(word_order[i],word_order[j]);
                    strcpy(word_order[j],temp);
                 }
            }
        }
    }

        //printf("bad_count: %d\n", bad_count);

        //printf("index_count: %d\n", indexed_count);

    capitilize_word(indexed_words, indexed_count);
    print_order(word_order, indexed_words, indexed_count);

 
    fclose(stdout);

    return 0;

}