#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

char libraryName[10];
char libraryPath[20];

char letters[MAX_WORD_LENGTH + 1];
char positions[MAX_WORD_LENGTH + 1];
char guess[MAX_WORD_LENGTH + 1] = INITIAL_WORD;
int numGuesses = 0;

int solved = 0;

int initialSetup() {

    if (DEBUG) printf("[DEBUG] initialSetup()\n");
    if (DEBUG) printf("[DEBUG] MAX_WORD_LENGTH: %d\n", MAX_WORD_LENGTH);

    sprintf(libraryName, "%d_%s.txt", MAX_WORD_LENGTH, LANGUAGE);
    
    if (DEBUG) printf("[DEBUG] Setting up the library as %s\n", libraryName);

    sprintf(libraryPath, "Libraries/%s", libraryName);

    return 0;
}

int probability(char *word) {

    if (DEBUG) printf("[DEBUG] probability()\n");

    int score = 0;
    for (size_t i = 0; i < strlen(word); i++) {
        if (word[i] == 'a' || word[i] == 'e' || word[i] == 'i' || word[i] == 'o' || word[i] == 'u') {
            score++;
        }
    }

    return 0;
}

int giveWord() {

    if (DEBUG) printf("[DEBUG] giveWord()\n");

    printf("A likely word is: %s\t\n", guess);

    return 0;
}

int getLetters() {

    if (DEBUG) printf("[DEBUG] getLetters()\n");

    // Get the correct letters
    printf("Enter the correct Letters\n->");
    scanf(" %s", letters);

    // Check if the input length is greater than MAX_WORD_LENGTH
    if (strlen(letters) > MAX_WORD_LENGTH) {
        printf("Error: Input is longer than %d letters.\n", MAX_WORD_LENGTH);
        return -1;
    }

    if (DEBUG) printf("[DEBUG] strlen(letters): %d\n", strlen(letters));

    // Get the positions of the letters
    printf("Enter the correct Positions for following Letters\n(Use \"-\" for unknown spots)\n");

    // Print each letter followed by a comma
    for (size_t i = 0; i < strlen(letters); i++) {
        // If it's the last letter, print a colon instead of a comma
        if (i == strlen(letters) - 1) {
            printf("%c; ", letters[i]);
        } else {
            printf("%c, ", letters[i]);
        }
    }

    printf("\n->");

    scanf(" %s", positions);
    

    if (DEBUG) {
        printf("[DEBUG] Letters: %s\n", letters);
        printf("[DEBUG] Positions: %s\n", positions);
        for (size_t i = 0; i < strlen(positions); i++) {
            if (positions[i] != '-') {
                printf("[DEBUG] Position %d: %c\n", i, positions[i]);
            }
        }
    }

    return 0;
}

int getPossibleWords() {

    if (DEBUG) printf("[DEBUG] getPossibleWords()\n");

    if (letters[0] == '\0') {
        printf("Enter some letters to recieve possible words\n");
    }
    
    if (letters[0] != '\0') {

        FILE *library = fopen(libraryPath, "r");

        char word[MAX_WORD_LENGTH + 1];

        if (DEBUG) printf("[DEBUG] Opening library file %s\n", libraryName);

        if (library == NULL) {
            printf("Error: Could not open library file\n");
            return -1;
        }

        while (fscanf(library, " %s", word) != EOF) {
            if (DEBUG) printf("[DEBUG] Word: %s\n", word);

        }

        fclose(library);
    }

    return 0;
}

int main() {

    if (DEBUG) printf("[DEBUG] main()\n");

    initialSetup();

    do {
        printf("\n");

        if (DEBUG) printf("[DEBUG] entering main loop\n");

        giveWord();
        getLetters();
        getPossibleWords();

        printf("<-------------------->\n");

    } while (!solved);

    return 0;
}