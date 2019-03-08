#include "Header.h"

changes_list_t* initHistoryList()
{
	changes_list_t* initItem = (changes_list_t*)malloc(sizeof(changes_list_t));
	initItem->head = NULL;
	initItem->tail = NULL;
	return initItem;
}

void addElementToHistory(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	changes_list_item_t* newItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;

	newItem->next = NULL;
	newItem->prev = data->historyList->tail;

	if (data->historyList->tail) data->historyList->tail->next = newItem;
	data->historyList->tail = newItem;
	if (data->historyList->head == NULL) data->historyList->head = newItem;
}

void handleRevertMenu(cryptogram_t* data)
{
	history_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Выход в главное меню\n");
		if (data->historyList->tail != NULL)
		{
			printf("2. Отменить последнее действие: ");
			printf("замену %c на %c\n",
				data->historyList->tail->originalLetter, data->historyList->tail->replacedTo);
		}
		else printf("История замен пуста.\n");
		printf("Введите код нужной команды ");
		printf("(любой код, кроме перечисленных, будет проигнорирован): ");
		scanf("\n%c", &operationCode);
		if (operationCode == UNDO && data->historyList->tail != NULL) deleteLastChange(data);
	} while (operationCode != DECLINE_REVERT);
}

void undoLastChange(cryptogram_t* data)
{
	(data->letter + data->historyList->tail->originalLetter - 'А')->replacedTo = NO_REPLACEMENT;
	
	changes_list_item_t* next = data->historyList->tail;
	data->historyList->tail = data->historyList->tail->prev;
}

void deleteLastChange(cryptogram_t* data)
{
	undoLastChange(data);
	data->historyList->tail = NULL;
	free(data->historyList->tail);
}