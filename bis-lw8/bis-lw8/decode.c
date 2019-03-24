#include "Header.h"

int decode()
{
	//������� ������������� ����
	FILE* srcFile = fopen(DEST_FILE_PATH, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to source file with message.\n");
		return 0;
	}

	//������� ���� ��� ���������
	FILE* textFile = fopen(DECODED_FILE_PATH, "w");
	if (textFile == NULL)
	{
		printf("ERROR: unable to create file for decoding result.\n");
		return 0;
	}

	//���������� ��������� 1-�� ������� � �����������
	fseek(srcFile, OFFSET_POS_HEX, SEEK_SET);
	int offset = getc(srcFile);
	fseek(srcFile, offset, SEEK_SET);

	//�������� ����� ��������� �� ������ 8-�� ������
	int size = 0;
	int digit = 0;
	char destCh = 0;
	for (int i = 0; i < LEN_SIZE_BYTE; i++)
	{
		fread(&destCh, sizeof(char), 1, srcFile);
		for (int j = 0; j < LEN_SIZE / LEN_SIZE_BYTE; j++)
		{
			if (destCh & (1 << j)) size |= (1 << digit);
			digit++;
		}
	}

	//�������� ��������� ��������
	int pack = 0;
	fread(&destCh, sizeof(char), 1, srcFile);
	for (int j = 0; j < PACK_SIZE; j++)
	{
		if (destCh & (1 << j)) pack |= (1 << j);
	}

	//����������� ������ ������ �� 8 �����
	int numOfDecodedChars = 0;
	int decodedBits = 0;
	int decodingPosition = 0;
	char decChar = 0;
	fread(&destCh, sizeof(char), 1, srcFile);
	while (numOfDecodedChars < size)
	{
		while (decodedBits < 8)
		{
			if (destCh & (1 << decodingPosition)) decChar |= (1 << decodedBits);
			else decChar &= ~(1 << decodedBits);
			decodingPosition++;
			decodedBits++;

			if (decodingPosition >= pack) 
			{
				fread(&destCh, sizeof(char), 1, srcFile);
				decodingPosition = 0;
			}
		}
		fwrite(&decChar, sizeof(char), 1, textFile);
		decodedBits = 0;
		++numOfDecodedChars;
	}

	fclose(srcFile);
	fclose(textFile);
	return 1;
}