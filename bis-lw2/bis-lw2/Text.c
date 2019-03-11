#include "Header.h"

void initTextAndGetEncounters(cryptogram_t* data, FILE* f)
{
	fclose(f);
	f = fopen(DATA_PATH, "r");
	if (f == NULL) return;
	long int fileSize = getFileSize(f);
	char* temp = (char*)calloc(fileSize, sizeof(char));
	if (temp == NULL) return NULL;
	data->text = (char*)calloc(fileSize, sizeof(char));
	if (data->text == NULL)
	{
		temp = NULL;
		free(temp);
		return NULL;
	}
	while (fgets(temp, fileSize, f) != NULL) handleDataFromString(data, temp);
	free(temp);
}

long int getFileSize(FILE *f)
{
	long int sav = ftell(f);
	fseek(f, 0L, SEEK_END);
	long int fileSize = ftell(f);
	fseek(f, sav, SEEK_SET);
	return(fileSize);
}

void handleDataFromString(cryptogram_t* data, char* str)
{
	char* sav = data->text;
	while (*(data->text)) data->text++;
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