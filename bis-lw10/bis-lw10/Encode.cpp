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

	//Закодировать
	if (encode(srcFile, destFile, dataBitsNum) == FAIL)
	{
		printf("ERROR: unable to encode message.\n");
		return;
	};
	printf("Successfully encoded.\n");
}

func_res_t encode(FILE* srcFile, FILE* destFile, size_t dataBitsNum)
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
		char container[MAX_CONTAINER] = { 0 };
		char block[MAX_ENCODED_BLOCK] = { 0 };
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
				posInBlock = 1;
			}

			if (posInCh == BITS_IN_BYTE)
			{
				if ((curCh = getc(srcFile)) == EOF) dataEnded = TRUE;
				posInCh = 0;
			}
			posInContainer++;
		}
		posInContainer = 0;
		printAsChars(destFile, container, containerSize);
	}
	return SUCCESS;
}

size_t getParityBitsNum(size_t dataBitsNum)
{
	size_t result = (size_t)ceil(log2(dataBitsNum));
	if (result < MIN_PARITY_BITS) result = MIN_PARITY_BITS; //для обработки по 8 бит
	return result;
}

BOOL isPowerOf2(int n)
{
	return (n > 0 && (n & (n - 1)) == 0);
}

void toggleParityBits(char* block, size_t num)
{
	size_t curPos = 0;
	while (num)
	{
		if ((num & (0x01 << 0)) != 0)
			if (block[pow2(curPos)] == '0') block[pow2(curPos)] = '1';
			else block[pow2(curPos)] = '0';
		num >>= 1;
		curPos++;
	}
}

int pow2(int num)
{
	if (num == 0) return 1;
	return 2 << (num - 1);
}

int getNOD(int n1, int n2)
{
	int div;
	if (n1 == n2)  return n1;
	int d = n1 - n2;
	if (d < 0) {
		d = -d;  div = getNOD(n1, d);
	}
	else
		div = getNOD(n2, d);
	return div;
}

int getNOK(int n1, int n2)
{
	return n1 * n2 / getNOD(n1, n2);
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