#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#include "Constants.h"
#include "Types.h"

bool_t isCapitalLetter(char item);
bool_t isLowercaseLetter(char item);
bool_t isLetter(char item);
bool_t isUsedAsReplacement(char symbolToCheck, letter_t* letter);
bool_t areSameWords(char* a, char* b);
bool_t wordIsUnique(words_list_item_t* newWord, words_list_item_t* list);

char fixCodeForCyrillicChar(char item);
char getCyrrilicLetterOrExitSymbol();

void initLetters(letter_t* letter);
changes_list_item_t* initChangesList();

int getSizeOfText(char* text);
void copyStringAndGetEncounters(cryptogram_t* data, char* str);
void handleDataFromString(cryptogram_t* data, char* str);
void initTextAndGetEncounters(cryptogram_t* data, FILE* f);
void getFrequencies(cryptogram_t* data);

void handleWordData(words_list_item_t* newWord, cryptogram_t* data);
void addWordToList(cryptogram_t* data);
void parseTextIntoWords(cryptogram_t* data);

cryptogram_t* initCryptogram();

void printText(cryptogram_t* data);
void printEncryptionKey(letter_t* letter);

letter_t* findMaxFrqFromUndesiphered(letter_t* letter);
char findMaxFrqFromNotReplacement(letter_t* letter);
void printLettersFrequencies(letter_t* letter);

void printReplacementSuggestion(letter_t* letter);
void suggestReplacement(cryptogram_t* data);

void getNumOfUndesiphered(cryptogram_t* data);

words_list_item_t* sortWordsByLen(words_list_item_t* firstWord);
words_list_item_t* sortWordsByUndeciphered(words_list_item_t* firstWord);

void printCharsWithEncryption(char* ptr, cryptogram_t* data);
void printWords(printing_operation_code_t order, cryptogram_t* data);
void handleWordsPrintingMenu(cryptogram_t* data);

void printCryptogram(cryptogram_t* data);

void addElementToHistory(char srcLetter, char letterForReplacement, cryptogram_t* data);

void replaceLetter(char srcLetter, char letterForReplacement, cryptogram_t* data);
void handleReplacementMenu(cryptogram_t* data);

void undoLastChange(cryptogram_t* data);
void redoLastChange(cryptogram_t* data);
void deleteLastChange(cryptogram_t* data);
void handleRevertMenu(cryptogram_t* data);

void replaceAndUpdateHistoryAuto(cryptogram_t* data);
void handleAutoreplacement(cryptogram_t* data);

void printMainMenu();
void handleMainCycle(cryptogram_t* data);