#include "Header.h"

void handleEncoding()
{
	//открыть файл с сообщением
	FILE* srcFile = fopen(SRC_NAME, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open file.\n");
		return;
	}

	//Создать файл для результата
	FILE* destFile = fopen(ENCODED_NAME, "wb");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create dest file.\n");
		return;
	}

	//Получить размер информационного блока
	size_t dataBitsNum = 0;
	printf("Enter number of bits to process at one time: ");
	scanf("%i", &dataBitsNum);
	while (dataBitsNum < MIN_BLOCK_SIZE || dataBitsNum > MAX_BLOCK_SIZE || dataBitsNum % VALID_SIZE_DIVIDER)
	{
		printf("Incorrect block size. You must enter one of these - 8, 12, 16, 24, 32, 48, 64: ");
		scanf("%i", &dataBitsNum);
	}
	encode(srcFile, destFile, dataBitsNum);
	printf("Successfully encoded.\n");
	fclose(srcFile);
	fclose(destFile);
}

void encode(FILE* srcFile, FILE* destFile, size_t dataBitsNum)
{
	size_t parityBitsNum = getParityBitsNum(dataBitsNum);
	size_t blockSize = dataBitsNum + parityBitsNum;
	size_t containerSize = getNOK(blockSize, BITS_IN_BYTE); //НОК размера блока и 8
	
	BOOL dataEnded = FALSE;
	char curCh = getc(srcFile);
	size_t posInContainer = 0; //0..containerSize - 1
	size_t posInBlock = 1; //1..blockSize
	size_t posInCh = 0; //0..BITS_IN_BYTE - 1

	while (!dataEnded)
	{
		char* container = (char*)malloc(containerSize * sizeof(char));
		char* block = (char*)malloc((blockSize + 1) * sizeof(char));
		while (posInContainer < containerSize)
		{
			if (!isPowerOf2(posInBlock) && !dataEnded)
			{
				if (curCh % 2)
				{
					block[posInBlock] = '1';
					toggleParityBits(block, posInBlock);
				}
				else block[posInBlock] = '0';
				curCh >>= 1;
				posInCh++;
			}
			else block[posInBlock] = '0';
			posInBlock++;

			if (posInBlock == blockSize + 1)
			{
				strncpy(container + posInContainer + 1 - blockSize, block + 1, blockSize);
				free(block);
				block = (char*)malloc((blockSize + 1) * sizeof(char));
				posInBlock = 1;
			}

			if (posInCh == BITS_IN_BYTE)
			{
				if ((curCh = getc(srcFile)) == EOF) dataEnded = TRUE;
				posInCh = 0;
			}
			posInContainer++;
		}
		printAsChars(destFile, container, containerSize);
		free(container);
		free(block);
		posInContainer = 0;
	}
}

void printAsChars(FILE* dest, char* container, size_t size)
{
	char* result = (char*)malloc(size / BITS_IN_BYTE * sizeof(char) + 1);
	char* temp = result;
	char ch = 0;
	size_t posInCh = 0;
	for (int i = 0; i < size; i++)
	{
		if (container[i] == '1') ch = ch | (1 << posInCh);
		else ch = ch & ~(1 << posInCh);

		if (posInCh + 1 == BITS_IN_BYTE)
		{
			*temp = ch;
			temp++;
			ch = 0;
			posInCh = 0;
		}
		else posInCh++;
	}
	*temp = '\0';
	fwrite(result, size / BITS_IN_BYTE, 1, dest);
	free(result);
}