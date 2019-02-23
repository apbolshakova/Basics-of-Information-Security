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
#include <locale.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENTS 0
#define EMPTY_TEXT '\0'
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "оеаинтсрвлкмдпуяыьгзбчйчжшюцщэфъ"
//#define ALPHABET "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" TODO: нужен ли

#define INVALID_DATA_MESSAGE "Полученные данные не подходят для расшифровки: отсутствуют буквы.\n"
#define SUCESS_INIT_MESSAGE "Исходный текст криптограммы успешно загружен.\n"
#define RETURN_TO_MENU_MESSAGE "Нажмите пробел, чтобы вернуться в главное меню.\n"

typedef enum 
{
	NULL_OPERATION,              //операция для инициализации
	ANALYZE,                     //анализ частоты букв во входном файле и вывод предполагаемых замен в соответстви с частотами распределения букв русского алфавита
	PRINT_WORDS_BY_LENGTH,       //вывод на экран всех слов, сгруппированных по количеству букв
	PRINT_WORDS_BY_UNDECIPHERED, //вывод на экран всех слов, сгруппированных по количеству нерасшифрованных на данный момент букв
	PRINT_TEXT,                  //отображение криптограммы с указанием расшифрованного на данный момент текста
	REPLACE_LETTERS,             //возможность замены букв в криптограмме
	REVERT,                      //хранение и откат истории замены букв в криптограмме
	AUTOREPLACEMENT,             //автоматическая замена букв
	EXIT 
} OPERATION_CODE;

typedef enum Bool
{ 
	FALSE, 
	TRUE 
} BOOL;

typedef struct Letter
{
	int encounteredInSrcText; //сколько раз встретился в тексте
	float frequencyInSrcText; //поссчитать при инициализации - в первом проходе
	char replacedTo;
} LETTER;

typedef struct Cryptogram
{
	char* text; //текст с текущими заменами для вывода
	LETTER* letter;
} CRYPTOGRAM;

void initLetters(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENTS;
	}
}

BOOL isLetter(char item)
{
	if ('А' <= item && item <= 'Я') return TRUE;
	else return FALSE;
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str) //TODO: рефакторинг
{
	int sizeOfOldText = 0; 
	char* sav = data->text;
	while (*(data->text)) //получить место, где строка кончается
	{
		sizeOfOldText++;
		data->text++;
	}

	data->text = sav;
	data->text = (char*) realloc(data->text, (sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char)); //увеличить память
	
	sav = data->text;
	data->text += sizeOfOldText;
	while (*str) //копировать символы
	{
		*(data->text) = *str;
		if (isLetter(*(data->text)))
		{
			(data->letter + (*(data->text) + 64))->encounteredInSrcText++;
		}
		(data->text)++;
		str++;
	}
	*(data->text) = '\0';
	data->text = sav;
}

void initTextAndCalculateEncounters(CRYPTOGRAM* data, FILE* f)
{
	fclose(f);
	f = fopen(DATA_PATH, "r");
	char* temp = (char*) calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
	while (fgets(temp, SIZE_OF_STRING_TO_COPY * sizeof(char), f) != NULL) handleDataFromNewString(data, temp);
	free(temp);
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char*) + sizeof(LETTER*));
	data->text = malloc(sizeof(char));
	data->letter = (LETTER*)calloc(ALPHABET_SIZE, sizeof(LETTER));
	*(data->text) = EMPTY_TEXT;

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndCalculateEncounters(data, f);
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
	printf("8. Выход\n");
	printf("Введите код нужной команды (любой код, кроме перечисленных, будет проигнорирован): ");
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
	system("cls");
	printf("Текст криптограммы с выполненными заменами:\n\n");
    
	char* ptr = text;
	while (*(ptr)) printf("%c", *(ptr++));
	printf("\n\n");

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
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
		case PRINT_TEXT: printText(data->text); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		default: break;
		}
	} while (operationCode != EXIT);
}

int main(void)
{
	setlocale(LC_ALL, "rus");

	CRYPTOGRAM* data = initCryptogram();
	if (*(data->text) == EMPTY_TEXT) //в полученном тексте пусто
	{
		printf(INVALID_DATA_MESSAGE);
		_getch();
	}
	else handleMainCycle(data);
	free(data);
	return 0;
}