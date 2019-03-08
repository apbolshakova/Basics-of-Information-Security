#include "Header.h"

bool_t isCapitalLetter(char item)
{
	if ('А' <= item && item <= 'Я') return TRUE;
	else return FALSE;
}

bool_t isLowercaseLetter(char item)
{
	if ('а' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

bool_t isLetter(char item)
{
	if ('А' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

bool_t isUsedAsReplacement(char symbolToCheck, letter_t* letter)
{
	bool_t isUsed = FALSE;
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

bool_t wordIsUnique(words_list_item_t* newWord, words_list_item_t* head)
{
	words_list_item_t* item = head;
	bool_t isUnique = TRUE;
	while (item != NULL && isUnique)
	{
		if (item->len == newWord->len &&
			areSameWords(item->chars, newWord->chars)) isUnique = FALSE;
		item = item->next;
	}
	return isUnique;
}

bool_t areSameWords(char* a, char* b)
{
	bool_t areSame = TRUE;
	while (*a && *b)
	{
		if (*a != *b)
		{
			areSame = FALSE;
			break;
		}
		++a;
		++b;
	}
	if (*a || *b) areSame = FALSE; //должны были закончиться одновременно
	return areSame;
}