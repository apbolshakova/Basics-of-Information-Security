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
	newItem->prev = data->curChange;
	if (data->curChange) data->curChange->next = newItem;
	data->curChange = newItem;
	if (data->curChange->head == NULL) data->curChange->head = newItem;
}

void handleRevertMenu(cryptogram_t* data)
{
	history_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. ����� � ������� ����\n");
		if (data->curChange->prev == NULL && data->curChange->next == NULL)
		{
			printf("������� ����� �����.\n");
		}
		else
		{
			if (data->curChange->prev != NULL)
			{
				printf("2. �������� ��������� ��������: ");
				printf("������ %c �� %c\n",
					data->curChange->originalLetter, data->curChange->replacedTo);
			}
			if (data->curChange->next != NULL)
			{
				printf("3. ������������ ��������� ��������� ��������: ");
				printf("������ %c �� %c\n",
					data->curChange->next->originalLetter,
					data->curChange->next->replacedTo);
			}
		}

		printf("������� ��� ������ ������� ");
		printf("(����� ���, ����� �������������, ����� ��������������): ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case UNDO: if (data->curChange->prev != NULL) undoCurChange(data); break;
		case REDO: if (data->curChange->next != NULL) redoCurChange(data); break;
		default: break;
		}
	} while (operationCode != DECLINE_REVERT);
}

void undoCurChange(cryptogram_t* data)
{
	(data->letter + data->curChange->originalLetter - '�')->replacedTo = NO_REPLACEMENT;
	changes_list_item_t* oldCurChange = data->curChange;
	data->curChange = data->curChange->prev;
	data->curChange->next = oldCurChange;
}

void redoCurChange(cryptogram_t* data)
{
	changes_list_item_t* oldCurChange = data->curChange;
	data->curChange = data->curChange->next;
	data->curChange->prev = oldCurChange;
	(data->letter + data->curChange->originalLetter - '�')->replacedTo =
		data->curChange->replacedTo;
}

void deleteCurChange(cryptogram_t* data)
{
	undoCurChange(data);
	data->curChange->next = NULL;
	free(data->curChange->next);
}