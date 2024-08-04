#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

char libraryName[10];
char libraryPath[20];

char letters[MAX_WORD_LENGTH + 1];
char positions[MAX_WORD_LENGTH + 1];
unsigned int incorrectLetters[26] = {0};

char **guesses = NULL;
int guessCount = 1;

char **incorrectWords = NULL;
int incorrectWordCount = 0;

int solved = 0;


int initialSetup() {

    if (DEBUG) printf("[DEBUG] initialSetup()\n");
    if (DEBUG) printf("[DEBUG] MAX_WORD_LENGTH: %d\n", MAX_WORD_LENGTH);

    sprintf(libraryName, "%d_%s.txt", MAX_WORD_LENGTH, LANGUAGE);
    
    if (DEBUG) printf("[DEBUG] Setting up the library as %s\n", libraryName);

    sprintf(libraryPath, "Libraries/%s", libraryName);

    //Populate the incorrectLetters array with the alphabet
    for (size_t i = 0; i < 26; i++) {
        incorrectLetters[i] = 'a' + i;
    }

    // Set the initial guess
    guesses = (char **)malloc((unsigned int)guessCount * sizeof(char *));
    if (guesses == NULL) {
        perror("Failed to allocate memory");
        return -1;
    }
    guesses[0] = (char *)malloc(MAX_WORD_LENGTH * sizeof(char));
    if (guesses[0] == NULL) {
        perror("Failed to allocate memory");
        free(guesses);
        return -1;
    }
    strcpy(guesses[0], INITIAL_WORD);

    return 0;
}


int giveWord() {
    if (DEBUG) printf("[DEBUG] giveWord()\n");

    printf("A likely word is: %s\t\n", guesses[0]);

    char response;
    int i = 0;

    do {
        printf("Do you need a different word?\n");
        scanf(" %c", &response);
        if (response == 'y' || response == 'Y') {
            // Add the current word to the incorrectWords array
            char **temp = realloc(incorrectWords, (unsigned int)(incorrectWordCount + 1) * sizeof(char *));
            if (temp == NULL) {
                perror("Failed to reallocate memory");
                return -1;
            }
            incorrectWords = temp;
            incorrectWords[incorrectWordCount] = strdup(guesses[i]);
            if (incorrectWords[incorrectWordCount] == NULL) {
                perror("Failed to allocate memory for word");
                return -1;
            }
            incorrectWordCount++;

            i++;
            printf("A likely word is: %s\t\n", guesses[i]);
        }
    } while (response == 'y' || response == 'Y');

    printf("Is the word correct?\n");
    scanf(" %c", &response);
    if (response == 'y' || response == 'Y') {
        printf("The word is %s\n", guesses[i]);
        solved = 1;
    }
    return 0;
}


int getLetters() {

    if (DEBUG) printf("[DEBUG] getLetters()\n");

    // Get the correct letters
    printf("Enter the correct Letters\n->");
    scanf(" %s", letters);

    // Check if the input length is greater than MAX_WORD_LENGTH
    if (strlen(letters) > MAX_WORD_LENGTH) {
        printf("[ERROR] Input is longer than %d letters.\n", MAX_WORD_LENGTH);
        return -1;
    }

    if (DEBUG) printf("[DEBUG] strlen(letters): %d\n", strlen(letters));

    //Get the incorrect letters
    for (size_t i = 0; i < strlen(guesses[0]); i++) {
        if (strchr(letters, guesses[0][i]) == NULL) {
            incorrectLetters[guesses[0][i] - 'a'] = 1;
        }
    }

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
        printf("[ERROR] Enter some letters to receive possible words\n");
        return 0;
    }

    FILE *library = fopen(libraryPath, "r");
    if (library == NULL) {
        printf("[ERROR] Could not open library file\n");
        return -1;
    }

    char word[MAX_WORD_LENGTH + 1];
    int index = 0;

    while (fscanf(library, " %s", word) != EOF) {
        if (DEBUG) printf("[DEBUG] Word: %s\n", word);

        if (strlen(word) != strlen(guesses[0])) {
            continue;
        }
        if (DEBUG) printf("[DEBUG] Word length matches guess length\n");

        if (DEBUG) printf("[DEBUG] Checking if word contains correct letters\n");
        int containsAllLetters = 1;
        int letterCounts[26] = {0}; // Array to count occurrences of each letter in the word
        int requiredCounts[26] = {0}; // Array to count occurrences of each letter in the given letters

        // Count occurrences of each letter in the word
        for (size_t i = 0; i < strlen(word); i++) {
            letterCounts[word[i] - 'a']++;
        }

        // Count occurrences of each letter in the given letters
        for (size_t i = 0; i < strlen(letters); i++) {
            requiredCounts[letters[i] - 'a']++;
        }

        // Check if the word contains the correct number of each letter
        for (size_t i = 0; i < 26; i++) {
            if (letterCounts[i] < requiredCounts[i]) {
                containsAllLetters = 0;
                if (DEBUG) printf("[DEBUG] Word does not contain all correct letters\n");
                break;
            }
        }

        if (!containsAllLetters) {
            continue;
        }

        if (DEBUG) printf("[DEBUG] Checking if word contains incorrect letters\n");
        // Check if the word contains any of the incorrect letters
        int containsIncorrectLetters = 0;
        for (size_t i = 0; i < strlen(word); i++) {
            if (incorrectLetters[word[i] - 'a'] == 1) {
                containsIncorrectLetters = 1;
                if (DEBUG) printf("[DEBUG] Word contains incorrect letters\n");
                break;
            }
        }
        if (containsIncorrectLetters) {
            continue;
        }

        if (DEBUG) printf("[DEBUG] Checking if word matches correct positions\n");
        // Check if the word contains the correct letters in the correct positions
        int correctPositions = 1;
        for (size_t i = 0; i < strlen(word); i++) {
            if (positions[i] != '-' && word[i] != positions[i]) {
                correctPositions = 0;
                if (DEBUG) printf("[DEBUG] Word does not match correct positions\n");
                break;
            }
        }
        if (!correctPositions) {
            continue;
        }

        if (DEBUG) printf("[DEBUG] Possible word: %s\n", word);

        // Check if the word is already in the guesses array or incorrectWords array
        int alreadyExists = 0;
        for (int i = 0; i < index; i++) {
            if (strcmp(guesses[i], word) == 0) {
                alreadyExists = 1;
                break;
            }
        }
        for (int i = 0; i < incorrectWordCount; i++) {
            if (strcmp(incorrectWords[i], word) == 0) {
                alreadyExists = 1;
                break;
            }
        }
        if (alreadyExists) {
            continue;
        }

        // Correct reallocation of guesses array and handling potential memory leak
        char **temp = realloc(guesses, (unsigned int)(index + 1) * sizeof(char*));
        if (temp == NULL) {
            perror("Failed to reallocate memory");
            fclose(library);
            return -1;
        }
        guesses = temp;

        guesses[index] = strdup(word); // Store the word in guesses array
        if (guesses[index] == NULL) {
            perror("Failed to allocate memory for word");
            fclose(library);
            return -1;
        }
        index++;
    }

    guessCount = index;
    fclose(library);

    if (DEBUG) printf("[DEBUG] guessCount: %d\n", guessCount);

    return 0;
}


int main() {
    if (DEBUG) printf("[DEBUG] main()\n");

    initialSetup();

    do {
        printf("\n");

        if (DEBUG) printf("[DEBUG] entering main loop\n");

        giveWord();
        if (solved) {
            break;
        }
        getLetters();
        getPossibleWords();

        printf("<-------------------->\n");

    } while (!solved);

    // Free the guesses array
    for (int i = 0; i <guessCount; i++) {
        free(guesses[i]);
    }
    free(guesses);

    
    // Free the incorrectWords array
    for (int i = 0; i < incorrectWordCount; i++) {
        free(incorrectWords[i]);
    }
    free(incorrectWords);

    return 0;
}