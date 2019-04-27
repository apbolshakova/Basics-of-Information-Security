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
	fclose(srcFile);
	fclose(destFile);
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
	
	char decodedCh = 0;
	size_t posInDecodedCh = 0;

	while (!dataEnded)
	{
		//Сформировать контейнер
		char* container = (char*)malloc(containerSize * sizeof(char));
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
			strncpy(block + 1, container + blockSize * i, blockSize);

			//Получить строку с контрольными битами для проверки
			char* awaitedParityBits = (char*)malloc(blockSize + 2);
			for (int k = 0; k < blockSize + 1; k++) awaitedParityBits[k] = '0';
			while (posInBlock <= blockSize)
			{
				if ((!isPowerOf2(posInBlock) || posInBlock == pow2(parityBitsNum)) 
					&& block[posInBlock] == '1')
					toggleParityBits(awaitedParityBits, posInBlock);
				posInBlock++;
			}

			//Получить номер бита, для которого не совпадают контрольные и исправить
			int corruptedBit = 0;
			for (int k = 0; k < parityBitsNum; k++)
			{
				if (block[pow2(k)] != awaitedParityBits[pow2(k)])
					corruptedBit += pow2(k);
			}
			if (corruptedBit)
			{
				if (block[corruptedBit] == '0') container[corruptedBit + (blockSize - 1)* i] = '1';
				else container[corruptedBit + (blockSize - 1)* i] = '0';
			}
			free(block);
			free(awaitedParityBits);
		}
		//Составить исходное сообщение из битов со значащей информацией, напечатать в dest
		printResult(destFile, container, containerSize, blockSize, parityBitsNum, &decodedCh, &posInDecodedCh);
		free(container);
		container = NULL;
		posInContainer = 0;
	}
}

void printResult(FILE* dest, char* container, size_t containerSize, size_t blockSize, size_t parityBitsNum, char* ch, size_t* posInCh)
{
	char* result = (char*)malloc(containerSize * sizeof(char));
	char* temp = result;
	size_t size = 0; //размер полученного сообщения
	size_t posInBlock = 1; //1..blockSize
	size_t posInContainer = 0; //0..containerSize - 1

	while (posInContainer < containerSize)
	{
		if (!isPowerOf2(posInBlock) || posInBlock == pow2(parityBitsNum)) //это информационный бит
		{
			if (container[posInContainer] == '1') *ch = *ch | (1 << *posInCh);
			else *ch = *ch & ~(1 << *posInCh);
			(*posInCh)++;
		}
		posInBlock++;
		if (posInBlock == blockSize + 1) posInBlock = 1;
		if (*posInCh == BITS_IN_BYTE)
		{
			if (*ch)
			{
				*temp = *ch;
				temp++;
				size++;
			}
			*ch = 0;
			*posInCh = 0;
		}
		posInContainer++;
	}
	fwrite(result, size, 1, dest);
	free(result);
}