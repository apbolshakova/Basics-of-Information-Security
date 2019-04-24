#include "Header.h"

void handleDecoding()
{
	//������� ���� � ����������
	FILE* srcFile = fopen(ENCODED_NAME, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open file.\n");
		return;
	}

	//������� ���� ��� ����������
	FILE* destFile = fopen(DECODED_NAME, "wb");
	if (destFile == NULL)
	{
		printf("ERROR: unable to create dest file.\n");
		return;
	}

	//�������� ������ ��������������� �����
	size_t dataBitsNum = 0;
	printf("Enter how many bits were processed at one time: ");
	scanf("%i", &dataBitsNum);
	while (dataBitsNum < MIN_BLOCK_SIZE || dataBitsNum > MAX_BLOCK_SIZE || dataBitsNum % VALID_SIZE_DIVIDER)
	{
		printf("Incorrect block size. You must enter one of these - 8, 12, 16, 24, 32, 48, 64: ");
		scanf("%i", &dataBitsNum);
	}

	//������������
	if (decode(srcFile, destFile, dataBitsNum) == FAIL)
	{
		printf("ERROR: unable to encode message.\n");
		return;
	};
	printf("Successfully encoded.\n");
}

func_res_t decode(FILE* srcFile, FILE* destFile, size_t dataBitsNum)
{
	size_t parityBitsNum = getParityBitsNum(dataBitsNum);
	size_t blockSize = dataBitsNum + parityBitsNum;
	size_t containerSize = getNOK(blockSize, BITS_IN_BYTE); //��� ������� ����� � 8

	//������������ ��������� - ������� ������
	//��������� ����������� ���� � ��������� ������
	//��������� �������� ��������� �� ����� �� �������� �����������, ���������� � dest
	return SUCCESS;
}