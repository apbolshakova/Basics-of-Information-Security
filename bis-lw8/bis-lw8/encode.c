#include "Header.h"

int encode()
{
	//Открыть файл-изображение
	FILE* srcFile = fopen(SRC_FILE_PATH, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open source file.\n");
		_getch();
		return 0;
	}

	//Открыть файл-сообщение
	FILE* textFile = fopen("text.txt", "r");
	if (textFile == NULL)
	{
		printf("ERROR: unable to open file with message.\n");
		_getch();
		return 0;
	}

	//Опредить палитру
	fseek(srcFile, PALETTE_POS_HEX, SEEK_SET);
	if (getc(srcFile) != PALETTE)
	{
		printf("Invalid file. Suggested 24-bit bmp.");
		_getch();
		return 0;
	}

	//Определить количество байтов в изображении, куда можно писать текст
	fseek(srcFile, SIZE_POS_HEX, SEEK_SET);
	int bytesNum = 0;
	for (int i = 0; i < 4; i++)
	{
		char temp = getc(srcFile);
		bytesNum = bytesNum + temp * pow(256, i);
	}
	bytesNum = bytesNum - LEN_NUMBER_SIZE - 1; //первые 9 байт на информацию

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
	if ((double)textSize >= pow(4, LEN_NUMBER_SIZE))
	{
		printf("Unable to encrypt message: too long to save it's size.\n");
		_getch();
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
		_getch();
		return 0;
	}

	//Создать копию файла - изображения
	FILE* destFile = fopen(DEST_FILE_PATH, "w+");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create destination file.\n");
		_getch();
		return 0;
	}
	fseek(srcFile, 0L, SEEK_SET);
	copyFile(srcFile, destFile);
	
	//Зашифровать длину сообщения в первых 8 байтах (по 2 мл. разряда в каждом)
	fseek(destFile, offset, SEEK_SET);
	fseek(textFile, 0L, SEEK_SET);
	char destCh = 0;
	for (int i = 0; i < LEN_NUMBER_SIZE; i++)
	{
		fread(&destCh, sizeof(char), 1, destFile);
		fseek(destFile, -1L, SEEK_CUR);
		for (int j = 0; j < 2; j++)
		{
			if (textSize & (1 << 0)) destCh |= (1 << j);
			else destCh &= ~(1 << j);
			textSize = textSize >> 1;
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
	char temp = getc(textFile);
	while (temp != EOF) //пока есть символы для кодирования
	{
		fread(&destCh, sizeof(char), 1, destFile);
		fseek(destFile, -1L, SEEK_CUR);
		for (int i = 0; i < pack; i++) 
		{
			if (!temp) temp = getc(textFile);
			if (temp == EOF) break;
			//записать последний бит temp в i-ый бит байта изображения
			if (temp & (1 << 0)) destCh |= (1 << i); 
			else destCh &= ~(1 << i);
			temp = temp >> 1;
		}
		fwrite(&destCh, sizeof(char), 1, destFile);
		fflush(destFile);
	}
	fclose(srcFile);
	fclose(textFile);
	fclose(destFile);
	return 1;
}

void copyFile(FILE *ifp, FILE *ofp)
{
	int chread;
	while ((chread = fgetc(ifp)) != EOF)
	{
		char ch = chread;
		putc(ch, ofp);
	}
}