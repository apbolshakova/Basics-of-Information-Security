#include "Header.h"

void parseTextIntoWords(cryptogram_t* data)
{
	char* sav = data->text;
	while (*data->text && !isLetter(*data->text)) data->text++;
	if (isLetter(*data->text)) addWordToList(data);
	while (*data->text)
	{
		while (*data->text && !isLetter(*data->text)) data->text++;
		if (isLetter(*data->text))
		{
			addWordToList(data);
		}
	}
	data->text = sav;
}

void addWordToList(cryptogram_t* data)
{
	words_list_item_t* newWord = (words_list_item_t*)malloc(sizeof(words_list_item_t));
	handleWordData(newWord, data);
	if (wordIsUnique(newWord, data->wordListHead))
	{
		newWord->next = data->wordListHead;
		data->wordListHead = newWord;
	}
}

void handleWordData(words_list_item_t* newWord, cryptogram_t* data)
{
	char* textSav = data->text;
	newWord->len = 0;
	newWord->numOfUndecipheredLetters = 0;
	while (isLetter(*data->text))
	{
		newWord->len++;
		data->text++;
	}
	newWord->chars = (char*)malloc(newWord->len * sizeof(char) + 1);
	data->text = textSav;
	char* charsSav = newWord->chars;
	while (isLetter(*data->text))
	{
		*(newWord->chars) = *data->text;
		data->text++;
		newWord->chars++;
	}
	*(newWord->chars) = '\0';
	newWord->chars = charsSav;
}

void handleWordsPrintingMenu(cryptogram_t* data)
{
	printing_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. ������� �����, �������������� �� �����\n"); //TODO: ������� � �������
		printf("2. ������� �����, �������������� �� ���������� ���� ��� �����\n");
		printf("3. ��������� � ������� ����\n");
		printf("������� ��� ������ ������� ");
		printf("(����� ���, ����� �������������, ����� ��������������) : ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case BY_LENGTH: 
			data->wordListHead = sortWordsByLen(data->wordListHead);
			printWords(BY_LENGTH, data); 
			break; //����� ���������� ��� �������������
		case BY_UNDECIPHERED: 
			getNumOfUndesiphered(data);
			data->wordListHead = sortWordsByUndeciphered(data->wordListHead);
			printWords(BY_UNDECIPHERED, data);
			break;
		default: break;
		}
	} while (operationCode != DECLINE_PRINTING);
}

void getNumOfUndesiphered(cryptogram_t* data)
{
	words_list_item_t* word = data->wordListHead;
	while (word != NULL)
	{
		word->numOfUndecipheredLetters = 0;
		char* curLetter = word->chars;
		while (*curLetter)
		{
			if ((data->letter + *curLetter - '�')->replacedTo == NO_REPLACEMENT)
				word->numOfUndecipheredLetters++;
			curLetter++;
		}
		word = word->next;
	}
}

words_list_item_t* sortWordsByLen(words_list_item_t* firstWord)
{
	words_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		words_list_item_t* item = firstWord;
		firstWord = firstWord->next;

		if (newfirstWord == NULL || item->len < newfirstWord->len)
		{
			item->next = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			words_list_item_t* current = newfirstWord;
			while (current->next != NULL && !(item->len < current->next->len))
			{
				current = current->next;
			}
			item->next = current->next;
			current->next = item;
		}
	}
	return newfirstWord;
}

words_list_item_t* sortWordsByUndeciphered(words_list_item_t* firstWord) //TODO: ���������� � ���� ������� � ByLen
{
	words_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		words_list_item_t* item = firstWord;
		firstWord = firstWord->next;

		if (newfirstWord == NULL ||
			item->numOfUndecipheredLetters < newfirstWord->numOfUndecipheredLetters)
		{
			item->next = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			words_list_item_t* current = newfirstWord;
			while (current->next != NULL &&
				item->numOfUndecipheredLetters >= current->next->numOfUndecipheredLetters)
			{
				current = current->next;
			}
			item->next = current->next;
			current->next = item;
		}
	}
	return newfirstWord;
}

void printWords(printing_operation_code_t order, cryptogram_t* data)
{
	system("cls");

	words_list_item_t* word = data->wordListHead;
	int prevValue = 0;
	if (order == BY_LENGTH) prevValue = word->len;
	if (order == BY_UNDECIPHERED) prevValue = word->numOfUndecipheredLetters;
	printf("�� %i:\n", prevValue);
	printCharsWithEncryption(word->chars, data);

	word = word->next;
	while (word != NULL)
	{
		int nextValue = 0;
		if (order == BY_LENGTH) nextValue = word->len;
		if (order == BY_UNDECIPHERED) nextValue = word->numOfUndecipheredLetters;

		if (nextValue != prevValue) printf("\n�� %i:\n", nextValue);
		printCharsWithEncryption(word->chars, data);

		word = word->next;
		prevValue = nextValue;
	}

	printf("������� ������, ����� ��������� � ������� ����.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printCharsWithEncryption(char* ptr, cryptogram_t* data)
{
	while (*ptr)
	{
		int curSymbolIndex = *(ptr)-'�';
		if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *ptr);
		else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		ptr++;
	}
	printf("\n");
}

/*
TODO: ������������ ��� ������� ��� ����������� �-�� ���������� � ����

void insertByLen(words_list_item_t** newFirstWord, words_list_item_t** itemToInsert)
{
	if (*newFirstWord == NULL ||
		(*itemToInsert)->numOfUndecipheredLetters < (*newFirstWord)->numOfUndecipheredLetters)
	{
		(*itemToInsert)->nextWord = *newFirstWord;
		*newFirstWord = *itemToInsert;
	}
	else
	{
		words_list_item_t** currentToCompareWith = *newFirstWord;
		while (currentToCompareWith != NULL &&
				  !((*itemToInsert)->numOfUndecipheredLetters <
				  (*currentToCompareWith)->numOfUndecipheredLetters))
		{
			*currentToCompareWith = (*currentToCompareWith)->nextWord;
		}
		(*itemToInsert)->nextWord = (*currentToCompareWith)->nextWord;
		(*currentToCompareWith)->nextWord = *itemToInsert;
	}
}

void insertByUndeciphered(words_list_item_t** newFirstWord, words_list_item_t* item)
{
	if (*newFirstWord == NULL || item->len < (*newFirstWord)->len)
	{
		item->nextWord = *newFirstWord;
		*newFirstWord = item;
	}
	else
	{
		words_list_item_t* current = *newFirstWord;
		while (current->nextWord != NULL && !(item->len < current->nextWord->len))
		{
			current = current->nextWord;
		}
		item->nextWord = current->nextWord;
		current->nextWord = item;
	}
}

*/