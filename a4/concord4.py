import sys
import re
import fileinput

class concord:

    def __init__(self, input=None, output=None):
        self.input = input
        self.output = output
        self.__new_output()


    """
    DESCRIPTION: writes/prints the list returned by full_concordance to the output file for driver-new (input/output != None) 
    ----------- 
    PARAMETERS: None
    -----------
    RETURNS:    Nothing
    """ 
    def __new_output(self):

        if (self.output != None):
            output_list = []
            output_list = self.full_concordance()
            
            with open(self.output, "w") as f:
                f.write('\n'.join(output_list))
                f.write("\n")

    """
    DESCRIPTION:  performs concordance functions on the input to created a list of strings corresponding to the required output lines
    ----------- 
    PARAMETERS: None 
    -----------
    RETURNS:    list[str] sorted_list: sorted list of index sentences with the correct required alignment
    """ 
    def full_concordance(self):
        
        exclusion_words = []    
        sentences = []
        self.__organize_input(exclusion_words, sentences)
        
        sorted_dicts = self.__sorted_dicts(sentences, exclusion_words)
        locations_list = self.__find_index(sorted_dicts)
        sorted_list = self.__make_list(sorted_dicts)
        self.__align(sorted_list, locations_list)

        return sorted_list

    """
    DESCRIPTION: reads through the input and organizes each line into a lists of exclusion words and sentences
    ----------- 
    PARAMETERS: (list[str]) sentences: list to be populated with input sentences 
                (list[str]) exclusion_words:  list to be populated  the input exclusion words
    -----------
    RETURNS:    Nothing
    """ 
    def __organize_input(self, exclusion_words, sentences):
        on_sentences = False
        for line_og in fileinput.input(files=(self.input)):
            line = line_og.rstrip()
            if line and line != '\n':
                if (line[0] == "\'" or on_sentences == False):
                    on_sentences = False
                    if (line[0] != "\'" and line[0] != "2"):
                        exclusion_words.append(line)
                if (line[0] == '\"' or on_sentences == True ):
                    if (line[0] != '\"'):
                        sentences.append(line)
                    on_sentences = True
       
    """
    DESCRIPTION: finds each index word by comparing each sentence with exclusion words, creates a dictionary for each index word 
                 by calling __capped_dict, and creates a list of dictionaries alphabetically sorted by index
    -----------
    PARAMETERS: (list[str]) sentences: list of input sentences
                (list[str]) exclusion_words: list of input exclusion words 
    -----------
    RETURNS:    list[dict] sorted_dicts: sorted list of dictionaries containing each index word and its corresponding capped sentence
    """ 
    def __sorted_dicts(self, sentences, exclusion_words):
        dict_list = []
        for sentence in sentences:
            words = re.split(r'\s', str(sentence))
            for word in words:
                include = True
                for exclusion in exclusion_words:
                    word_lower = word.lower()
                    exclusion_lower = exclusion.lower()
                    if word_lower == exclusion_lower:
                        include = False  
                if include == True:
                    dict_list.append(self.__capped_dict(word, words))      
        sorted_dicts = sorted(dict_list, key=lambda i: i['word'])
        return sorted_dicts


    """
    DESCRIPTION: capitilizes the index word in sentence and adds each word and sentence to a dictionary
    ----------- 
    PARAMETERS: (str) word: the current index word
                (str) words: sentence containing the current index word
    -----------
    RETURNS:    (dict) dict: dictionary containing each index word and its corresponding capped sentence
    """ 
    def __capped_dict(self, word, words):

        string = ' '.join([str(j) for j in words])
        wordu = word.upper()
        capped_sentence = re.sub(r"\b{}\b".format(str(word)), wordu, string) 
        dict = {'word': wordu, 'sentence': capped_sentence}
        return dict

    """
    DESCRIPTION: creates a list of integers corresponding to the location the index word in each sentence
    ----------- 
    PARAMETERS: list[dict] sorted_dicts: sorted list of dictionaries containing index words and corresponding sentences 
    -----------
    RETURNS:    list[str] location_list: list of indexes forresponding to the location of the index word in each sentence
    """ 
    def __find_index(self, sorted_dicts):

        location_list = []
        for dict in sorted_dicts:
            sentence = dict['sentence']
            word = dict['word'].upper()
            string = ''.join([str(j) for j in sentence])
            index = string.find(word)
            location_list.append(index)
        return location_list


    """
    DESCRIPTION: takes a sorted list of dictionaries and adds each dictionary's sentence to a list
    ----------- 
    PARAMETERS: list[dict] sorted_dicts: sorted list of dictionaries containing index words and corresponding sentences 
    -----------
    RETURNS:    list[str] sorted_list: sorted list of index sentences
    """ 
    def __make_list(self, sorted_dicts):
        
        sorted_list = []
        for dict in sorted_dicts:
            cur_sentence = dict['sentence']
            string = ''.join([str(j) for j in cur_sentence])
            sorted_list.append(string)
        return sorted_list


    """
    DESCRIPTION: aligns and trims sentences to be between column 10 and 60 with each index word at column 30
    ----------- 
    PARAMETERS: (list[str]) sorted_list: sorted list of index sentences
                (list[int]) locations_list: list of indexes forresponding to the location of the index word in each sentence
    ----------- 
    RETURNS:    Nothing
    """ 
    def __align(self, sorted_list, locations_list):  
        i = 0
        for string in sorted_list:
            index = locations_list[i]
            num_spaces = 29 - index
            if index > 20:
                string = self.__change_front(string, index)
            else:
                 string = " " * num_spaces + string
            if len(string) > 60:
                string = self.__change_back(string) 
            if string[-1] == ' ':
                string = re.sub(".$", "", string)
            sorted_list[i] = string
            i += 1
    

    """
    DESCRIPTION: removes characters from the front of the string and until column 10 or until nearest whole word
    ----------- 
    PARAMETERS: (str) string: a string with index word at a column > 20 needing to be shortened at the front
                (int) index: integer correseponding to the index of the index word in string 
    ----------- 
    RETURNS:    (str) string: original string shortened to column 60 or right-most whole word
    """ 
    def __change_front(self, string, index):
        num_spaces = 9
        start = index - 20
        string_start = string[start:]
        if string[start-1] != ' ':
            while string_start[0] != ' ':
                string_start = re.sub("^.", "", string_start)
                num_spaces += 1
        string = " " * num_spaces + string_start
        return string


    """
    DESCRIPTION: removes characters from the back of the string up until column 60 or the end of the nearest whole word
    ----------- 
    PARAMETERS: (str) string: a string with length > 60 needing to be shortened
    ----------- 
    RETURNS:    (str) string: original string shortened to column 60 or right-most whole word
    """
    def __change_back(self, string):
        
        string = string[:61]
        if string[-1] != ' ':
            while string[-1] != " ":
                string = re.sub(".$", "", string)
        return string
            