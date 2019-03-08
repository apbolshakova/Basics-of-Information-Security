#include "Header.h"

cryptogram_t* initCryptogram()
{
	cryptogram_t* data = (cryptogram_t*)malloc(sizeof(cryptogram_t));
	data->text = malloc(sizeof(char));
	*(data->text) = '\0';
	data->letter = (letter_t*)calloc(ALPHABET_SIZE, sizeof(letter_t));
	data->numOfLetters = 0;
	data->curChange = initChangesList();

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndGetEncounters(data, f);
	}
	fclose(f);
	if (data->numOfLetters != NO_LETTERS)
	{
		getFrequencies(data);
		data->words = parseTextIntoWords(data->text);
	}
	return data;
}

void printCryptogram(cryptogram_t* data)
{
	system("cls");
	printEncryptionKey(data->letter);
	printText(data);

	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printEncryptionKey(letter_t* letter)
{
	printf("Текущий ключ шифрования:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> ", (letter + i)->symbol);
		if ((letter + i)->replacedTo == NO_REPLACEMENT) printf("замена не определена");
		else printf("%c", (letter + i)->replacedTo);
		printf("\n");
	}
	printf("\n");
}

void printText(cryptogram_t* data)
{
	printf("Текст криптограммы с выполненными заменами:\n\n");
	char* ptr = data->text;
	while (*ptr)
	{
		if (isLetter(*ptr))
		{
			int curSymbolIndex = *(ptr)-'А';
			if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT)
				printf("%c", *ptr);
			else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		}
		else printf("%c", *ptr);
		ptr++;
	}
	printf("\n\n");
}