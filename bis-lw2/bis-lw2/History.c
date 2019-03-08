#include "Header.h"

changes_list_item_t* initChangesList()
{
	changes_list_item_t* initialItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	initialItem->head = NULL;
	initialItem->prev = NULL;
	initialItem->next = NULL;
	initialItem->originalLetter = NO_REPLACEMENT;
	initialItem->replacedTo = NO_REPLACEMENT;
	return initialItem;
}

void addElementToHistory(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	changes_list_item_t* newItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;
	newItem->next = NULL;
	newItem->prev = data->lastChange;
	if (data->lastChange) data->lastChange->next = newItem;
	data->lastChange = newItem;
	if (data->lastChange->head == NULL) data->lastChange->head = newItem;
}

void handleRevertMenu(cryptogram_t* data)
{
	history_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Выход в главное меню\n");
		if (data->lastChange->prev == NULL && data->lastChange->next == NULL)
		{
			printf("История замен пуста.\n");
		}
		else
		{
			if (data->lastChange->prev != NULL)
			{
				printf("2. Отменить последнее действие: ");
				printf("замену %c на %c\n",
					data->lastChange->originalLetter, data->lastChange->replacedTo);
			}
			if (data->lastChange->next != NULL)
			{
				printf("3. Восстановить последнее отменённое действие: ");
				printf("замену %c на %c\n",
					data->lastChange->next->originalLetter,
					data->lastChange->next->replacedTo);
			}
		}

		printf("Введите код нужной команды ");
		printf("(любой код, кроме перечисленных, будет проигнорирован): ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case UNDO: if (data->lastChange->prev != NULL) undoLastChange(data); break;
		case REDO: if (data->lastChange->next != NULL) redoLastChange(data); break;
		default: break;
		}
	} while (operationCode != DECLINE_REVERT);
}

void undoLastChange(cryptogram_t* data)
{
	(data->letter + data->lastChange->originalLetter - 'А')->replacedTo = NO_REPLACEMENT;
	changes_list_item_t* oldLastChange = data->lastChange;
	data->lastChange = data->lastChange->prev;
	data->lastChange->next = oldLastChange;
}

void redoLastChange(cryptogram_t* data)
{
	changes_list_item_t* oldLastChange = data->lastChange;
	data->lastChange = data->lastChange->next;
	data->lastChange->prev = oldLastChange;
	(data->letter + data->lastChange->originalLetter - 'А')->replacedTo =
		data->lastChange->replacedTo;
}

void deleteLastChange(cryptogram_t* data)
{
	undoLastChange(data);
	data->lastChange->next = NULL;
	free(data->lastChange->next);
}