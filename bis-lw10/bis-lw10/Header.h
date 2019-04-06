#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include "Constants.h"

typedef enum FuncRes_ { FAIL, SUCCESS } func_res_t;

int encodeBlock(FILE* srcFile, size_t dataBitsNum, size_t parityBitsNum);
size_t getTextSizeInBits(FILE* srcFile);
BOOL isPowerOfTwo(int n);
size_t getParityBitsNum(size_t blockSize);

func_res_t encode(FILE* srcFile, FILE* resFile, size_t dataBitsNum);
func_res_t decode();

void handleEncoding();
void handleDecoding();