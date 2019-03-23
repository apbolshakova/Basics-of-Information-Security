#include "Header.h"

int decode()
{
	//Открыть зашифрованный файл
	FILE* srcFile = fopen(DEST_FILE_PATH, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to source file with message.\n");
		return 0;
	}

	//Создать файл для сообщения
	FILE* textFile = fopen(DECODED_FILE_PATH, "w");
	if (textFile == NULL)
	{
		printf("ERROR: unable to create file for decoding result.\n");
		return 0;
	}

	//Определить положение 1-го пикселя в изображении
	fseek(srcFile, OFFSET_POS_HEX, SEEK_SET);
	int offset = getc(srcFile);
	fseek(srcFile, offset, SEEK_SET);

	//Получить длину сообщения из первых 8-ми байтов
	int size = 0;
	int digit = 0;
	char destCh = 0;
	for (int i = 0; i < LEN_NUMBER_SIZE; i++)
	{
		fread(&destCh, sizeof(char), 1, srcFile);
		for (int j = 0; j < 2; j++)
		{
			if (destCh & (1 << j)) size = size + pow(2, digit);
			digit++;
		}
	}

	//Получить плотность упаковки
	int pack = 0;
	fread(&destCh, sizeof(char), 1, srcFile);
	for (int j = 0; j < PACK_SIZE; j++)
	{
		if (destCh & (1 << j)) pack = pack + pow(2, j);
	}

	//Формировать каждый символ из 8 битов до тех пор, пока не будет прочитано длина сообщения символов
	return 1;
}