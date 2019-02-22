/*ВОПРОСЫ:
Нужно ли очищать строку после каждого этапа
На основе чего предполагаются замены
"расшифрованный на данный момент текст" как выделить
Сколько истории хранить и на что должно быть похоже
Автозамена на основе каких предположений замен
*/

#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define UNKNOWN_OPERATION_MESSAGE "Неизвестный код команды, попробуйте другой: "

enum OperationCode 
{
	NULL_OPERATION,              //операция для инициализации
	ANALYZE,                     //анализ частоты букв во входном файле и вывод предполагаемых замен в соответстви с частотами распределения букв русского алфавита
	PRINT_WORDS_BY_LENGTH,       //вывод на экран всех слов, сгруппированных по количеству букв
	PRINT_WORDS_BY_UNDECIPHERED, //вывод на экран всех слов, сгруппированных по количеству нерасшифрованных на данный момент букв
	PRINT_BUF,                   //отображение криптограммы с указанием расшифрованного на данный момент текста
	REPLACE_LETTERS,             //возможность замены букв в криптограмме
	REVERT,                      //хранение и откат истории замены букв в криптограмме
	AUTOREPLACEMENT,             //автоматическая замена букв
	EXIT 
};

typedef struct Cryptogram
{
	char* curText; //текст с текущими заменами для вывода
	char** word; //массив элементов-слов для вывода
	char* srcLetter; //массив исходных букв C, D, A, F для откатов
	char* curLetter; //массив замен '','', b, '' -> A поменяли на b. Тогда откат - удаление элемента из этого массива
} CRYPTOGRAM;

CRYPTOGRAM* initCryptogram()
{
	return 0;
}

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика. Возможные действия:\n");
	printf("1. Анализировать частоты букв и предложить замены\n");
	printf("2. Вывести слова, сгруппированные по количеству букв\n");
	printf("3. Вывести слова, сгруппированные по количеству нерасшифрованных букв\n");
	printf("4. Отобразить криптограмму\n");
	printf("5. Заменить буквы\n");
	printf("6. Отменить замену\n");
	printf("7. Произвести автоматическую замену\n");
	printf("Введите код нужной команды: ");
}

void suggestReplacementsBasingOnFrequencyAnalysis(CRYPTOGRAM* data)
{
	//TODO
}

void printWordsOrderByLength(char** word)
{
	//TODO
}

void printWordsOrderByUndeciphered(char** word)
{
	//TODO
}

void printText(char* text)
{
	//TODO
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	//TODO
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	//TODO
	//вывести список проведённых замен и предложить возможность отмены
}

void replaceLettersAutomatically(CRYPTOGRAM* data)
{
	//TODO
}

void handleMainCycle(CRYPTOGRAM* data)
{
	OperationCode operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsOrderByLength(data->word); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsOrderByUndeciphered(data->word); break;
		case PRINT_BUF: printText(data->curText); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		case EXIT: break;
		default: printf(UNKNOWN_OPERATION_MESSAGE);
		}
	} while (operationCode != EXIT);
}

int main(void)
{
	CRYPTOGRAM* data = initCryptogram();
	if (data->srcLetter == 0) //в полученном тексте нет букв
	{
		printf("Полученные данные не подходят для расшифровки: отсутствуют буквы.");
		_getch();
		return 0;
	}
	handleMainCycle(data);
	return 0;
}