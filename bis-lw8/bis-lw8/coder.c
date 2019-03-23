#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "conio.h"
#include "math.h"

#define SRC_FILE_PATH "src.bmp"
#define DEST_FILE_PATH "result.bmp"
#define TEXT_FILE_PATH "text.txt"

#define OFFSET_POS_HEX 0x0A //адрес отступа, начиная с которого идут байты изображения
#define PALETTE_POS_HEX 0x1C //адрес палитры
#define SIZE_POS_HEX 0x22 //адрес размера контейнера

#define PALETTE 24
#define MIN_PACK 1
#define MAX_PACK 7
#define BITS_IN_BYTE 8
#define LEN_NUMBER_SIZE 8 //количество пар бит для хранения длины зашифрованного текста
#define PACK_SIZE 3 //количество бит для хранения степени упаковки

void copyFile(FILE *ifp, FILE *ofp);

int main(void)
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

	//Определить количество байтов в изображении, где будет произведение перезапись
	fseek(srcFile, SIZE_POS_HEX, SEEK_SET);
	int bytesNum = 0;
	for (int i = 0; i < 4; i++) bytesNum = bytesNum + getc(srcFile) * pow(16, i);

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
	if ((double)textSize >= pow(2, LEN_NUMBER_SIZE * 2))
	{
		printf("Unable to encrypt message: too long to save it's size.\n");
		_getch();
		return 0;
	}

	//Проверить, достаточно ли эл-ов в контейнере
	int maxMesLen = bytesNum * pack; //размер сообщения + степень упаковки + символы
	int mesSize = textSize * BITS_IN_BYTE;
	if (mesSize > maxMesLen)
	{
		printf("Unable to encrypt message: image is too small.\n");
		printf("Message must contains no more than %i chars.", maxMesLen / BITS_IN_BYTE); 
		printf("Current message contains %i chars.\n", mesSize / BITS_IN_BYTE);
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

	for (int i = 0; i < LEN_NUMBER_SIZE; i++)
	{
		char destCh = 0;
		fscanf(destFile, "%c", &destCh);

		for (int j = 0; j < 2; j++)
		{
			if (textSize & (1 << 0)) destCh |= (1 << i);
			else destCh &= ~(1 << i);
			textSize = textSize >> 1;
		}
		fseek(destFile, -1L, SEEK_CUR);
		fprintf(destFile, "%x", destCh);
		fseek(destFile, 1L, SEEK_CUR);
	}

	//Зашифровать плотность упаковки
	int sav = pack;
	char destCh = 0;
	fscanf(destFile, "%c", &destCh);
	for (int i = 0; i < PACK_SIZE; i++)
	{
		if (sav & (1 << 0)) destCh |= (1 << i);
		else destCh &= ~(1 << i);
		sav = sav >> 1;
	}
	fseek(destFile, -1L, SEEK_CUR);
	fprintf(destFile, "%x", destCh);
	fseek(destFile, 1L, SEEK_CUR);

	//Зашифровать текст
	char temp = getc(textFile);
	while (temp != EOF) //пока есть символы для кодирования
	{
		char destCh = 0;
		fscanf(destFile, "%c", &destCh);
		for (int i = 0; i < pack; i++) 
		{
			if (!temp) temp = getc(textFile);
			if (temp == EOF) break;
			//записать 1 в i-ый бит текущего символа из destFile
			if (temp & (1 << 0)) destCh |= (1 << i); 
			else destCh &= ~(1 << i);
			temp = temp >> 1;
		}
		fseek(destFile, -1L, SEEK_CUR);
		fprintf(destFile, "%c", destCh);
		fseek(destFile, 1L, SEEK_CUR);
	}
	_getch();
	return 0;
}

void copyFile(FILE *ifp, FILE *ofp)
{
	char c;
	while ((c = getc(ifp)) != EOF)
	{
		putc(c, ofp);
	}
}