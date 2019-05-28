#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Constants.h"
#include "Types.h"

/*Main.c*/
char* getSrcFileName();
char* getResFileName();
int* getConfigs();

/*Obfuscator.c*/
status_t handleObfuscation(FILE* fSrc, FILE* fRes, int* config);
char* getSrcCode(FILE* fSrc);
void fprintText(FILE* fRes, char* text, int* config);
void addDummies(char* src);
void changeVarNames(char* src);
//void deleteComments(char* src);
void deleteSpaces(char* src);