#include "Header.h"

void handleReplacementMenu(cryptogram_t* data)
{
	system("cls");
	printEncryptionKey(data->letter);

	printf("������� �����, ������ ������� ���������� �������� ��� ������ ��� ������ � ������� ����: ");
	char srcLetter = getCyrrilicLetterOrExitSymbol();
	if (srcLetter == RETURN_TO_MENU_BTN_CODE) return;
	if (isLowercaseLetter(srcLetter)) srcLetter -= ALPHABET_SIZE; //����� �� ��. �������

	printf("������� �����, �� ������� � ���������� �������� ��� ������ ��� ������ � ������� ����: ");
	char letterForReplacement = getCyrrilicLetterOrExitSymbol();
	if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
	if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	while (isUsedAsReplacement(letterForReplacement, data->letter))
	{
		printf("��� ����� ��� ������������ � �������� ������. ");
		printf("������� ������ ����� �������� �������� ��� ������� ������ ��� ������ � ������� ����: ");
		letterForReplacement = getCyrrilicLetterOrExitSymbol();
		if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
		if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	}
	replaceLetter(srcLetter, letterForReplacement, data);
	addElementToHistory(srcLetter, letterForReplacement, data);

	system("cls");
	printf("������ ��������� �������.\n");
	printEncryptionKey(data->letter);
	printf("������� ������, ����� ��������� � ������� ����.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

char getCyrrilicLetterOrExitSymbol()
{
	char letter = '\n'; //��� ����������� �������� �� ��������� ENTER �� ������� ������
	while (letter == '\n') scanf("%c", &letter);
	letter = fixCodeForCyrillicChar(letter);
	while (letter != RETURN_TO_MENU_BTN_CODE && !isLetter(letter))
	{
		printf("������������ ������. ������� ����� �������� �������� ��� ������ ��� ������ � ������� ����: ");
		scanf("%c", &letter);
		while (letter == '\n') scanf("%c", &letter);
		letter = fixCodeForCyrillicChar(letter);
	}
	return letter;
}

//����������� ��������� ����� ��������, ������ ������� ����������������
char fixCodeForCyrillicChar(char item)
{
	if (-128 <= item && item <= -81) return item + 64;
	if (-32 <= item && item <= -17) return item + 16;
	return item;
}

void replaceLetter(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	(data->letter + srcLetter - '�')->replacedTo = letterForReplacement;
}

void handleAutoreplacement(cryptogram_t* data)
{
	system("cls");
	replaceAndUpdateHistoryAuto(data);
	printf("������� ������, ����� ��������� � ������� ����.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void replaceAndUpdateHistoryAuto(cryptogram_t* data)
{
	letter_t* srcLetter = findMaxFrqFromUndesiphered(data->letter);
	if (srcLetter == LETTER_IS_NOT_FOUND)
	{
		printf("���������� ���������� ����������� ������.\n");
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
			printf("������ %c ��� ������ �� %c.\n", srcLetter->symbol, letterForReplacement);
			srcLetter = nextSrcLetter;
		}
		else
		{
			deleteLastChange(data);
			printf("���������� ���������� ��������� ����������� ������.\n");
			break;
		}
	}
}