#include "Header.h"

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика.");
	printf("Возможные действия: \n");
	printf("1. Предложить замену на основе частотного анализа\n");
	printf("2. Вывести слова из криптограммы\n");
	printf("3. Отобразить криптограмму\n");
	printf("4. Заменить буквы\n");
	printf("5. Отменить или вернуть последнее изменение\n");
	printf("6. Произвести автоматическую замену\n");
	printf("7. Выход\n");
	printf("Введите код нужной команды " );
	printf("(любой код, кроме перечисленных, будет проигнорирован): ");
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
		printf("Полученные данные не подходят для расшифровки: отсутствуют буквы.\n");
		_getch();
	}
	else handleMainCycle(data);

	//TODO: вынести в функции
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