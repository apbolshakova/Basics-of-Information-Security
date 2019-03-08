#include "Header.h"

void printMainMenu()
{
	printf("��� ������������ ���������, ����������� ������� ����������� ���������������.");
	printf("��������� ��������: \n");
	printf("1. ���������� ������ �� ������ ���������� �������\n");
	printf("2. ������� ����� �� ������������\n");
	printf("3. ���������� ������������\n");
	printf("4. �������� �����\n");
	printf("5. �������� ��� ������� ��������� ���������\n");
	printf("6. ���������� �������������� ������\n");
	printf("7. �����\n");
	printf("������� ��� ������ ������� " );
	printf("(����� ���, ����� �������������, ����� ��������������): ");
}

void handleMainCycle(cryptogram_t* data)
{
	operation_code_t operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case SUGGEST_REPLACEMENT: suggestReplacement(data); break;
		case PRINT_WORDS: handleWordsPrintingMenu(data); break;
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
	cryptogram_t* data = initCryptogram();
	if (data->numOfLetters == NO_LETTERS)
	{
		printf("���������� ������ �� �������� ��� �����������: ����������� �����.\n");
		_getch();
	}
	else handleMainCycle(data);

	//TODO: ������� � �������
	changes_list_item_t* changesItem = data->curChange->head;
	changes_list_item_t* nextChangesItem = NULL;
	while (changesItem)
	{
		nextChangesItem = changesItem->next;
		free(changesItem);
		changesItem = nextChangesItem;
	}
	free(data->curChange);
	data->curChange = NULL;


	word_list_item_t* wordsItem = data->words->firstWord;
	word_list_item_t* nextWordsItem = NULL;
	while (wordsItem)
	{
		nextWordsItem = wordsItem->nextWord;
		free(wordsItem);
		wordsItem = nextWordsItem;
	}
	free(data->words);
	data->words = NULL;
	

	data->words = NULL;
	free(data->words);

	data->text = NULL;
	free(data->text);

	data->curChange = NULL;
	free(data->curChange);

	data->letter = NULL;
	free(data->letter);

	data = NULL;
	free(data);
	return 0;
}