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
	for (int i = 0; i < LEN_NUMBER_SIZE; i++)
	{
		fread(&destCh, sizeof(char), 1, srcFile);
		for (int j = 0; j < 2; j++)
		{
			if (destCh & (1 << j)) size = size + pow(2, digit);
			digit++;
		}
	}

	//�������� ��������� ��������
	int pack = 0;
	fread(&destCh, sizeof(char), 1, srcFile);
	for (int j = 0; j < PACK_SIZE; j++)
	{
		if (destCh & (1 << j)) pack = pack + pow(2, j);
	}

	//����������� ������ ������ �� 8 ����� �� ��� ���, ���� �� ����� ��������� ����� ��������� ��������
	return 1;
}