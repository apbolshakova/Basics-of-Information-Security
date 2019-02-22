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

void suggestReplacementsBasingOnFrequencyAnalysis(CRYPTOGRAM* data)
{
	//TODO
}

void printWordsOrderByLength(char** word)
{
	//TODO
}

void printWordsOrderByUndeciphered(char** word)
{
	//TODO
}

void printText(char* text)
{
	//TODO
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	//TODO
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	//TODO
	//������� ������ ���������� ����� � ���������� ����������� ������
}

void replaceLettersAutomatically(CRYPTOGRAM* data)
{
	//TODO
}

void handleMainCycle(CRYPTOGRAM* data)
{
	OperationCode operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsOrderByLength(data->word); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsOrderByUndeciphered(data->word); break;
		case PRINT_BUF: printText(data->curText); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		case EXIT: break;
		default: printf(UNKNOWN_OPERATION_MESSAGE);
		}
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