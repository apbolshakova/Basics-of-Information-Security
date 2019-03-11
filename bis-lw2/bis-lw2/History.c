#include "Header.h"

changes_list_t* initHistoryList()
{
	changes_list_t* initItem = (changes_list_t*)malloc(sizeof(changes_list_t));
	if (initItem == NULL) return NULL;
	initItem->head = NULL;
	initItem->tail = NULL;
	return initItem;
}

void addElementToHistory(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	changes_list_item_t* newItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	if (newItem == NULL) return;
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;

	newItem->next = NULL;

	if (data->historyList->tail == NULL)
	{
		newItem->prev = NULL;
		data->historyList->head = newItem;
		data->historyList->tail = newItem;
	}
	else
	{
		newItem->prev = data->historyList->tail;
		data->historyList->tail->next = newItem;
		data->historyList->tail = data->historyList->tail->next;
	}
}

void handleRevertMenu(cryptogram_t* data)
{
	history_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. ����� � ������� ����\n");
		if (data->historyList->tail != NULL)
		{
			printf("2. �������� ��������� ��������: ");
			printf("������ %c �� %c\n",
				data->historyList->tail->originalLetter, data->historyList->tail->replacedTo);
		}
		else printf("������� ����� �����.\n");
		printf("������� ��� ������ ������� ");
		printf("(����� ���, ����� �������������, ����� ��������������): ");
		scanf("\n%c", &operationCode);
		if (operationCode == UNDO && data->historyList->tail != NULL) deleteLastChange(data);
	} while (operationCode != DECLINE_REVERT);
}

void deleteLastChange(cryptogram_t* data)
{
	(data->letter + data->historyList->tail->originalLetter - '�')->replacedTo = NO_REPLACEMENT; 
	
	data->historyList->tail = data->historyList->tail->prev;
	if (data->historyList->tail == NULL) data->historyList->head = NULL;
	else
	{
		data->historyList->tail->next = NULL;
		free(data->historyList->tail->next);
	}
}

void cleanHistoryList(cryptogram_t* data)
{
	changes_list_item_t *item = data->historyList->head;
	changes_list_item_t *nextItem = NULL;
	while (item)
	{
		nextItem = item->next;
		free(item);
		item = nextItem;
	}
	free(data->historyList);
	(data->historyList) = NULL;
}