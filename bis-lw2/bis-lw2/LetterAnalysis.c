#include "Header.h"

void initLetters(letter_t* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->symbol = '�' + i;
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

void getFrequencies(cryptogram_t* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->frequencyInSrcText =
			(float)(data->letter + i)->encounteredInSrcText / data->numOfLetters;
	}
}

void suggestReplacement(cryptogram_t* data)
{
	system("cls");

	printLettersFrequencies(data->letter);
	printReplacementSuggestion(data->letter);

	printf("������� ������, ����� ��������� � ������� ����.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printLettersFrequencies(letter_t* letter)
{
	printf("������� ������ ���� �� ������� ������������:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.4f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(letter_t* letter)
{
	letter_t* srcLetter = findMaxFrqFromUndesiphered(letter);
	if (srcLetter == LETTER_IS_NOT_FOUND)
		printf("���������� ���������� ����������� ������.\n");
	else
	{
		char letterForReplacement = findMaxFrqFromNotReplacement(letter);
		printf("������ �� ���������� ������� ������ �����, ");
		printf("��� ����� %c, ��������, ������� �������� �� %c.\n",
			srcLetter->symbol, letterForReplacement);
	}
}

letter_t* findMaxFrqFromUndesiphered(letter_t* letter)
{
	letter_t* letterWithMaxFrq = LETTER_IS_NOT_FOUND;
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

char findMaxFrqFromNotReplacement(letter_t* letter)
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
