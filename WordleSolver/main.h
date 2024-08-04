#ifndef MAIN_H
#define MAIN_H

// DEBUG
#define DEBUG 0

// SETTINGS
#define MAX_WORD_LENGTH 5
#define MAX_GUESSES 500
#define LANGUAGE "en"
#define INITIAL_WORD "adieu"

// FUNCTIONS
int initialSetup();
int getLetters();
int giveWord();
int getPossibleWords();


#endif