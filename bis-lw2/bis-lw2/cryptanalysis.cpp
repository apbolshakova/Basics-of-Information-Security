#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <ctype.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENT 0
#define NO_LETTERS_IN_TEXT 0
#define NO_LETTERS_IN_WORD 0
#define LETTER_IS_NOT_FOUND 0
#define INITIAL_FRQ 0.0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "��������������������������������"

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

typedef struct wordListItem
{
	char* chars;
	int len;
	int numOfUndecipheredLetters;
	struct wordListItem* prevWord;
} WORD_LIST_ITEM;

typedef struct wordsInfo
{
	WORD_LIST_ITEM* lastWord;
	int maxWordLen;
} WORDS_INFO;

typedef struct Cryptogram
{
	char* text;
	int numOfLetters;
	LETTER* letter;
	CHANGES_LIST_ITEM* curChange;
	WORDS_INFO* words;
} CRYPTOGRAM;

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

BOOL isCapitalLetter(char item)
{
	if ('�' <= item && item <= '�') return TRUE;
	else return FALSE;
}

BOOL isLowercaseLetter(char item)
{
	if ('�' <= item && item <= '�') return TRUE;
	else return FALSE;
}

BOOL isLetter(char item)
{
	if ('�' <= item && item <= '�') return TRUE;
	else return FALSE;
}

char fixCodeForCyrillicCharFromInput(char item) //����������� ��������� ����� ��������, ������ ������� ����������������
{
	if (-128 <= item && item <= -81) return item + 64;
	if (-32 <= item && item <= -17) return item + 16;
	return item;
}

int moveTextPtrToTheEndAndGetSizeOfText(char* text)
{
	int sizeOfText = 0;
	while (*(text))
	{
		sizeOfText++;
		text++;
	}
	return sizeOfText;
}

void copyStringAndCalculateLettersEncounters(CRYPTOGRAM* data, char* str)
{
	while (*str)
	{
		*(data->text) = *str;
		if (isCapitalLetter(*(data->text)))
		{
			data->numOfLetters++;
			(data->letter + (*(data->text) - '�'))->encounteredInSrcText++;
		}
		(data->text)++;
		str++;
	}
	*(data->text) = '\0';
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str)
{
	char* sav = data->text;
	int sizeOfOldText = moveTextPtrToTheEndAndGetSizeOfText(data->text);

	data->text = sav;
	data->text = (char*) realloc(data->text, (sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char));
	sav = data->text;

	data->text += sizeOfOldText;
	copyStringAndCalculateLettersEncounters(data, str);
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
	if (aSymbol > bSymbol) return 1;
	if (aSymbol < bSymbol) return -1;
	return 0;
}

BOOL areSameWords(char* a, char* b)
{
	BOOL areSame = TRUE;
	while (*a && *b)
	{
		if (*a != *b)
		{
			areSame = FALSE;
			break;
		}
		a++;
		b++;
	}
	if (*a || *b) areSame = FALSE; //������ ���� ����������� ������������
	return areSame;
}

BOOL wordIsUnique(WORD_LIST_ITEM* newWord, WORDS_INFO* wordsInfo)
{
	WORD_LIST_ITEM* item = wordsInfo->lastWord;
	BOOL isUnique = TRUE;
	while (item != NULL && isUnique)
	{
		if (item->len == newWord->len && 
			areSameWords(item->chars, newWord->chars)) isUnique = FALSE;
		item = item->prevWord;
	}
	return isUnique;
}

void addNewWordToList(WORD_LIST_ITEM* newWord, WORD_LIST_ITEM* lastWord)
{
	WORD_LIST_ITEM* oldLastWord = lastWord;
	lastWord = newWord;
	lastWord->prevWord = oldLastWord;
}

void handleWord(WORDS_INFO* wordsInfo, char* text) //TODO: �����������
{
	char* sav = text;
	WORD_LIST_ITEM* newWord = (WORD_LIST_ITEM*)malloc(sizeof(WORD_LIST_ITEM));
	newWord->len = 0;
	newWord->numOfUndecipheredLetters = 0; 
	while (isLetter(*text))
	{
		newWord->len++;
		text++;
	}
	newWord->chars = (char*)malloc(newWord->len * sizeof(char) + 1);
	
	text = sav;
	while (isLetter(*text))
	{
		newWord->chars = *text;
		text++;
		newWord->chars++;
	}
	newWord->chars = '\0';
	if (wordIsUnique(newWord, wordsInfo))
	{
		if (wordsInfo->maxWordLen == 0 || newWord->len > wordsInfo->lastWord->len) 
		    wordsInfo->maxWordLen = newWord->len;
		addNewWordToList(newWord, wordsInfo->lastWord); //TODO: �������� ��� �������
	} 
}

WORDS_INFO* parseTextIntoWords(char* text) //TODO ��������
{
	char* sav = text;
	WORDS_INFO* wordsInfo = (WORDS_INFO*)malloc(sizeof(WORDS_INFO));
	wordsInfo->lastWord = (WORD_LIST_ITEM*)malloc(sizeof(WORD_LIST_ITEM));
	wordsInfo->lastWord->chars = NO_LETTERS_IN_WORD;
	wordsInfo->lastWord->prevWord = NULL;
	wordsInfo->maxWordLen = 0;
	while (*text)
	{
		while (!isLetter(*text)) text++;
		if (isLetter(*text)) handleWord(wordsInfo, text);
	}
	text = sav;
	return wordsInfo;
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = (CRYPTOGRAM*) malloc(sizeof(CRYPTOGRAM));
	data->text = malloc(sizeof(char));
	*(data->text) = '\0';
	data->letter = (LETTER*) calloc(ALPHABET_SIZE, sizeof(LETTER));
	data->numOfLetters = 0;
	data->curChange = initChangesList();
	data->words = parseTextIntoWords(data->text);

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
	printf("6. �������� ��� ������� ��������� ���������\n");
	printf("7. ���������� �������������� ������\n");
	printf("8. �����\n");
	printf("������� ��� ������ ������� (����� ���, ����� �������������, ����� ��������������): ");
}

void printText(CRYPTOGRAM* data)
{
	printf("����� ������������ � ������������ ��������:\n\n");
	char* ptr = data->text;
	while (*ptr)
	{
		if (isLetter(*ptr))
		{
			int curSymbolIndex = *(ptr)-'�';
			if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *ptr);
			else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		}
		else printf("%c", *ptr);
		ptr++;
	}
	printf("\n\n");
}

void printEncryptionKey(LETTER* letter)
{
	printf("������� ���� ����������:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> ", (letter + i)->symbol);
		if ((letter + i)->replacedTo == NO_REPLACEMENT) printf("������ �� ����������");
		else printf("%c", (letter + i)->replacedTo);
		printf("\n");
	}
	printf("\n");
}

LETTER* findLetterWithMaxFrequencyFromUndesiphered(LETTER* letter)
{
	LETTER* letterWithMaxFrq = LETTER_IS_NOT_FOUND;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if ((letter + i)->replacedTo == NO_REPLACEMENT
			&& (letterWithMaxFrq == LETTER_IS_NOT_FOUND ||
				letterWithMaxFrq->frequencyInSrcText < (letter + i)->frequencyInSrcText))
		{
			letterWithMaxFrq = letter + i;
		}
	}
	return letterWithMaxFrq;
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

char findLetterWithMaxFrequencyFromUnusedAsReplacement(LETTER* letter)
{
	int index = 0;
	while (index < ALPHABET_SIZE)
	{
		if (isUsedAsReplacement(LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index], letter)) index++;
		else break;
	}
	if (index == ALPHABET_SIZE) return LETTER_IS_NOT_FOUND;
    else return LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index];
}

void printLettersFrequencies(LETTER* letter)
{
	printf("������� ������ ���� �� ������� ������������:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.4f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(LETTER* letter)
{
	LETTER* srcLetter = findLetterWithMaxFrequencyFromUndesiphered(letter);
	//TODO: �������� ��������, �� �������� �� ���� � srcLetter
	char letterForReplacement = findLetterWithMaxFrequencyFromUnusedAsReplacement(letter);
	printf("������ �� ���������� ������� ������ �����, ��� ����� %c, ��������, ������� �������� �� %c.\n", srcLetter->symbol, letterForReplacement);
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
	//TODO
	//qsort(word, numberOfWords, sizeof(WORD*), cmpByLenAsc);
	//������� ��� �����
}

void printWordsInOrderByUndeciphered(char* text)
{
	//TODO
	//������������ ��� ���� undeciphered �������
	//qsort(word, numberOfWords, sizeof(WORD*), cmpByUndecipheredAsc);
	//������� ��� �����
}

void printCryptogram(CRYPTOGRAM* data)
{
	system("cls");
	printEncryptionKey(data->letter);
	printText(data);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

char getCyrrilicLetter()
{
	char letter = '\n'; //��� ����������� �������� � ������� �� ������� ������
	while (letter == '\n') scanf("%c", &letter);
	letter = fixCodeForCyrillicCharFromInput(letter);
	while (!isLetter(letter))
	{
		printf("������������ ������. ������� ����� �������� ��������: ");
		scanf("%c", &letter);
		while (letter == '\n') scanf("%c", &letter);
		letter = fixCodeForCyrillicCharFromInput(letter);
	}
	return letter;
}

CHANGES_LIST_ITEM* addNewElementToHistory(char srcLetter, char letterForReplacement, CRYPTOGRAM* data)
{
	CHANGES_LIST_ITEM* newItem = (CHANGES_LIST_ITEM*)malloc(2 * sizeof(CHANGES_LIST_ITEM*) + 2 * sizeof(char));
	newItem->prev = data->curChange;
	newItem->next = NULL;
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;
	data->curChange = newItem;
}

void replaceLetter(char srcLetter, char letterForReplacement, CRYPTOGRAM* data)
{
	(data->letter + srcLetter - '�')->replacedTo = letterForReplacement;
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	system("cls");
	printEncryptionKey(data->letter);

	printf("������� �����, ������ ������� ���������� ��������: ");
	char srcLetter = getCyrrilicLetter();
	if (isLowercaseLetter(srcLetter)) srcLetter -= ALPHABET_SIZE; //����� � ��������� ���� �� �������
	
	printf("������� �����, �� ������� � ���������� ��������: ");
	char letterForReplacement = getCyrrilicLetter();
	if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	while (isUsedAsReplacement(letterForReplacement, data->letter))
	{
		printf("��� ����� ��� ������������ � �������� ������. ������� ������ ����� �������� ��������: ");
		letterForReplacement = getCyrrilicLetter();
		if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	} 
	replaceLetter(srcLetter, letterForReplacement, data);
	addNewElementToHistory(srcLetter, letterForReplacement, data);

	system("cls");
	printf("������ ��������� �������.\n");
	printEncryptionKey(data->letter);
	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
	//TODO: ��������� ����� ��� ������ � ������� ����
}

void undoCurChange(CRYPTOGRAM* data)
{
	(data->letter + data->curChange->originalLetter - '�')->replacedTo = NO_REPLACEMENT;
	CHANGES_LIST_ITEM* oldCurChange = data->curChange;
	data->curChange = data->curChange->prev;
	data->curChange->next = oldCurChange;
}

void redoCurChange(CRYPTOGRAM* data)
{
	(data->letter + data->curChange->originalLetter - '�')->replacedTo = data->curChange->next->replacedTo;
	CHANGES_LIST_ITEM* oldCurChange = data->curChange;
	data->curChange = data->curChange->next;
	data->curChange->prev = oldCurChange;
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	int operationCode = 0; //TODO: ������� enum ��� ����� �������� � ��������
	do
	{
		system("cls");

		printf("1. ����� � ������� ����\n");
		if (data->curChange->prev == NULL && data->curChange->next == NULL)
		{
			printf("������� ����� �����.\n");
		}
		else
		{
			if (data->curChange->prev != NULL) printf("2. �������� ��������� ��������: ������ %c �� %c\n",
				data->curChange->originalLetter,
				data->curChange->replacedTo);
			if (data->curChange->next != NULL) printf("3. ������������ ��������� ��������� ��������: ������ %c �� %c\n",
				data->curChange->next->originalLetter,
				data->curChange->next->replacedTo);
		}

		printf("������� ��� ������ ������� (����� ���, ����� �������������, ����� ��������������): ");
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case '2': if (data->curChange->prev != NULL) undoCurChange(data); break;
		case '3': if (data->curChange->next != NULL) redoCurChange(data); break;
		default: break;
		}
	} while (operationCode != '1');
}

void replaceLettersAndUpdateHistoryAutomatically(CRYPTOGRAM* data)
{
	float prevUndesipheredLetterFrq = INITIAL_FRQ;
	float curUndesipheredLetterFrq = INITIAL_FRQ;
	LETTER* srcLetter = LETTER_IS_NOT_FOUND;
	do
	{
		prevUndesipheredLetterFrq = curUndesipheredLetterFrq;
		srcLetter = findLetterWithMaxFrequencyFromUndesiphered(data->letter);
		if (srcLetter != LETTER_IS_NOT_FOUND)
		{
			curUndesipheredLetterFrq = srcLetter->frequencyInSrcText;
			if (curUndesipheredLetterFrq != prevUndesipheredLetterFrq)
			{
				char letterForReplacement = findLetterWithMaxFrequencyFromUnusedAsReplacement(data->letter); //����� �������� ������ �� �����, �.�. ���-�� �������� � ����� ���������� ���������� ���-��
				replaceLetter(srcLetter->symbol, letterForReplacement, data);
				addNewElementToHistory(srcLetter->symbol, letterForReplacement, data);
				printf("������ %c ��� ������ �� %c.\n", srcLetter->symbol, letterForReplacement);
			}
			else break;
		}
		else break;
	} while (srcLetter != LETTER_IS_NOT_FOUND);
}

void handleAutoreplacement(CRYPTOGRAM* data)
{
	system("cls");
	replaceLettersAndUpdateHistoryAutomatically(data);
	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
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
		case AUTOREPLACEMENT: handleAutoreplacement(data); break;
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
		printf("���������� ������ �� �������� ��� �����������: ����������� �����.\n");
		_getch();
	}
	else handleMainCycle(data);
	//TODO: �������� �������, ����������� ������
	free(data);
	return 0;
}