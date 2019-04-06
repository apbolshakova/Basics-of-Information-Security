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

func_res_t encode(FILE* srcFile, FILE* destFile, size_t dataBitsNum)
{
	size_t parityBitsNum = getParityBitsNum(dataBitsNum);
	size_t blockSizeInBytes = ceil((double)(dataBitsNum + parityBitsNum) / 8);

	size_t textSize = getTextSizeInBits(srcFile);
	size_t blocksNum = ceil((double)textSize / dataBitsNum);
	for (int i = 0; i < blocksNum; i++)
	{
		int encodedBlock = encodeBlock(srcFile, dataBitsNum, parityBitsNum);
		fwrite(&encodedBlock, blockSizeInBytes, 1, destFile);
	}
	return SUCCESS;
}

size_t getParityBitsNum(size_t dataBitsNum)
{
	return (size_t)ceil(log2(dataBitsNum));
}

BOOL isPowerOfTwo(int n)
{
	return (n > 0 && (n & (n - 1)) == 0);
}

size_t getTextSizeInBits(FILE* srcFile)
{
	fseek(srcFile, 0L, SEEK_END);
	return ftell(srcFile) * BITS_IN_BYTE;
}

int encodeBlock(FILE* srcFile, size_t dataBitsNum, size_t parityBitsNum)
{
	//TODO: получать закодированный блок двоичного кода длиной dataBitsNum + parityBitsNum бит
}