/*
 * concord3.c
 *
 * Starter file provided to students for Assignment #3, SENG 265,
 * Fall 2022.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "emalloc.h"
#include "seng265-list.h"
#include <ctype.h>




/*
DESCRIPTION: goes through the input and adds to a linked list line by line  
PARAMETERS: none
RETURNS: linked list of input lines created
*/
node_t *get_input() {
    
    node_t *temp_node = NULL;
    node_t *head = NULL;
    char line[MAX_LINE_LEN];
    
    if (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        if (line[0] == '1') {
            printf("Input is version 1, concord2.py expected version 2");
            exit(0);    
        }
     }
    while (fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        int len = strlen(line);
        if( line[len-1] == '\n' ){
            line[len-1] = 0;
        }
        temp_node = new_node(line);
        head = add_end(head, temp_node);      
    }
    return head;
}

/*
DESCRIPTION: takes the input linked list and creates exclusion word linked list
PARAMETERS: node_t *input - linked list of input lines
RETURNS: head - of exclusion list created
*/
node_t *get_exclusions(node_t *input) {

    node_t *temp_node = NULL;
    node_t *head = NULL;
    char *stop = "\"\"\"\"";
    input = input->next;                //skips ''''
    char *cur = input->text;
    while(strcmp(cur, stop) != 0) {     //adds to exclusion words until """"
        temp_node = new_node(cur);
        head = add_end(head, temp_node);
        input = input->next;
        cur = input->text;
    }
    return head;
}

/*
DESCRIPTION: takes the input linked list and creates a linked list containing the sentences
PARAMETERS: node_t *input - linked list of input lines
RETURNS: head - of sentence list created
*/
node_t *get_sentences(node_t *input) {

    node_t *temp_node = NULL;
    node_t *head = NULL;
    char *start = "\"\"\"\"";
    char *cur = input->text;

    while (strcmp(cur, start) != 0) {   //skips through exclusion words
        input = input->next;
        cur = input->text;
    }
    input = input->next;                //skips """"
    cur = input->text;
    while(input != NULL) {              //adds each sentence to list
        temp_node = new_node(cur);
        head = add_end(head, temp_node);
        input = input->next;
        cur = input->text;
    }
    return head;
}

/*
DESCRIPTION: takes a char word and capitilizes each character
PARAMETERS: char *word: word that is to be capitilized
RETURNS: nothing
*/
void upper(char *word) {
    int len = strlen(word);
    for (int i = 0; i < len; i++){
        word[i] = toupper(word[i]);
    }
}

/*
DESCRIPTION: compares a word with each exclusion word, determines whether it is an index word
PARAMETERS: char *cur - word to be compared, node_t *exclusion_words - linked list of exclusion words
RETURNS: 0 if t is an exclusion word, 1 if word is an index word
*/
int is_index(char *cur, node_t *exclusion_words, char *sentence) {

    char exclusion_capped[MAX_WORD_LEN];
    char cur_capped[MAX_WORD_LEN];
    
    while(exclusion_words != NULL) {
        char *exclusion = exclusion_words->text;
        strncpy(exclusion_capped, exclusion, MAX_WORD_LEN);
        strncpy(cur_capped, cur, MAX_WORD_LEN);
        upper(exclusion_capped);
        upper(cur_capped);
        if (strcmp(cur_capped, exclusion_capped) == 0) {
            return 0; //cur is an exclusion word
        }
        exclusion_words = exclusion_words->next;
    }
    return 1; //cur is an index word
}





// Got inspiration from:  https://stackoverflow.com/questions/27160073/replacing-words-inside-string-in-c
// and https://stackoverflow.com/questions/20213799/finding-last-occurence-of-string%5C
/* 
DESCRIPTION: finds word in sentence and replaces it with replacement word replace
PARAMETERS: char *sentence - sentence to be searched/changed, char *find - word to be replaced, char *replace - word to replace find with 
RETURNS: dest - sentence with word replaced
*/
char *replaceword (char *sentence, char *find, char *replace) {
    
    char sent_copy[MAX_LINE_LEN];
    char *dest = malloc (strlen(sentence)-strlen(find)+strlen(replace)+1);
    strcpy (dest, sentence);
    strncpy(sent_copy, sentence, MAX_LINE_LEN);
    int length = strlen(sent_copy);
    int word_len = strlen(find);
    char *location;
    int position;

    for (location = sent_copy + length; location >= sent_copy; location--) {
        if (strncmp(location, find, word_len) == 0) {
            break;
        }
    }

    position = (int) (location - sent_copy);
    strncpy(&dest[position], replace, word_len);
    
    return dest;
}

/*
DESCRIPTION: creates an in-order linked list of index_node structs containing capitilized sentences their associated index word
PARAMETERS: char *sentence - sentence to be indexed and added, node_t *exclusion words - list of exclusion words, 
index_node *head - head of new index_node list being created
RETURNS: head - of the created index_node in_order list
*/
index_node *add_indexes (char *sentence, node_t *exclusion_words, index_node *head) {
    
    index_node *temp_node = NULL;
    char *t;
    char *capped_sentence;
    char sentence_og[MAX_LINE_LEN];
    strncpy(sentence_og, sentence, MAX_LINE_LEN);
    t = strtok (sentence, " ,");
    
    while (t) {
        int value = is_index(t, exclusion_words, sentence_og);
        if (value == 1) {
            char replace[MAX_WORD_LEN];
            strncpy(replace, t, MAX_WORD_LEN);
            upper(replace);
            capped_sentence = replaceword(sentence_og, t, replace);
            temp_node = new_index_node(t, capped_sentence, replace);
            head = add_inorder(head, temp_node);
    }
        t = strtok (NULL, " ,");
    }
    return head;
}


/*
DESCRIPTION: passes a each sentence to add_indexes to be indexed and added in_order
PARAMETERS: node_t *sentences - linked list of sentences, node_t *exclusion_words - linked list of exclusion words
RETURNS: head - of in-order linked list created
*/
index_node *create_index_nodes(node_t *sentences, node_t *exclusion_words) {

    index_node *head = NULL;

    while (sentences != NULL) {

        char *cur_sentence = sentences->text;
        head = add_indexes(cur_sentence, exclusion_words, head);
        sentences = sentences->next;
    }
    return head;
}


/*
DESCRIPTION: takes the in-order linked list of indexes, cuts and adds spaces where necessary
PARAMETERS: index_node *indexes - in-order linked list of index_nodes with sentences and index words
RETURNS: nothing, prints output
*/
void align(index_node *indexes) {

    int num_spaces;
    char space[1] = " ";
    
    while(indexes != NULL) {

        char *temp = NULL;
        char sentence[MAX_LINE_LEN];
        char word[MAX_WORD_LEN];
        strncpy(sentence, indexes->sentence, MAX_LINE_LEN);
        strncpy(word, indexes->capped_word, MAX_WORD_LEN);
        char *location = strstr(sentence, word); //get location of index word in the sentence
        int position = (int) (location - sentence);
        num_spaces = 29 - position;
        
        int cut = 0;
        if (strlen(sentence) > (position + 31)) {
            sentence[position + 32] = '\0';
            cut = 1;
        }
        
        if (((sentence[strlen(sentence) - 1]) != ' ' || (sentence[strlen(sentence) - 2]) != ' ' ) && (cut == 1)) {
            
            while (sentence[strlen(sentence) - 1] != ' ') {
                sentence[strlen(sentence) - 1] = '\0';
            }
            if ((sentence[strlen(sentence) -1]) == ' ') {
                sentence[strlen(sentence) - 1] = '\0';
            }
        }

        if (position > 20) {
            int difference = position - 20;
            temp = (char*)sentence;
            num_spaces = 8;
            int i = 0;
            
            while (i < difference - 1) {                //remove characters before position - 20
                temp = temp + 1;
                i++;
            }

            if (temp[0] != space[0]) {                  //if splits sentence in middle of word
                while(temp[0] != space[0]) {
                    temp = temp + 1;
                    num_spaces++;
                }
            }
            if ((temp[strlen(temp) - 1]) == ' ') {      //if last character is a space, deletes until not a space
                while (temp[strlen(temp) - 1] == ' ') {
                    sentence[strlen(sentence) - 1] = '\0';
                }
            }
            printf("%*c%s\n", num_spaces, ' ', temp);
        } else {
            printf("%*c%s\n", num_spaces, ' ', sentence);
        }
        indexes = indexes->next; 
    }
}

/*
DESCRIPTION: frees linked lists of type node_t
PARAMETERS: node_t *head - linked list to be
RETURNS: nothing
*/
void release(node_t *head) {
    
    node_t *temp_node = NULL;
    temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);
        head = remove_front(head);
        free(temp_node);
        temp_node = head;
    }
    assert(head == NULL); 
}

/*
DESCRIPTION: frees linked lists of type index_node
PARAMETERS: index_node *head - list to be freed
RETURNS: nothing
*/
void freedom(index_node *head) {
        
    index_node *temp_node = NULL;
    temp_node = head;
    while (temp_node != NULL) {
        assert(temp_node != NULL);            
        head = remove_first(head);
        free(temp_node);
        temp_node = head;
    }
    assert(head == NULL);
}


int main(int argc, char *argv[]) {

    node_t *input = get_input();
    node_t *exclusion_words = get_exclusions(input);
    node_t *sentences = get_sentences(input);

    index_node *indexes = create_index_nodes(sentences, exclusion_words);
    
    align(indexes);
    release(input);
    release(exclusion_words);
    release(sentences);
    freedom(indexes);

exit(0);

}
