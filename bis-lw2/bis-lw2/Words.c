#include "Header.h"

words_info_t* parseTextIntoWords(char* text)
{
	char* sav = text;
	words_info_t* wordsInfo = (words_info_t*)malloc(sizeof(words_info_t));
	wordsInfo->firstWord = NULL;
	wordsInfo->lastWord = NULL;
	while (*text)
	{
		while (*text && !isLetter(*text)) text++;
		if (isLetter(*text)) text = handleWord(wordsInfo, text);
	}
	text = sav;
	return wordsInfo;
}

char* handleWord(words_info_t* wordsInfo, char* text) //TODO: рефакторинг
{
	char* textSav = text;
	word_list_item_t* newWord = (word_list_item_t*)malloc(sizeof(word_list_item_t));
	newWord->len = 0;
	newWord->numOfUndecipheredLetters = 0;
	while (isLetter(*text))
	{
		newWord->len++;
		text++;
	}
	newWord->chars = (char*)malloc(newWord->len * sizeof(char) + 1);

	text = textSav;
	char* charsSav = newWord->chars;
	while (isLetter(*text))
	{
		*(newWord->chars) = *text;
		text++;
		newWord->chars++;
	}
	*(newWord->chars) = '\0';
	newWord->chars = charsSav;
	if (wordIsUnique(newWord, wordsInfo)) addWordToList(newWord, wordsInfo);
	return text;
}

void addWordToList(word_list_item_t* newWord, words_info_t* wordsInfo)
{
	if (wordsInfo->lastWord == NULL) //список пуст
	{
		wordsInfo->firstWord = newWord;
		wordsInfo->lastWord = wordsInfo->firstWord;
	}
	else
	{
		wordsInfo->lastWord->nextWord = newWord;
		wordsInfo->lastWord = newWord;
	}
	wordsInfo->lastWord->nextWord = NULL;
}

void handleWordsPrintingMenu(cryptogram_t* data)
{
	printing_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Вывести слова, сгрупированные по длине\n"); //TODO: вынести в функцию
		printf("2. Вывести слова, сгрупированные по количеству букв без замен\n");
		printf("3. Вернуться в главное меню\n");
		printf("Введите код нужной команды ");
		printf("(любой код, кроме перечисленных, будет проигнорирован) : ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case BY_LENGTH: 
			data->words->firstWord = sortWordsByLen(data->words->firstWord);
			printWords(BY_LENGTH, data); 
			break; //длины подсчитаны при инициализации
		case BY_UNDECIPHERED: 
			getNumOfUndesiphered(data);
			data->words->firstWord = sortWordsByUndeciphered(data->words->firstWord);
			printWords(BY_UNDECIPHERED, data);
			break;
		default: break;
		}
	} while (operationCode != DECLINE_PRINTING);
}

void getNumOfUndesiphered(cryptogram_t* data)
{
	word_list_item_t* word = data->words->firstWord;
	while (word != NULL)
	{
		word->numOfUndecipheredLetters = 0;
		char* curLetter = word->chars;
		while (*curLetter)
		{
			if ((data->letter + *curLetter - 'А')->replacedTo == NO_REPLACEMENT)
				word->numOfUndecipheredLetters++;
			curLetter++;
		}
		word = word->nextWord;
	}
}

word_list_item_t* sortWordsByLen(word_list_item_t* firstWord)
{
	word_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		word_list_item_t* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL || item->len < newfirstWord->len)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			word_list_item_t* current = newfirstWord;
			while (current->nextWord != NULL && !(item->len < current->nextWord->len))
			{
				current = current->nextWord;
			}
			item->nextWord = current->nextWord;
			current->nextWord = item;
		}
	}
	return newfirstWord;
}

word_list_item_t* sortWordsByUndeciphered(word_list_item_t* firstWord) //TODO: переписать в одну функцию с ByLen
{
	word_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		word_list_item_t* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL ||
			item->numOfUndecipheredLetters < newfirstWord->numOfUndecipheredLetters)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			word_list_item_t* current = newfirstWord;
			while (current->nextWord != NULL &&
				item->numOfUndecipheredLetters >= current->nextWord->numOfUndecipheredLetters)
			{
				current = current->nextWord;
			}
			item->nextWord = current->nextWord;
			current->nextWord = item;
		}
	}
	return newfirstWord;
}

void printWords(printing_operation_code_t order, cryptogram_t* data)
{
	system("cls");

	word_list_item_t* word = data->words->firstWord;
	int prevValue = 0;
	if (order == BY_LENGTH) prevValue = word->len;
	if (order == BY_UNDECIPHERED) prevValue = word->numOfUndecipheredLetters;
	printf("По %i:\n", prevValue);
	printCharsWithEncryption(word->chars, data);

	word = word->nextWord;
	while (word != NULL)
	{
		int nextValue = 0;
		if (order == BY_LENGTH) nextValue = word->len;
		if (order == BY_UNDECIPHERED) nextValue = word->numOfUndecipheredLetters;

		if (nextValue != prevValue) printf("\nПо %i:\n", nextValue);
		printCharsWithEncryption(word->chars, data);

		word = word->nextWord;
		prevValue = nextValue;
	}

	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printCharsWithEncryption(char* ptr, cryptogram_t* data)
{
	while (*ptr)
	{
		int curSymbolIndex = *(ptr)-'А';
		if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *ptr);
		else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		ptr++;
	}
	printf("\n");
}

/*
TODO: использовать эти функции для объединения ф-ии сортировки в одну

void insertByLen(word_list_item_t** newFirstWord, word_list_item_t** itemToInsert)
{
	if (*newFirstWord == NULL ||
		(*itemToInsert)->numOfUndecipheredLetters < (*newFirstWord)->numOfUndecipheredLetters)
	{
		(*itemToInsert)->nextWord = *newFirstWord;
		*newFirstWord = *itemToInsert;
	}
	else
	{
		word_list_item_t** currentToCompareWith = *newFirstWord;
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

void insertByUndeciphered(word_list_item_t** newFirstWord, word_list_item_t* item)
{
	if (*newFirstWord == NULL || item->len < (*newFirstWord)->len)
	{
		item->nextWord = *newFirstWord;
		*newFirstWord = item;
	}
	else
	{
		word_list_item_t* current = *newFirstWord;
		while (current->nextWord != NULL && !(item->len < current->nextWord->len))
		{
			current = current->nextWord;
		}
		item->nextWord = current->nextWord;
		current->nextWord = item;
	}
}

*/