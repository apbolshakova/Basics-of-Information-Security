#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENT 0
#define NO_LETTERS_IN_TEXT 0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "��������������������������������"

#define INVALID_DATA_MESSAGE "���������� ������ �� �������� ��� �����������: ����������� �����.\n"
#define SUCESS_INIT_MESSAGE "�������� ����� ������������ ������� ��������.\n"
#define RETURN_TO_MENU_MESSAGE "������� ������, ����� ��������� � ������� ����.\n"

typedef enum OperationCode
{
	NULL_OPERATION,              //�������� ��� �������������
	PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT = '1', //����� �������������� ������ � ����������� � ��������� ������������� ���� �������� ��������
	PRINT_WORDS_BY_LENGTH,       //����� �� ����� ���� ����, ��������������� �� ���������� ����
	PRINT_WORDS_BY_UNDECIPHERED, //����� �� ����� ���� ����, ��������������� �� ���������� ���������������� �� ������ ������ ����
	PRINT_CRYPTOGRAM,            //����������� ������������ � ��������� ��������������� �� ������ ������ ������
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
	char symbol;
	int encounteredInSrcText;
	float frequencyInSrcText;
	char replacedTo;
} LETTER;

typedef struct ChangesListItem
{
	struct ChangesListItem* prev;
	struct ChangesListItem* next;
	char originalLetter;
	char replacedTo;
} CHANGES_LIST_ITEM;

typedef struct Cryptogram
{
	char* text;
	int numOfLetters;
	LETTER* letter;
	CHANGES_LIST_ITEM* curChange;
} CRYPTOGRAM;

//�������� ������ ���� (����� �������� �������, ���� �����, ������� ������ ���� ������), �������� ���������� �����
typedef struct Word
{
	char* chars;
	int len;
	int numOfUndecipheredLetters;
} WORD;

void initLetters(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->symbol = '�' + i;
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

CHANGES_LIST_ITEM* initChangesList()
{
	CHANGES_LIST_ITEM* initialItem = (CHANGES_LIST_ITEM*)malloc(2 * sizeof(CHANGES_LIST_ITEM*) + 2 * sizeof(char));
	initialItem->prev = NULL;
	initialItem->next = NULL;
	initialItem->originalLetter = NO_REPLACEMENT;
	initialItem->replacedTo = NO_REPLACEMENT;
	return initialItem;
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
			(data->letter + (*(data->text) - '�'))->encounteredInSrcText++;
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

int cmpByFrequencyDesc(const void *a, const void *b)
{
	float aFrq = ((LETTER*)a)->frequencyInSrcText;
	float bFrq = ((LETTER*)b)->frequencyInSrcText;
	if (aFrq > bFrq) return -1;
	if (aFrq < bFrq) return 1;
	return 0;
}

int cmpBySymbolAsc(const void *a, const void *b)
{
	float aSymbol = ((LETTER*)a)->symbol;
	float bSymbol = ((LETTER*)b)->symbol;
	if (aSymbol > bSymbol) return -1; //��������� ����� ������������� ����
	if (aSymbol < bSymbol) return 1;
	return 0;
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char*) + sizeof(LETTER*) + sizeof(int) + sizeof(CHANGES_LIST_ITEM*));
	data->text = malloc(sizeof(char));
	*(data->text) = '\0';
	data->letter = (LETTER*) calloc(ALPHABET_SIZE, sizeof(LETTER));
	data->numOfLetters = 0;
	data->curChange = initChangesList();

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndCalculateEncounters(data, f);
	}
	fclose(f);
	if (data->numOfLetters != NO_LETTERS_IN_TEXT) calculateFrequencies(data);
	return data;
}

void printMainMenu()
{
	printf("��� ������������ ���������, ����������� ������� ����������� ���������������. ��������� ��������:\n");
	printf("1. ���������� ������ �� ������ ���������� �������\n");
	printf("2. ������� �����, ��������������� �� ���������� ����\n");
	printf("3. ������� �����, ��������������� �� ���������� ���������������� ����\n");
	printf("4. ���������� ������������\n");
	printf("5. �������� �����\n");
	printf("6. �������� ���� �� ��������� �����\n");
	printf("7. ���������� �������������� ������\n");
	printf("8. �����\n");
	printf("������� ��� ������ ������� (����� ���, ����� �������������, ����� ��������������): ");
}

void printText(CRYPTOGRAM* data)
{
	char* ptr = data->text;
	while (*(ptr))
	{
		int curSymbolIndex = *(ptr)-'�';
		if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *(ptr));
		else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		ptr++;
	}
	printf("\n\n");
}

void printEncryptionKey(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> ", (letter + i)->symbol);
		if ((letter + i)->replacedTo == NO_REPLACEMENT) printf("������ �� ����������");
		else printf("%c", (letter + i)->replacedTo);
		printf("\n");
	}
	printf("\n");
}

char findLetterWithBiggestFrequencyFromUndesiphered(LETTER* letter)
{
	int index = 0;
	qsort(letter, ALPHABET_SIZE, sizeof(*letter), cmpByFrequencyDesc); //������������� ����� �� ��������
	while (index < ALPHABET_SIZE)
	{
		if ((letter + index)->replacedTo == NO_REPLACEMENT) break;
		else index++;
	}
	char result = (letter + index)->symbol;
	qsort(letter, ALPHABET_SIZE, sizeof(*letter), cmpBySymbolAsc); //������� ���������� �� ��������
	return result;
}

BOOL isUsedAsReplacement(char symbolToCheck, LETTER* letter)
{
	BOOL isUsed = FALSE;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (symbolToCheck == (letter + i)->replacedTo)
		{
			isUsed = TRUE;
			break;
		}
	}
	return isUsed;
}

char findLetterWithBiggestFrequencyFromUnusedAsReplacement(LETTER* letter)
{
	int index = 0;
	while (index < ALPHABET_SIZE)
	{
		if (isUsedAsReplacement(LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index], letter)) index++;
		else break;
	}
	return LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index];
}

void printLettersFrequencies(LETTER* letter)
{
	printf("������� ������ ���� �� ������� ������������:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.2f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(LETTER* letter)
{
	char srcLetter = findLetterWithBiggestFrequencyFromUndesiphered(letter);
	char letterForReplacement = findLetterWithBiggestFrequencyFromUnusedAsReplacement(letter);
	printf("������ �� ���������� ������� ������ �����, ��� ����� %c, ��������, ������� �������� �� %c.\n", srcLetter, letterForReplacement);
	//TODO: �������� ������ � �������������� �������
}

void analyseFrequencyAndSuggestReplacement(CRYPTOGRAM* data)
{
	system("cls");

	printLettersFrequencies(data->letter);
	printReplacementSuggestion(data->letter);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printWordsInOrderByLength(char* text)
{
	//WORD* word = parseTextIntoWords(text); //��������� �� ������ �����
	//qsort(word, ����� ����, sizeof(WORD), cmpByFrequencyDesc);
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

void printCryptogram(CRYPTOGRAM* data)
{
	system("cls");
	printf("������� ���� ����������:\n");
	printEncryptionKey(data->letter);

	printf("����� ������������ � ������������ ��������:\n\n");
	printText(data);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	//TODO
	//������� ������� ���� ���������� � ����� ������������
	//��������� ������ ����� ��� ������
	//�� ����� �����
	//������
	//�������� ������ - ��� ���� ��� ������� ����
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	//TODO
	//������� ������� ���� ���������� � ����� ������������
	//��� ����� - ��������
	//��������� ������ - ���������� �����
	//�� ��������� ������ - ���������� ����� ��� �������
	//��� ������ �������� ������ curChange �� ��-� ����� ��� �����
	//��� ��� ��� ������� ����
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
		case PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT: analyseFrequencyAndSuggestReplacement(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->text); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->text); break;
		case PRINT_CRYPTOGRAM: printCryptogram(data); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		//TODO: �������, ��������� ������� ���� ��������� ������
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
	//TODO: �������� �������, ����������� ������
	free(data);
	return 0;
}