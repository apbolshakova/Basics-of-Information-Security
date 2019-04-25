#include "Header.h"

void handleDecoding()
{
	//открыть файл с сообщением
	FILE* srcFile = fopen(ENCODED_NAME, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open file.\n");
		return;
	}

	//Создать файл для результата
	FILE* destFile = fopen(DECODED_NAME, "wb");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create dest file.\n");
		return;
	}

	//Получить размер информационного блока
	size_t dataBitsNum = 0;
	printf("Enter how many bits were processed at one time: ");
	scanf("%i", &dataBitsNum);
	while (dataBitsNum < MIN_BLOCK_SIZE || dataBitsNum > MAX_BLOCK_SIZE || dataBitsNum % VALID_SIZE_DIVIDER)
	{
		printf("Incorrect block size. You must enter one of these - 8, 12, 16, 24, 32, 48, 64: ");
		scanf("%i", &dataBitsNum);
	}

	decode(srcFile, destFile, dataBitsNum);
	printf("Successfully decoded.\n");
}

void decode(FILE* srcFile, FILE* destFile, size_t dataBitsNum)
{
	size_t parityBitsNum = getParityBitsNum(dataBitsNum);
	size_t blockSize = dataBitsNum + parityBitsNum;
	size_t containerSize = getNOK(blockSize, BITS_IN_BYTE); //НОК размера блока и 8

	BOOL dataEnded = FALSE;
	char curCh = getc(srcFile);
	size_t posInContainer = 0; //0..containerSize - 1
	size_t posInCh = 0; //0..BITS_IN_BYTE - 1

	while (!dataEnded)
	{
		//Сформировать контейнер
		char container[MAX_CONTAINER] = { 0 };
		while (posInContainer < containerSize)
		{
			container[posInContainer] = '0';
			if (!dataEnded)
			{
				if (curCh % 2) container[posInContainer] = '1';
				curCh >>= 1;
				posInCh++;
			}
			if (posInCh == BITS_IN_BYTE)
			{
				if ((curCh = getc(srcFile)) == EOF) dataEnded = TRUE;
				posInCh = 0;
			}
			posInContainer++;
		}

		//Проверить блоки данных в контейнере
		for (int i = 0; i < containerSize / blockSize; i++) //цикл по каждому блоку
		{
			//Выделить блок
			size_t posInBlock = 1; //1..blockSize
			char* block = (char*)malloc(blockSize + 2); //+1 для сдвига и +1 для \0
			block[0] = '0';
			strncpy(block + 1, container, blockSize);

			//Получить строку с контрольными битами для проверки
			char* awaitedParityBits = (char*)malloc(blockSize + 2);
			for (int k = 0; k < blockSize + 1; k++) awaitedParityBits[k] = '0';
			awaitedParityBits[blockSize + 1] = '\0';
			while (posInBlock <= blockSize)
			{
				if ((!isPowerOf2(posInBlock) && block[posInBlock] == '1'))
					toggleParityBits(awaitedParityBits, posInBlock);
				posInBlock++;
			}

			//Получить номер бита, для которого не совпадают контрольные и исправить
			int corruptedBit = 0;
			for (int k = 1; k <= parityBitsNum; k++)
			{
				if (block[pow2(k)] != awaitedParityBits[pow2(k)])
					corruptedBit += pow2(k);
			}
			if (block[corruptedBit] == '0') block[corruptedBit] = '1';
			else block[corruptedBit] = '0';
		}
		//Составить исходное сообщение из битов со значащей информацией, напечатать в dest
		posInContainer = 0;

	}
}