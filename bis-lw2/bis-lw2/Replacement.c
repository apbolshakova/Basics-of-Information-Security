#include "Header.h"

void handleReplacementMenu(cryptogram_t* data)
{
	system("cls");
	printEncryptionKey(data->letter);

	printf("Введите букву, замену которой необходимо провести или пробел для выхода в главное меню: ");
	char srcLetter = getCyrrilicLetterOrExitSymbol();
	if (srcLetter == RETURN_TO_MENU_BTN_CODE) return;
	if (isLowercaseLetter(srcLetter)) srcLetter -= ALPHABET_SIZE; //сдвиг на др. регистр

	printf("Введите букву, на которую её необходимо заменить или пробел для выхода в главное меню: ");
	char letterForReplacement = getCyrrilicLetterOrExitSymbol();
	if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
	if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	while (isUsedAsReplacement(letterForReplacement, data->letter))
	{
		printf("Эта буква уже использована в качестве замены. ");
		printf("Введите другую букву русского алфавита или введите пробел для выхода в главное меню: ");
		letterForReplacement = getCyrrilicLetterOrExitSymbol();
		if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
		if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	}
	replaceLetter(srcLetter, letterForReplacement, data);
	addElementToHistory(srcLetter, letterForReplacement, data);

	system("cls");
	printf("Замена проведена успешно.\n");
	printEncryptionKey(data->letter);
	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

char getCyrrilicLetterOrExitSymbol()
{
	char letter = '\n'; //для исправления проблемы со считанным ENTER во входном потоке
	while (letter == '\n') scanf("%c", &letter);
	letter = fixCodeForCyrillicChar(letter);
	while (letter != RETURN_TO_MENU_BTN_CODE && !isLetter(letter))
	{
		printf("Недопустимый символ. Введите букву русского алфавита или пробел для выхода в главное меню: ");
		scanf("%c", &letter);
		while (letter == '\n') scanf("%c", &letter);
		letter = fixCodeForCyrillicChar(letter);
	}
	return letter;
}

//исправление считанных кодов символов, сдвиги найдены экспериментально
char fixCodeForCyrillicChar(char item)
{
	if (-128 <= item && item <= -81) return item + 64;
	if (-32 <= item && item <= -17) return item + 16;
	return item;
}

void replaceLetter(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	(data->letter + srcLetter - 'А')->replacedTo = letterForReplacement;
}

void handleAutoreplacement(cryptogram_t* data)
{
	system("cls");
	replaceAndUpdateHistoryAuto(data);
	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void replaceAndUpdateHistoryAuto(cryptogram_t* data)
{
	letter_t* srcLetter = findMaxFrqFromUndesiphered(data->letter);
	if (srcLetter == LETTER_IS_NOT_FOUND)
	{
		printf("Невозможно определить оптимальную замену.\n");
		return;
	}
	while (srcLetter != LETTER_IS_NOT_FOUND)
	{
		char letterForReplacement = findMaxFrqFromNotReplacement(data->letter);
		replaceLetter(srcLetter->symbol, letterForReplacement, data);
		addElementToHistory(srcLetter->symbol, letterForReplacement, data);

		letter_t* nextSrcLetter = findMaxFrqFromUndesiphered(data->letter);
		if (nextSrcLetter == LETTER_IS_NOT_FOUND ||
			srcLetter->frequencyInSrcText != nextSrcLetter->frequencyInSrcText)
		{
			printf("Символ %c был заменён на %c.\n", srcLetter->symbol, letterForReplacement);
			srcLetter = nextSrcLetter;
		}
		else
		{
			deleteLastChange(data);
			printf("Невозможно определить следующую оптимальную замену.\n");
			break;
		}
	}
}