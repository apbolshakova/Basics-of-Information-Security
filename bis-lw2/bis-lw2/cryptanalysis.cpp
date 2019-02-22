/*ВОПРОСЫ:
Нужно ли очищать строку после каждого этапа
На основе чего предполагаются замены
"расшифрованный на данный момент текст" как выделить
Сколько истории хранить и на что должно быть похоже
Автозамена на основе каких предположений замен
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENT 0
#define EMPTY_TEXT 0
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "оеаинтсрвлкмдпуяыьгзбчйчжшюцщэфъ"
#define ALPHABET "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"

#define UNKNOWN_OPERATION_MESSAGE "Неизвестный код команды, попробуйте другой: "
#define INVALID_DATA_MESSAGE "Полученные данные не подходят для расшифровки: отсутствуют буквы."

typedef enum 
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
} OPERATION_CODE;

typedef enum 
{ 
	FALSE, 
	TRUE 
} BOOL;

typedef struct
{
	int encounteredInSrcText; //сколько раз встретился в тексте - для высчитывания частоты
	float frequencyInSrcText; //поссчитать при инициализации - в первом проходе
	char replacedTo;
} LETTER;

typedef struct
{
	char* text; //текст с текущими заменами для вывода
	LETTER* letter;
} CRYPTOGRAM;

void initLetters(CRYPTOGRAM* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->encounteredInSrcText = 0;
		(data->letter + i)->frequencyInSrcText = 0;
		(data->letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

BOOL isLetter(char item)
{
	if (item <= 'А' && item <= 'Я') return TRUE;
	else return FALSE;
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str)
{
	int sizeOfOldText = sizeof(data->text); 
	data->text = (char*) realloc(data->text, sizeOfOldText + SIZE_OF_STRING_TO_COPY * sizeof(char));

	char* item = data->text + sizeOfOldText - 1; //адрес, начиная с которого дописывать новые символы
	while (*str)
	{
		*item = *str;
		if (isLetter(*item))
		{
			(data->letter + (*item - 'A'))->encounteredInSrcText++;
		}
		item++;
		str++;
	}
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char) + ALPHABET_SIZE * sizeof(LETTER));
	//data->letter = (LETTER*)calloc(ALPHABET_SIZE, sizeof(LETTER));
	//data->text = (char*) malloc(sizeof(char));
	*(data->text) = EMPTY_TEXT;

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fscanf(f, "%s") != EOF))
	{
		initLetters(data);
		fclose(f);
		f = fopen(DATA_PATH, "r");
		char* temp = (char*) calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
		while (fgets(temp, sizeof(temp), f) != NULL)
		{
			handleDataFromNewString(data, temp);
		}
		free(temp);
	}
	fclose(f);
	return data;
}

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика. Возможные действия:\n");
	printf("1. Анализировать частоты букв и предложить замены\n");
	printf("2. Вывести слова, сгруппированные по количеству букв\n");
	printf("3. Вывести слова, сгруппированные по количеству нерасшифрованных букв\n");
	printf("4. Отобразить криптограмму\n");
	printf("5. Заменить буквы\n");
	printf("6. Отменить одну из сделанных замен\n");
	printf("7. Произвести автоматическую замену\n");
	printf("Введите код нужной команды: ");
}

void suggestReplacementsBasingOnFrequencyAnalysis(CRYPTOGRAM* data)
{
	//TODO
}

void printWordsInOrderByLength(char* text)
{
	//TODO
}

void printWordsInOrderByUndeciphered(char* text)
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
	OPERATION_CODE operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case ANALYZE: suggestReplacementsBasingOnFrequencyAnalysis(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->text); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->text); break;
		case PRINT_BUF: printText(data->text); break;
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
	if (data->text == EMPTY_TEXT) //в полученном тексте нет букв
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