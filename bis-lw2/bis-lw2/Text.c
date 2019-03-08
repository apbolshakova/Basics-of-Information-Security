#include "Header.h"

void initTextAndGetEncounters(cryptogram_t* data, FILE* f)
{
	fclose(f);
	f = fopen(DATA_PATH, "r");
	char* temp = (char*)calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
	while (fgets(temp, SIZE_OF_STRING_TO_COPY * sizeof(char), f) != NULL)
		handleDataFromString(data, temp);
	free(temp);
}

void handleDataFromString(cryptogram_t* data, char* str)
{
	char* sav = data->text;
	int sizeOfOldText = getSizeOfText(data->text);

	data->text = sav;
	data->text = (char*)realloc(data->text,
		(sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char) + 1);
	sav = data->text;

	data->text += sizeOfOldText;
	copyStringAndGetEncounters(data, str);
	data->text = sav;
}

int getSizeOfText(char* text)
{
	int sizeOfText = 0;
	while (*(text))
	{
		sizeOfText++;
		text++;
	}
	return sizeOfText;
}

void copyStringAndGetEncounters(cryptogram_t* data, char* str)
{
	while (*str)
	{
		*(data->text) = *str;
		if (isCapitalLetter(*(data->text)))
		{
			data->numOfLetters++;
			(data->letter + (*(data->text) - 'À'))->encounteredInSrcText++;
		}
		(data->text)++;
		str++;
	}
	*(data->text) = '\0';
}