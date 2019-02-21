/*ВОПРОСЫ:
Нужно ли очищать строку после каждого этапа
На основе чего предполагаются замены
"расшифрованный на данный момент текст" как выделить
Сколько истории хранить
Автозамена на основе каких предположений замен
*/

#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>

#define MENU_TEXT "Menu text" //TODO: написать меню
#define UNKNOWN_OPERATION_MESSAGE "Unknown operation code. Enter another one: " //TODO: написать меню


enum OperationCode 
{
	NULL_OPERATION, //операция для инициализации
	ANALYZE,        //анализ частоты букв во входном файле и вывод предполагаемых замен в соответстви с частотами распределения букв русского алфавита
	PRINT_WORDS,    //вывод на экран всех слов, сгруппированных по количеству нерасшифрованных на данный момент букв
	PRINT_BUF,      //отображение криптограммы с указанием расшифрованного на данный момент текста
	REPLACE_LETTERS, //возможность замены букв в криптограмме
	REVERT,         //хранение и откат истории замены букв в криптограмме
	AUTOREPLACEMENT,     //автоматическая замена букв
	EXIT 
};

char* getBufFromInput()
{
	//TODO
}

void suggestReplacementsBasingOnFrequencyAnalysis(char* buf)
{
	//TODO
}

void printWordsSortedByUndecipheredLetters(char* buf)
{
	//TODO
}

void printBuf(char* buf)
{
	//TODO
}

void handleReplacementMenu(char* buf)
{
	//TODO
}

void printHistoryAndRevertBuf(char* buf)
{
	//TODO
}

void replaceLettersAutomatically(char* buf)
{
	//TODO
}

void handleMainCycle(char* buf)
{
	OperationCode operationCode = NULL_OPERATION;
	do 
	{
		printf(MENU_TEXT);
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(buf); break;
		case PRINT_WORDS: printWordsSortedByUndecipheredLetters(buf); break;
		case PRINT_BUF: printBuf(buf);  break;
		case REPLACE_LETTERS: handleReplacementMenu(buf); break; //скорее всего замена нужна не одна за раз => отдельная менюшка
		case REVERT: printHistoryAndRevertBuf(buf); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(buf); break;
		case EXIT: break;
		default: printf(UNKNOWN_OPERATION_MESSAGE);
		}
		printf("\n\n"); //для отделения выполненных операций
	} while (operationCode != EXIT);
}

int main(void)
{
	char* buf = getBufFromInput(); //получить строку из входного файла в buf
	handleMainCycle(buf); //запуск основного цикла программы
	return 0;
}