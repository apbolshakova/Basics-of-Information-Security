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

#define SIZE_OF_STRING_TO_COPY 1000

#define UNKNOWN_OPERATION_MESSAGE "Неизвестный код команды, попробуйте другой: "
#define INVALID_DATA_MESSAGE "Полученные данные не подходят для расшифровки: отсутствуют буквы."
#define DATA_PATH "input.txt"

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

void handleDataFromNewString(CRYPTOGRAM* data, char* str)
{
	//увеличить память в тексте
	//скопировать текст в data, во время копирования выделять слова в word и буквы в srcLetter, выделять память в curLetter
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data;
	data->srcLetter = (char*) malloc(sizeof(char));
	*(data->srcLetter) = 0;

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fscanf(f, "%s") != EOF))
	{
		fclose(f);
		f = fopen(DATA_PATH, "r");
		char* temp = (char*) malloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
		while (fgets(temp, sizeof(temp), f) != NULL)
		{
			handleDataFromNewString(data, temp);
		}
	}
	fclose(f);
	return data;

	//прочитать текст в curText
	//в массиве word получить слова
	//наполнить srcLetter буквами или нулём
	//выделить столько же памяти для curLetter и проинициализировать
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

void printWordsInOrderByLength(char** word)
{
	//TODO
}

void printWordsInOrderByUndeciphered(char** word)
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
	enum OperationCode operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->word); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->word); break;
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
	}
	else
	{
		handleMainCycle(data);
	}
	free(data);
	return 0;
}