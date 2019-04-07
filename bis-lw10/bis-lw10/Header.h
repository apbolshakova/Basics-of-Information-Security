#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include "Constants.h"

typedef enum FuncRes_ { FAIL, SUCCESS } func_res_t;

int pow2(int num);
void toggleParityBits(int* blockWithParityBits, size_t num);
void handleBit(int* dest, size_t posInDest, char src, size_t posInSrc);
int encodeBlock(FILE* srcFile, size_t dataBitsNum, size_t parityBitsNum,
	char* ch, size_t* posInLastCh);
size_t getTextSizeInBits(FILE* srcFile);
BOOL isPowerOf2(int n);
size_t getParityBitsNum(size_t blockSize);

func_res_t encode(FILE* srcFile, FILE* resFile, size_t dataBitsNum);
func_res_t decode();

void handleEncoding();
void handleDecoding();