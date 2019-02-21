/*�������:
����� �� ������� ������ ����� ������� �����
�� ������ ���� �������������� ������
"�������������� �� ������ ������ �����" ��� ��������
������� ������� �������
���������� �� ������ ����� ������������� �����
*/

#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>

#define MENU_TEXT "Menu text" //TODO: �������� ����
#define UNKNOWN_OPERATION_MESSAGE "Unknown operation code. Enter another one: " //TODO: �������� ����


enum OperationCode 
{
	NULL_OPERATION, //�������� ��� �������������
	ANALYZE,        //������ ������� ���� �� ������� ����� � ����� �������������� ����� � ����������� � ��������� ������������� ���� �������� ��������
	PRINT_WORDS,    //����� �� ����� ���� ����, ��������������� �� ���������� ���������������� �� ������ ������ ����
	PRINT_BUF,      //����������� ������������ � ��������� ��������������� �� ������ ������ ������
	REPLACE_LETTERS, //����������� ������ ���� � ������������
	REVERT,         //�������� � ����� ������� ������ ���� � ������������
	AUTOREPLACEMENT,     //�������������� ������ ����
	EXIT 
};

char* getBufFromInput()
{
	//TODO
}

void suggestReplacementsBasingOnFrequencyAnalysis(char* buf)
{
	//TODO
}

void printWordsSortedByUndecipheredLetters(char* buf)
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
}

void replaceLettersAutomatically(char* buf)
{
	//TODO
}

void handleMainCycle(char* buf)
{
	OperationCode operationCode = NULL_OPERATION;
	do 
	{
		printf(MENU_TEXT);
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(buf); break;
		case PRINT_WORDS: printWordsSortedByUndecipheredLetters(buf); break;
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
	char* buf = getBufFromInput(); //�������� ������ �� �������� ����� � buf
	handleMainCycle(buf); //������ ��������� ����� ���������
	return 0;
}