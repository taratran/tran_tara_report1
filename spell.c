#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

#define MAX_LINE 1000

//fuction check_word(string word, hashmap hashtable[])
//{
//    Set int bucket to the output of hash_function(word).
//    Set hashmap_t cursor equal to hashmap[bucket].
//    While cursor is not NULL:
//        If word equals cursor->word:
//            return True.
//        Set curosr to cursor->next.
//    Set int bucket to the output of hash_function(word).
//    Set hashmap_t cursor equal to hashmap[bucket].
//    While cursor is  not NULL:
//        If lower_case(word) equals curosr->word:
//            return True.
//        Set curosr to cursor->next.
//    return False.
//}
bool check_word(const char* word, hashmap_t hashtable[]) {
    if (word == NULL || strlen(word) > LENGTH) {
        return false;
    }
    int countDigit = 0;
    for (int i = 0; i < strlen(word); i++) {
        if (isdigit(word[i])) {
            countDigit++;
        }
    }
    // If this is all digit letter
    if (strlen(word) == countDigit) {
        return true;
    }
    // Set int bucket to the output of hash_function(word).
    int bucket = hash_function(word);
    //Set hashmap_t cursor equal to hashmap[bucket].
    hashmap_t cursor = hashtable[bucket];
    //While cursor is not NULL:
    while (NULL != cursor) {
        //If word equals cursor->word:
        if (!strcmp(word, cursor->word)) {
            // return True.
            return true;
        }
        // Set curosr to cursor->next.
        cursor = cursor->next;
    }
    // Store the word low case
    char wordLowcase[LENGTH];
    // Convert to lowcase of letter
    for (int i = 0; i < strlen(word); i++) {
        wordLowcase[i] = tolower(word[i]);
    }
    // Set end string
    wordLowcase[strlen(word)] = '\0';
    // Set int bucket to the output of hash_function(word).
    bucket = hash_function(wordLowcase);
    // Set hashmap_t cursor equal to hashmap[bucket].
    cursor = hashtable[bucket];
    //While cursor is  not NULL:
    while (cursor != NULL) {
        //If lower_case(word) equals cursor->word:
        if (!strcmp(wordLowcase, cursor->word)) {
            //return True.
            return true;
        }
        // Set curosr to cursor->next.
        cursor = cursor->next;
    }
    // return False.
    return false;
}

//function load_dictionary(string dictionary, hashmap hashtable[])
//{
//    Initialize all values in hash table to NULL.
//    Open dict_file from path stored in dictionary.
//    If dict_file is NULL:
//        return false.
//    While word in dict_file is not EOF (end of file):
//        Set hashmap_t new_node to a new node.
//        Set new_node->next to NULL.
//        Set new_node->word equal to word.
//        Set int bucket to hash_function(word).
//        if hashtable[bucket] is NULL:
//            Set hashtable[bucket] to new_node.
//        else:
//            Set new_node->next to hashtable[bucket].
//            Set hashtable[bucket] to new_node.
//    Close dict_file.
//}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    // Initialize all values in hash table to NULL.
    for (int i = 0; i < HASH_SIZE; i++) {
        hashtable[i] = NULL;
    }

    // Open dict_file from path stored in dictionary.
    FILE* dictFile = fopen(dictionary_file, "r");

    // If dict_file is NULL:
    if (NULL == dictFile) {
        // return false.
        return false;
    }

    char dictWord[LENGTH];
    // While word in dict_file is not EOF (end of file):
    while (fscanf(dictFile, "%45s", dictWord) == 1) {
        // Set hashmap_t new_node to a new node.
        hashmap_t newNode = malloc(sizeof(node));
        // Set new_node->next to NULL.
        newNode->next = NULL;
        // Set new_node->word equal to word.
        strncpy(newNode->word, dictWord, strlen(dictWord));
        // Set end of string
        newNode->word[strlen(dictWord)] = '\0';
        // Set int bucket to hash_function(word).
        int bucket = hash_function(newNode->word);
        // if hashtable[bucket] is NULL:
        if (hashtable[bucket] == NULL) {
            // Set hashtable[bucket] to new_node.
            hashtable[bucket] = newNode;
        } else {
            // Set new_node->next to hashtable[bucket].
            newNode->next = hashtable[bucket];
            // Set hashtable[bucket] to new_node.
            hashtable[bucket] = newNode;
        }
    }

    // Close dict_file.
    fclose(dictFile);
    return true;
}

//function check_words(file fp, hashmap hashtable[], string misspelled[])
//{
//    Set int num_misspelled to 0.
//    While line in fp is not EOF (end of file):
//        Read the line.
//        Split the line on spaces.
//        For each word in line:
//            Remove punctuation from beginning and end of word.
//            If not check_word(word):
//                Append word to misspelled.
//                Increment num_misspelled.
//    Return num_misspelled.
//}
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    for (int i = 0; i < MAX_MISSPELLED; i++) {
        misspelled[i] = NULL;
    }
    // Set int num_misspelled to 0.
    int numMisspelled = 0;
    const char *delimeterList = " ,.?!/-;:\"'\n";
    char line[MAX_LINE];
    char *word;
    // While line in fp is not EOF (end of file):
    // Read the line.
    while (fgets(line, MAX_LINE, fp) != NULL) {
        // Split the line on spaces.
        word = strtok(line, delimeterList);
        // For each word in line:
        while (word != NULL) {
            // Remove punctuation from beginning and end of word.
            // If not check_word(word):
            if (!check_word(word, hashtable)) {
                // Append word to misspelled.
                misspelled[numMisspelled] = malloc(sizeof(char) * (LENGTH + 1));
                strncpy(misspelled[numMisspelled], word, LENGTH + 1);
                // Increment num_misspelled.
                numMisspelled++;
            }
            // Take next word
            word = strtok(NULL, delimeterList);
        }
    }
    // Return num_misspelled.
    return numMisspelled;
}
