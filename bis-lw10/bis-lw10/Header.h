#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include "Constants.h"

int getNOD(int n1, int n2);
int getNOK(int n1, int n2);
int pow2(int num);
void toggleParityBits(char* block, size_t num);
BOOL isPowerOf2(int n);
size_t getParityBitsNum(size_t blockSize);
void printAsChars(FILE* dest, char* container, size_t size);
void printResult(FILE* dest, char* container, size_t containerSize, size_t blockSize);

void encode(FILE* srcFile, FILE* resFile, size_t dataBitsNum);
void decode(FILE* srcFile, FILE* destFile, size_t dataBitsNum);

void handleEncoding();
void handleDecoding();