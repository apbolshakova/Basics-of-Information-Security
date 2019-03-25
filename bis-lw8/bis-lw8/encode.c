#include "Header.h"

int encode()
{
	//Открыть файл-изображение
	FILE* srcFile = fopen(SRC_FILE_PATH, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open source file.\n");
		return 0;
	}

	//Открыть файл-сообщение
	FILE* textFile = fopen(TEXT_FILE_PATH, "r");
	if (textFile == NULL)
	{
		printf("ERROR: unable to open file with message.\n");
		return 0;
	}

	//Опредить палитру
	fseek(srcFile, PALETTE_POS_HEX, SEEK_SET);
	if (getc(srcFile) != PALETTE)
	{
		printf("Invalid file. Suggested 24-bit bmp.");
		return 0;
	}

	//Определить количество байтов в контейнере
	fseek(srcFile, SIZE_POS_HEX, SEEK_SET);
	int bytesNum = 0;
	for (int i = 0; i < FILE_SIZE_LEN; i++) 
	{
		int temp = getc(srcFile);
		bytesNum = bytesNum + temp * pow(BIN_BASE, BITS_IN_BYTE * i);
	}
	bytesNum = bytesNum - (LEN_SIZE_BYTE + PACK_SIZE_BYTE); //первые 9 байт на информацию

	//Определить положение 1-го пикселя в изображении
	fseek(srcFile, OFFSET_POS_HEX, SEEK_SET);
	int offset = getc(srcFile);

	//Получить степень сжатия
	int pack = 0;
	printf("Enter how many bits can be replaced for encryption (1..7): ");
	scanf("%i", &pack);
	while (!(MIN_PACK <= pack && pack <= MAX_PACK))
	{
		printf("Invalid number: it must be from 1 to 7. Enter another one: ");
		scanf("%i", &pack);
	}
	
	//Получить размер текста сообщения в байтах
	fseek(textFile, 0L, SEEK_END);
	int textSize = ftell(textFile);
	if ((double)textSize >= pow(BIN_BASE, LEN_SIZE))
	{
		printf("Unable to encrypt message: too long to save it's size.\n");
		return 0;
	}

	//Проверить, достаточно ли битов в контейнере
	int maxMesLen = bytesNum * pack;
	int mesSize = textSize * BITS_IN_BYTE;
	if (mesSize > maxMesLen)
	{
		printf("Unable to encrypt message: image is too small.\n");
		printf("Message must contains no more than %i chars. ", maxMesLen / BITS_IN_BYTE );
		printf("Current message contains %i chars.\n", textSize);
		return 0;
	}

	//Создать копию файла - изображения
	FILE* destFile = fopen(DEST_FILE_PATH, "wb+");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create destination file.\n");
		return 0;
	}
	fseek(srcFile, 0L, SEEK_SET);
	fseek(destFile, 0L, SEEK_SET);
	CopyFile(SRC_FILE_PATH, DEST_FILE_PATH,FALSE);
	
	//Зашифровать длину сообщения в первых 8 байтах (по 2 мл. разряда в каждом)
	fseek(destFile, offset, SEEK_SET);
	fseek(textFile, 0L, SEEK_SET);
	char destCh = 0;
	int txtSize = textSize;
	for (int i = 0; i < LEN_SIZE_BYTE; i++)
	{
		fread(&destCh, sizeof(char), 1, destFile);
		fseek(destFile, -1L, SEEK_CUR);
		for (int j = 0; j < LEN_SIZE / LEN_SIZE_BYTE; j++)
		{
			if (txtSize & (1 << 0)) destCh |= (1 << j);
			else destCh &= ~(1 << j);
			txtSize = txtSize >> 1;
		}
		fwrite(&destCh, sizeof(char), 1, destFile);
		fflush(destFile);
	}
	
	//Зашифровать плотность упаковки
	int sav = pack;
	fread(&destCh, sizeof(char), 1, destFile);
	fseek(destFile, -1L, SEEK_CUR);
	for (int i = 0; i < PACK_SIZE; i++)
	{
		if (sav & (1 << 0)) destCh |= (1 << i);
		else destCh &= ~(1 << i);
		sav = sav >> 1;
	}
	fwrite(&destCh, sizeof(char), 1, destFile);
	fflush(destFile);

	//Зашифровать текст
	char temp = fgetc(textFile);
	fread(&destCh, sizeof(char), 1, destFile);
	fseek(destFile, -1L, SEEK_CUR);
	int encodedBits = 0;
	int encodingPos = 0;
	while (textSize) //пока есть символы для кодирования
	{
		while (encodedBits < BITS_IN_BYTE)
		{
			if (temp & (1 << 0)) destCh |= (1 << encodingPos);
			else destCh &= ~(1 << encodingPos);
			temp = temp >> 1;
			encodingPos++;
			encodedBits++;
			
			if (encodingPos >= pack) //перейти на след. канал
			{
				fwrite(&destCh, sizeof(char), 1, destFile);
				fflush(destFile);
				fread(&destCh, sizeof(char), 1, destFile);
				fseek(destFile, -1L, SEEK_CUR);
				encodingPos = 0;
			}
		}
		encodedBits = 0;
		temp = fgetc(textFile);
		--textSize;
	}
	fclose(srcFile);
	fclose(textFile);
	fclose(destFile);
	return 1;
}