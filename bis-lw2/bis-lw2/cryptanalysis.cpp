/*�������:
����� �� ������� ������ ����� ������� �����
�� ������ ���� �������������� ������
"�������������� �� ������ ������ �����" ��� ��������
������� ������� ������� � �� ��� ������ ���� ������
���������� �� ������ ����� ������������� �����
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENT 0
#define EMPTY_TEXT 0
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "��������������������������������"
#define ALPHABET "��������������������������������"

#define UNKNOWN_OPERATION_MESSAGE "����������� ��� �������, ���������� ������: "
#define INVALID_DATA_MESSAGE "���������� ������ �� �������� ��� �����������: ����������� �����."

typedef enum 
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
} OPERATION_CODE;

typedef enum 
{ 
	FALSE, 
	TRUE 
} BOOL;

typedef struct
{
	int encounteredInSrcText; //������� ��� ���������� � ������ - ��� ������������ �������
	float frequencyInSrcText; //���������� ��� ������������� - � ������ �������
	char replacedTo;
} LETTER;

typedef struct
{
	char* text; //����� � �������� �������� ��� ������
	LETTER* letter;
} CRYPTOGRAM;

void initLetters(CRYPTOGRAM* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->encounteredInSrcText = 0;
		(data->letter + i)->frequencyInSrcText = 0;
		(data->letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

BOOL isLetter(char item)
{
	if (item <= '�' && item <= '�') return TRUE;
	else return FALSE;
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str)
{
	int sizeOfOldText = sizeof(data->text); 
	data->text = (char*) realloc(data->text, sizeOfOldText + SIZE_OF_STRING_TO_COPY * sizeof(char));

	char* item = data->text + sizeOfOldText - 1; //�����, ������� � �������� ���������� ����� �������
	while (*str)
	{
		*item = *str;
		if (isLetter(*item))
		{
			(data->letter + (*item - 'A'))->encounteredInSrcText++;
		}
		item++;
		str++;
	}
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char) + ALPHABET_SIZE * sizeof(LETTER));
	//data->letter = (LETTER*)calloc(ALPHABET_SIZE, sizeof(LETTER));
	//data->text = (char*) malloc(sizeof(char));
	*(data->text) = EMPTY_TEXT;

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fscanf(f, "%s") != EOF))
	{
		initLetters(data);
		fclose(f);
		f = fopen(DATA_PATH, "r");
		char* temp = (char*) calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
		while (fgets(temp, sizeof(temp), f) != NULL)
		{
			handleDataFromNewString(data, temp);
		}
		free(temp);
	}
	fclose(f);
	return data;
}

void printMainMenu()
{
	printf("��� ������������ ���������, ����������� ������� ����������� ���������������. ��������� ��������:\n");
	printf("1. ������������� ������� ���� � ���������� ������\n");
	printf("2. ������� �����, ��������������� �� ���������� ����\n");
	printf("3. ������� �����, ��������������� �� ���������� ���������������� ����\n");
	printf("4. ���������� ������������\n");
	printf("5. �������� �����\n");
	printf("6. �������� ���� �� ��������� �����\n");
	printf("7. ���������� �������������� ������\n");
	printf("������� ��� ������ �������: ");
}

void suggestReplacementsBasingOnFrequencyAnalysis(CRYPTOGRAM* data)
{
	//TODO
}

void printWordsInOrderByLength(char* text)
{
	//TODO
}

void printWordsInOrderByUndeciphered(char* text)
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
	OPERATION_CODE operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->text); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->text); break;
		case PRINT_BUF: printText(data->text); break;
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
	if (data->text == EMPTY_TEXT) //� ���������� ������ ��� ����
	{
		printf("���������� ������ �� �������� ��� �����������: ����������� �����.");
		_getch();
	}
	else
	{
		handleMainCycle(data);
	}
	free(data);
	return 0;
}