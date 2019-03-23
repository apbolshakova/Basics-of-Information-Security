#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "conio.h"
#include "math.h"

#define SRC_FILE_PATH "src.bmp"
#define DEST_FILE_PATH "result.bmp"
#define TEXT_FILE_PATH "text.txt"

#define OFFSET_POS_HEX 0x0A //����� �������, ������� � �������� ���� ����� �����������
#define PALETTE_POS_HEX 0x1C //����� �������
#define SIZE_POS_HEX 0x22 //����� ������� ����������

#define PALETTE 24
#define MIN_PACK 1
#define MAX_PACK 7
#define BITS_IN_BYTE 8
#define LEN_NUMBER_SIZE 8 //���������� ��� ��� �������� ����� �������������� ������

void copyFile(FILE *ifp, FILE *ofp);

int main(void)
{
	//������� ����-�����������
	FILE* srcFile = fopen(SRC_FILE_PATH, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open source file.\n");
		_getch();
		return 0;
	}

	//������� ����-���������
	FILE* textFile = fopen("text.txt", "r");
	if (textFile == NULL)
	{
		printf("ERROR: unable to open file with message.\n");
		_getch();
		return 0;
	}

	//�������� �������
	fseek(srcFile, PALETTE_POS_HEX, SEEK_SET);
	if (getc(srcFile) != PALETTE)
	{
		printf("Invalid file. Suggested 24-bit bmp.");
		_getch();
		return 0;
	}

	//���������� ���������� ��-�� � �����������, ��� ����� ������������ ����������
	fseek(srcFile, SIZE_POS_HEX, SEEK_SET);
	int size = 0;
	for (int i = 0; i < 4; i++) size = size + getc(srcFile) * pow(16, i); //TODO: �����������

	//���������� ��������� 1-�� ������� � �����������
	fseek(srcFile, OFFSET_POS_HEX, SEEK_SET);
	int offset = getc(srcFile);

	//�������� ������� ������
	int pack = 0;
	printf("Enter how many bits can be replaced for encryption (1..7): ");
	scanf("%i", &pack);
	while (!(MIN_PACK <= pack && pack <= MAX_PACK))
	{
		printf("Invalid number: it must be from 1 to 7. Enter another one: ");
		scanf("%i", &pack);
	}
	
	//�������� ������ ������ ��������� � ������
	fseek(textFile, 0L, SEEK_END);
	int textSize = ftell(textFile);
	if ((double)textSize >= pow(2, LEN_NUMBER_SIZE))
	{
		printf("Unable to encrypt message: too long to save it's size (max = 255 chars).\n");
		_getch();
		return 0;
	}

	//���������, ���������� �� �������� ���������� ��� ������ � ������
	int maxMesLen = LEN_NUMBER_SIZE + size * pack; //�� 1 �� ����� � �� pack �� �������
	int mesSize = LEN_NUMBER_SIZE + textSize * BITS_IN_BYTE;
	if (mesSize > maxMesLen)
	{
		printf("Unable to encrypt message: image is too small.\n");
		printf("Message must contains no more than %i chars.", (maxMesLen - LEN_NUMBER_SIZE) / BITS_IN_BYTE); 
		printf("Current message contains %i chars.\n", (mesSize - LEN_NUMBER_SIZE) / BITS_IN_BYTE);
		_getch();
		return 0;
	}

	//������� ����� ����� - �����������
	FILE* destFile = fopen(DEST_FILE_PATH, "w+");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create destination file.\n");
		_getch();
		return 0;
	}
	fseek(srcFile, 0L, SEEK_SET);
	copyFile(srcFile, destFile);

	//����������� ��������� � ��� �����
	fseek(destFile, offset, SEEK_SET);
	fseek(textFile, 0L, SEEK_SET);

	//� 8 ������ ���� ��� ����������� �� ������� �������� ����� ������
	for (int i = 0; i < LEN_NUMBER_SIZE; i++)
	{
		char destCh = 0;
		fscanf(destFile, "%c", &destCh);

		if (textSize & (1 << 0)) destCh |= (1 << i);
		else destCh &= ~(1 << i);
		textSize = textSize >> 1;

		fseek(destFile, -1L, SEEK_CUR);
		fprintf(destFile, "%x", destCh);
		fseek(destFile, 1L, SEEK_CUR);
	}

	char temp = getc(textFile);
	while (temp != EOF) //���� ���� ������� ��� �����������
	{
		char destCh = 0;
		fscanf(destFile, "%c", &destCh);
		for (int i = 0; i < pack; i++) 		//� (0..7 - pack) ���� ������ �������� pack ����� �� ������ temp (if temp) 
		{
			if (!temp) temp = getc(textFile);
			if (temp == EOF) break;

			if (temp & (1 << 0)) destCh |= (1 << i); //�������� 1 � i-�� ��� �������� ������� �� destFile
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