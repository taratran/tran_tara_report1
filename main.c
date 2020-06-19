#include <stdlib.h>
#include "dictionary.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Used: %s <text file need check> <dictionary file>.\n", argv[0]);
        printf("E.g: spell_check a_tale_of_two_cities.txt wordlist.txt\n");
        return 1;
    }
    // Try to load the dictionary
    printf("Load the dictionary\n");
    char* dicFileName = argv[2];
    hashmap_t hashtable[HASH_SIZE];
    if (!load_dictionary(dicFileName, hashtable)) {
        printf(
                "Error: ERROR when loading the dictionary. Please check your input.\n");
        return 1;
    }
    printf("Load dictionary successfully from file: %s\n", dicFileName);

    char* testFileName = argv[1];
    // Open the test file
    FILE* fp = fopen(testFileName, "r");
    if (fp == NULL) {
        printf(
                "Error: Can not open the test file: %s. Please check your input.\n",
                testFileName);
        return 1;
    }
    char* misspelled[MAX_MISSPELLED];
    int numMisspelled = check_words(fp, hashtable, misspelled);
    printf("Misspelled number: %d in file name: %s\n", numMisspelled,
            testFileName);
    for (int i = 0; i < numMisspelled; i++) {
        printf("\t%s\n", misspelled[i]);
    }
    // Close file
    fclose(fp);
    // Clean memory
    hashmap_t currentElement;
    for (int i = 0; i < HASH_SIZE; i++) {
        if (hashtable[i] != NULL) {
            currentElement = hashtable[i];
            while (currentElement != NULL) {
                hashmap_t tmp = currentElement->next;
                free(currentElement);
                currentElement = tmp;
            }
        }
    }
    for (int i = 0; i < numMisspelled; i++) {
        free(misspelled[i]);
    }

    return 0;
}

