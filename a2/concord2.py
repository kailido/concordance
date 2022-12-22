#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from fileinput import filename
from operator import index
from typing import List, Dict

import sys
import re 
import datetime

"""
DESCRIPTION
-----------  
loops sentence by sentence, compares each word of the current sentence with 
each exclusion word. If word is not an exclusion word, word and sentence are 
added as a dictionary to a list of dictionaries with the index word capitilized

PARAMETERS: 
-----------
sentences: list[str]
list of sentences from the input
exclusion_words: list[str]
list of exclusion words from the input 

RETURNS: 
-----------
sorted_dicts: list[dict]
a list of index dictionaries sorted alphabetically by word
each dictionary contains an index word and its associated sentence

"""


def create_index_list(sentences, exclusion_words):

    dict_list = []
    for sentence in sentences:
        words = sentence.split()
        for word in words:
            include = True
            for exclusion in exclusion_words:
                word_lower = word.lower()
                exclusion_lower = exclusion.lower()
                if word_lower == exclusion_lower:
                    include = False  
            if include == True:
                capped_sentence = capitilize(words, word)
                word = word.upper()
                dict = {'word': word, 'sentence': capped_sentence}
                dict_list.append(dict)
    sorted_dicts = sorted(dict_list, key=lambda i: i['word'])
    return sorted_dicts

"""
DESCRIPTION
-----------  
takes current sentence and index word from create_index_list, 
capilitizes index word and replaces it in original sentence

PARAMETERS: 
-----------
sentence: str
current sentence passed by create_index_list

index: str
current index word passed by create_index_list

RETURNS: 
-----------
sorted_dicts: list[dict]
a list of index dictionaries sorted alphabetically by word

"""

def capitilize(sentence, index):
    
    capped_sentence = []
    for word in sentence:
        if word == index:
            word = word.upper()
            capped_sentence.append(word)
        else:
            capped_sentence.append(word)
    return capped_sentence

"""
DESCRIPTION
-----------  
finds the index location of each index word in its associated sentence 
and appends the index to location_list

PARAMETERS: 
-----------
sorted_dicts: list[dict]
contains index words and corresponding sentences sorted alphabetically

RETURNS: 
-----------
location_list: list[int]
a list of index locations in order corresponding to sorted_dicts

"""
def find_index(sorted_dicts):

    location_list = []
    for dict in sorted_dicts:
        sentence = dict['sentence']
        word = dict['word'].upper()
        string = ' '.join([str(j) for j in sentence])
        index = string.find(word)
        location_list.append(index)
    return location_list

"""
DESCRIPTION
-----------  
aligns index word of each sentence with column 30
if beginning of sentence exceeds column 10, trims and adds spaces to the beginning of the sentence to align with column 10
if end of sentence exceeds column 60, trims the end of sentence to align with column 60
after each line is properly aligned, it is printed

PARAMETERS: 
-----------
sorted_dicts: list[dict]
contains index words and corresponding sentences sorted alphabetically
locations_list: list[int]
list containing the index location of each index word

RETURNS: 
-----------
nothing

"""
def align_and_print(sorted_dicts, locations_list):

    
    i = 0
    for dict in sorted_dicts:

        cur_sentence = dict['sentence']
        index = locations_list[i]
        string = ' '.join([str(j) for j in cur_sentence])
        length = len(string)
        num_spaces = 29 - index
        
        if index > 20:
            num_spaces = 9
            start = index - 20
            string_start = string[start:]
            if string[start-1] != ' ' and string[start-2] != ' ':
                while string_start[0] != ' ':
                    string_start = string_start[1:]
                    num_spaces += 1
            string = string_start

        string = " " * num_spaces + string
        length = len(string)
        if length > 60:
            string = string[:61]

            if string[-1] != ' ' or string[-2] != " ":
                while string[-1] != " ":
                    string = string[:-1]

        if string[-1] == ' ':
            string = string[:-1]
  
        print(string)
        i += 1


def main():
    
    exclusion_words = []    # list[str] to hold the exclusion words from input
    sentences = []          # list[str] to hold the sentences from input
    sorted_dicts = []       # list[dict] to hold each index word and corresponding sentence sorted alphabetically
    locations_list = []     # list[int] to hold indexes corresponding to index word locations in their corresponding sentence
    on_sentences = False
    

    # loops through each line of input and organizes exclusion words and sentences into separate lists
    for line_og in sys.stdin:

        line = line_og.rstrip()
        
        # if input file is version 1, print statement and exit
        if line == '1':
            print("Input is version 1, concord2.py expected version 2")
            sys.exit()
        
        if line_og != '\n':
            
            if (line[0] == "\'" or on_sentences == False):
                exclusion_words.append(line)
                on_sentences = False

            if (line[0] == '\"' or on_sentences == True or line[0] == '\n'):
                sentences.append(line)
                on_sentences = True

    
    # removes 2, """", and '''' from the lists
    exclusion_words = exclusion_words[2:-1]
    sentences = sentences[1:]
    
    # sorted_dicts populated and returned by create_index_list
    sorted_dicts = create_index_list(sentences, exclusion_words)

    # locations_list populated and returned by find_index
    locations_list = find_index(sorted_dicts)
    
    # aligns and print index sentences in alphabetical order
    align_and_print(sorted_dicts, locations_list)  

if __name__ == '__main__':
    main()