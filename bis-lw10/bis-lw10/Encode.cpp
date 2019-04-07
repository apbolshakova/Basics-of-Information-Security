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
	fseek(srcFile, 0L, SEEK_SET);
	size_t blocksNum = ceil((double)textSize / dataBitsNum);
	char lastCh = getc(srcFile);
	size_t posInLastCh = 0; //индекс бита для обработки в последнем считанном символе
	for (int i = 0; i < blocksNum; i++)
	{
		int encodedBlock = encodeBlock(srcFile, dataBitsNum, parityBitsNum,
			                           &lastCh, &posInLastCh);
		if (encodedBlock == INVALID_BLOCK)
		{
			printf("ERROR: unexpected EOF.\n");
			return FAIL;
		}
		fwrite(&encodedBlock, blockSizeInBytes, 1, destFile); //TODO: узнать, что делать, когда записать нужно бы 12 бит, а можно 16
	}
	return SUCCESS;
}

size_t getParityBitsNum(size_t dataBitsNum)
{
	return (size_t)ceil(log2(dataBitsNum));
}

BOOL isPowerOf2(int n)
{
	return (n > 0 && (n & (n - 1)) == 0);
}

size_t getTextSizeInBits(FILE* srcFile)
{
	fseek(srcFile, 0L, SEEK_END);
	return ftell(srcFile) * BITS_IN_BYTE;
}

int encodeBlock(FILE* srcFile, size_t dataBitsNum, size_t parityBitsNum, 
	char* ch, size_t* posInCh)
{
	int encodedBlock = 0;
	for (int posInBlock = 1; posInBlock <= dataBitsNum + parityBitsNum; posInBlock++)
	{
		if (!isPowerOf2(posInBlock))
		{
			handleBit(&encodedBlock, posInBlock, *ch, *posInCh); //копировать текущий бит
			*posInCh = ((*posInCh) + 1) % BITS_IN_BYTE; //сдвиг на след. позицию в символе
			if (*posInCh == 0)
				if ((*ch = getc(srcFile)) == EOF) return INVALID_BLOCK;
		}
	}
	encodedBlock >>= 1; //в алгоритме биты нумеруются с 1, => нулевой бит не обрабатывался
	return encodedBlock;
}

void handleBit(int* dest, size_t posInDest, char src, size_t posInSrc)
{
	if ((src & (0x01 << posInSrc)) != 0)
	{
		*dest |= 0x01 << posInDest;
		toggleParityBits(dest, posInDest);
	}
	else *dest &= ~(0x01 << posInDest);
}

void toggleParityBits(int* blockWithParityBits, size_t num)
{
	size_t curPos = 0;
	while (num)
	{
		if ((num & (0x01 << 0)) != 0)
			*blockWithParityBits = (*blockWithParityBits) ^ (0x01 << pow2(curPos));
		num >>= 1;
		curPos++;
	}
}

int pow2(int num)
{
	if (num == 0) return 1;
	return 2 << (num - 1);
}