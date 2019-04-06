#include "Header.h"

void handleEncoding()
{
	//открыть файл с сообщением
	FILE* srcFile = fopen(FILE_NAME, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open file.\n");
		return;
	}

	//Создать временный файл
	FILE* tempFile = fopen(TEMP_FILE_NAME, "wb");
	if (tempFile == NULL)
	{
		printf("ERROR: unable to create temp file.\n");
		return;
	}

	size_t dataBitsNum = 0;
	printf("Enter number of bits to process at one time: ");
	scanf("%i", &dataBitsNum);
	while (dataBitsNum < MIN_BLOCK_SIZE || dataBitsNum > MAX_BLOCK_SIZE || dataBitsNum % VALID_SIZE_DIVIDER)
	{
		printf("Incorrect block size. You must enter one of these - 8, 12, 16, 24, 32, 48, 64: ");
		scanf("%i", &dataBitsNum);
	}

	if (encode(srcFile, tempFile, dataBitsNum) == FAIL)
	{
		printf("ERROR: unable to encode message.\n");
		return;
	};

	//удалить исходный файл с сообщением и переименовать temp
	if (!remove(FILE_NAME) || !rename(TEMP_FILE_NAME, FILE_NAME))
	{
		printf("ERROR: unable to write result in source file.\n");
		printf("Result can be found in temp.txt.\n");
		return;
	}
	printf("Successfully encoded.\n");
}

func_res_t encode(FILE* srcFile, FILE* resFile, size_t dataBitsNum)
{
	size_t parityBitsNum = getParityBitsNum(dataBitsNum);
	return SUCCESS;
}

size_t getParityBitsNum(size_t dataBitsNum)
{
	size_t result = log2(dataBitsNum);
	if (!isPowerOfTwo(dataBitsNum)) result++;
	return result;
}

BOOL isPowerOfTwo(int n)
{
	return (n > 0 && (n & (n - 1)) == 0);
}