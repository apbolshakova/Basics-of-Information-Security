/*�������:
����� �� ������� ������ ����� ������� �����
�� ������ ���� �������������� ������
"�������������� �� ������ ������ �����" ��� ��������
������� ������� ������� � �� ��� ������ ���� ������
���������� �� ������ ����� ������������� �����
*/

#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define UNKNOWN_OPERATION_MESSAGE "����������� ��� �������, ���������� ������: "

enum OperationCode 
{
	NULL_OPERATION,              //�������� ��� �������������
	ANALYZE,                     //������ ������� ���� �� ������� ����� � ����� �������������� ����� � ����������� � ��������� ������������� ���� �������� ��������
	PRINT_WORDS_BY_LENGTH,       //����� �� ����� ���� ����, ��������������� �� ���������� ����
	PRINT_WORDS_BY_UNDECIPHERED, //����� �� ����� ���� ����, ��������������� �� ���������� ���������������� �� ������ ������ ����
	PRINT_BUF,                   //����������� ������������ � ��������� ��������������� �� ������ ������ ������
	REPLACE_LETTERS,             //����������� ������ ���� � ������������
	REVERT,                      //�������� � ����� ������� ������ ���� � ������������
	AUTOREPLACEMENT,             //�������������� ������ ����
	EXIT 
};

typedef struct Cryptogram
{
	char* curText; //����� � �������� �������� ��� ������
	char** word; //������ ���������-���� ��� ������
	char* srcLetter; //������ �������� ���� C, D, A, F ��� �������
	char* curLetter; //������ ����� '','', b, '' -> A �������� �� b. ����� ����� - �������� �������� �� ����� �������
} CRYPTOGRAM;

CRYPTOGRAM* initCryptogram()
{
	return 0;
}

void printMainMenu()
{
	printf("��� ������������ ���������, ����������� ������� ����������� ���������������. ��������� ��������:\n");
	printf("1. ������������� ������� ���� � ���������� ������\n");
	printf("2. ������� �����, ��������������� �� ���������� ����\n");
	printf("3. ������� �����, ��������������� �� ���������� ���������������� ����\n");
	printf("4. ���������� ������������\n");
	printf("5. �������� �����\n");
	printf("6. �������� ������\n");
	printf("7. ���������� �������������� ������\n");
	printf("������� ��� ������ �������: ");
}

void suggestReplacementsBasingOnFrequencyAnalysis(char* buf)
{
	//TODO
}

void printWordsOrderByLength(char* buf)
{
	//TODO
}

void printWordsOrderByUndeciphered(char* buf)
{
	//TODO
}

void printBuf(char* buf)
{
	//TODO
}

void handleReplacementMenu(char* buf)
{
	//TODO
}

void printHistoryAndRevertBuf(char* buf)
{
	//TODO
	//������� ������ ���������� ����� � ���������� ����������� ������
}

void replaceLettersAutomatically(char* buf)
{
	//TODO
}

void handleMainCycle(CRYPTOGRAM* data)
{
	OperationCode operationCode = NULL_OPERATION;
	do 
	{
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(buf); break;
		case PRINT_WORDS_BY_LENGTH: printWordsOrderByLength(buf); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsOrderByUndeciphered(buf); break;
		case PRINT_BUF: printBuf(buf);  break;
		case REPLACE_LETTERS: handleReplacementMenu(buf); break; //������ ����� ������ ����� �� ���� �� ��� => ��������� �������
		case REVERT: printHistoryAndRevertBuf(buf); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(buf); break;
		case EXIT: break;
		default: printf(UNKNOWN_OPERATION_MESSAGE);
		}
		printf("\n\n"); //��� ��������� ����������� ��������
	} while (operationCode != EXIT);
}

int main(void)
{
	CRYPTOGRAM* data = initCryptogram();
	if (data->srcLetter == 0) //� ���������� ������ ��� ����
	{
		printf("���������� ������ �� �������� ��� �����������: ����������� �����.");
		_getch();
		return 0;
	}
	handleMainCycle(data);
	return 0;
}