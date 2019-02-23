#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENTS 0
#define NO_LETTERS_IN_TEXT 0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "��������������������������������"

#define INVALID_DATA_MESSAGE "���������� ������ �� �������� ��� �����������: ����������� �����.\n"
#define SUCESS_INIT_MESSAGE "�������� ����� ������������ ������� ��������.\n"
#define RETURN_TO_MENU_MESSAGE "������� ������, ����� ��������� � ������� ����.\n"

typedef enum 
{
	NULL_OPERATION,              //�������� ��� �������������
	ANALYZE = '1',               //������ ������� ���� �� ������� ����� � ����� �������������� ����� � ����������� � ��������� ������������� ���� �������� ��������
	PRINT_WORDS_BY_LENGTH,       //����� �� ����� ���� ����, ��������������� �� ���������� ����
	PRINT_WORDS_BY_UNDECIPHERED, //����� �� ����� ���� ����, ��������������� �� ���������� ���������������� �� ������ ������ ����
	PRINT_TEXT,                  //����������� ������������ � ��������� ��������������� �� ������ ������ ������
	REPLACE_LETTERS,             //����������� ������ ���� � ������������
	REVERT,                      //�������� � ����� ������� ������ ���� � ������������
	AUTOREPLACEMENT,             //�������������� ������ ����
	EXIT 
} OPERATION_CODE;

typedef enum Bool
{ 
	FALSE, 
	TRUE 
} BOOL;

typedef struct Letter
{
	int encounteredInSrcText;
	float frequencyInSrcText;
	char replacedTo; //TODO: ������� ��� ������� �� ��������� �������� � ������� �� ����� ������ �� ����� ����� ������
} LETTER;

typedef struct Cryptogram
{
	char* text;
	int numOfLetters;
	LETTER* letter;
} CRYPTOGRAM;

void initLetters(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENTS;
	}
}

BOOL isLetter(char item)
{
	if ('�' <= item && item <= '�') return TRUE;
	else return FALSE;
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str) //TODO: �����������
{
	int sizeOfOldText = 0; 
	char* sav = data->text;
	while (*(data->text)) //�������� �����, ��� ������ ���������
	{
		sizeOfOldText++;
		data->text++;
	}

	data->text = sav;
	data->text = (char*) realloc(data->text, (sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char)); //��������� ������
	
	sav = data->text;
	data->text += sizeOfOldText;
	while (*str) //���������� �������
	{
		*(data->text) = *str;
		if (isLetter(*(data->text)))
		{
			data->numOfLetters++;
			(data->letter + (*(data->text) + 64))->encounteredInSrcText++;
		}
		(data->text)++;
		str++;
	}
	*(data->text) = '\0';
	data->text = sav;
}

void initTextAndCalculateEncounters(CRYPTOGRAM* data, FILE* f)
{
	fclose(f);
	f = fopen(DATA_PATH, "r");
	char* temp = (char*) calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
	while (fgets(temp, SIZE_OF_STRING_TO_COPY * sizeof(char), f) != NULL) handleDataFromNewString(data, temp);
	free(temp);
}

void calculateFrequencies(CRYPTOGRAM* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->frequencyInSrcText = (float) (data->letter + i)->encounteredInSrcText / data->numOfLetters;
	}
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char*) + sizeof(LETTER*) + sizeof(int));
	data->text = malloc(sizeof(char));
	data->letter = (LETTER*)calloc(ALPHABET_SIZE, sizeof(LETTER));
	*(data->text) = '\0';
	data->numOfLetters = 0;

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndCalculateEncounters(data, f);
	}
	fclose(f);

	calculateFrequencies(data);
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
	printf("8. �����\n");
	printf("������� ��� ������ ������� (����� ���, ����� �������������, ����� ��������������): ");
}

void suggestReplacementsBasingOnFrequencyAnalysis(CRYPTOGRAM* data)
{
	//TODO
	//����� ���������� �������, ��� ������� �� ���� ������� ������ � ���������� �������� ����� ���� ������� �� 1-�� ���������������� � ������� ����� �� LETTERS_IN_ORDER_BY_FREQUENCY_DESC
}

void printWordsInOrderByLength(char* text)
{
	//TODO
	//�������� ������ ���� (����� �������� �������, ���� �����, ������� ������ ���� ������), �������� ���������� �����
	//������� �� ���������� ����� � ����������
}

void printWordsInOrderByUndeciphered(char* text)
{
	//TODO
	//�������� ������ ���� (����� �������� �������, ���� �����, ������� ������ ���� ������), �������� ���������� ���������� ���� ������
	//������� �� ���������� ����� � ����������
}

void printText(char* text)
{
	system("cls");
	printf("����� ������������ � ������������ ��������:\n\n");
    
	char* ptr = text;
	while (*(ptr)) printf("%c", *(ptr++));
	printf("\n\n");

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	//TODO
	//������� ������ ����������� ����
	//��������� ������ ����� ��� ������
	//�� ����� �����
	//�������� ������ - ��� ���� ��� ������� ����
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	//TODO
	//������� ������ ��� �������� ����� - � ��������� ������
	//������� �������� �����
	//�������� ��� �� ������� ��������� �� replacedTo
	//��������� ������ ��� ����� � ���� �����
	//��� ��� �� ��� ��� ������� ����
}

void replaceLettersAutomatically(CRYPTOGRAM* data)
{
	//TODO
	//����������� ����� letters �� ������� (�� ��������) � ������
	//�� 0 �� 31: ���������� ������� - ������� �� i-�� �� �������������� ������ ��� �������� ��������
	//            �� ���������� ������� - ���������� ����������
}

void handleMainCycle(CRYPTOGRAM* data)
{
	OPERATION_CODE operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->text); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->text); break;
		case PRINT_TEXT: printText(data->text); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		default: break;
		}
	} while (operationCode != EXIT);
}

int main(void)
{
	setlocale(LC_ALL, "rus");

	CRYPTOGRAM* data = initCryptogram();
	if (data->numOfLetters == NO_LETTERS_IN_TEXT)
	{
		printf(INVALID_DATA_MESSAGE);
		_getch();
	}
	else handleMainCycle(data);
	free(data);
	return 0;
}

/*�������:
����� �� ������� ������ ����� ������� �����
�� ������ ���� �������������� ������
"�������������� �� ������ ������ �����" ��� ��������
������� ������� ������� � �� ��� ������ ���� ������
���������� �� ������ ����� ������������� �����
*/