#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NO_REPLACEMENT 0
#define NO_LETTERS_IN_TEXT 0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "оеаинтсрвлкмдпуяыьгзбчйчжшюцщэфъ"

#define INVALID_DATA_MESSAGE "Полученные данные не подходят для расшифровки: отсутствуют буквы.\n"
#define SUCESS_INIT_MESSAGE "Исходный текст криптограммы успешно загружен.\n"
#define RETURN_TO_MENU_MESSAGE "Нажмите пробел, чтобы вернуться в главное меню.\n"

typedef enum OperationCode
{
	NULL_OPERATION,              //операция для инициализации
	PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT = '1', //вывод предполагаемой замены в соответстви с частотами распределения букв русского алфавита
	PRINT_WORDS_BY_LENGTH,       //вывод на экран всех слов, сгруппированных по количеству букв
	PRINT_WORDS_BY_UNDECIPHERED, //вывод на экран всех слов, сгруппированных по количеству нерасшифрованных на данный момент букв
	PRINT_CRYPTOGRAM,            //отображение криптограммы с указанием расшифрованного на данный момент текста
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
	char symbol;
	int encounteredInSrcText;
	float frequencyInSrcText;
	char replacedTo;
} LETTER;

typedef struct ChangesListItem
{
	struct ChangesListItem* prev;
	struct ChangesListItem* next;
	char originalLetter;
	char replacedTo;
} CHANGES_LIST_ITEM;

typedef struct Cryptogram
{
	char* text;
	int numOfLetters;
	LETTER* letter;
	CHANGES_LIST_ITEM* curChange;
} CRYPTOGRAM;

//выделить массив слов (слово содержит символы, свою длину, сколько разных букв капсом), получить наибольшую длину
typedef struct Word
{
	char* chars;
	int len;
	int numOfUndecipheredLetters;
} WORD;

void initLetters(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->symbol = 'А' + i;
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

CHANGES_LIST_ITEM* initChangesList()
{
	CHANGES_LIST_ITEM* initialItem = (CHANGES_LIST_ITEM*)malloc(2 * sizeof(CHANGES_LIST_ITEM*) + 2 * sizeof(char));
	initialItem->prev = NULL;
	initialItem->next = NULL;
	initialItem->originalLetter = NO_REPLACEMENT;
	initialItem->replacedTo = NO_REPLACEMENT;
	return initialItem;
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
			data->numOfLetters++;
			(data->letter + (*(data->text) - 'А'))->encounteredInSrcText++;
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

void calculateFrequencies(CRYPTOGRAM* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->frequencyInSrcText = (float) (data->letter + i)->encounteredInSrcText / data->numOfLetters;
	}
}

int cmpByFrequencyDesc(const void *a, const void *b)
{
	float aFrq = ((LETTER*)a)->frequencyInSrcText;
	float bFrq = ((LETTER*)b)->frequencyInSrcText;
	if (aFrq > bFrq) return -1;
	if (aFrq < bFrq) return 1;
	return 0;
}

int cmpBySymbolAsc(const void *a, const void *b)
{
	float aSymbol = ((LETTER*)a)->symbol;
	float bSymbol = ((LETTER*)b)->symbol;
	if (aSymbol > bSymbol) return -1; //кириллица имеет отрицательные коды
	if (aSymbol < bSymbol) return 1;
	return 0;
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = malloc(sizeof(char*) + sizeof(LETTER*) + sizeof(int) + sizeof(CHANGES_LIST_ITEM*));
	data->text = malloc(sizeof(char));
	*(data->text) = '\0';
	data->letter = (LETTER*) calloc(ALPHABET_SIZE, sizeof(LETTER));
	data->numOfLetters = 0;
	data->curChange = initChangesList();

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndCalculateEncounters(data, f);
	}
	fclose(f);
	if (data->numOfLetters != NO_LETTERS_IN_TEXT) calculateFrequencies(data);
	return data;
}

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика. Возможные действия:\n");
	printf("1. Предложить замену на основе частотного анализа\n");
	printf("2. Вывести слова, сгруппированные по количеству букв\n");
	printf("3. Вывести слова, сгруппированные по количеству нерасшифрованных букв\n");
	printf("4. Отобразить криптограмму\n");
	printf("5. Заменить буквы\n");
	printf("6. Отменить одну из сделанных замен\n");
	printf("7. Произвести автоматическую замену\n");
	printf("8. Выход\n");
	printf("Введите код нужной команды (любой код, кроме перечисленных, будет проигнорирован): ");
}

void printText(CRYPTOGRAM* data)
{
	char* ptr = data->text;
	while (*(ptr))
	{
		int curSymbolIndex = *(ptr)-'А';
		if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *(ptr));
		else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		ptr++;
	}
	printf("\n\n");
}

void printEncryptionKey(LETTER* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> ", (letter + i)->symbol);
		if ((letter + i)->replacedTo == NO_REPLACEMENT) printf("замена не определена");
		else printf("%c", (letter + i)->replacedTo);
		printf("\n");
	}
	printf("\n");
}

char findLetterWithBiggestFrequencyFromUndesiphered(LETTER* letter)
{
	int index = 0;
	qsort(letter, ALPHABET_SIZE, sizeof(*letter), cmpByFrequencyDesc); //отсортировать буквы по частотам
	while (index < ALPHABET_SIZE)
	{
		if ((letter + index)->replacedTo == NO_REPLACEMENT) break;
		else index++;
	}
	char result = (letter + index)->symbol;
	qsort(letter, ALPHABET_SIZE, sizeof(*letter), cmpBySymbolAsc); //вернуть сортировку по символам
	return result;
}

BOOL isUsedAsReplacement(char symbolToCheck, LETTER* letter)
{
	BOOL isUsed = FALSE;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (symbolToCheck == (letter + i)->replacedTo)
		{
			isUsed = TRUE;
			break;
		}
	}
	return isUsed;
}

char findLetterWithBiggestFrequencyFromUnusedAsReplacement(LETTER* letter)
{
	int index = 0;
	while (index < ALPHABET_SIZE)
	{
		if (isUsedAsReplacement(LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index], letter)) index++;
		else break;
	}
	return LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index];
}

void printLettersFrequencies(LETTER* letter)
{
	printf("Частоты встреч букв во входной криптограмме:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.2f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(LETTER* letter)
{
	char srcLetter = findLetterWithBiggestFrequencyFromUndesiphered(letter);
	char letterForReplacement = findLetterWithBiggestFrequencyFromUnusedAsReplacement(letter);
	printf("Исходя из частотного анализа сделан вывод, что букву %c, возможно, следует поменять на %c.\n", srcLetter, letterForReplacement);
	//TODO: выводить превью с предположенной заменой
}

void analyseFrequencyAndSuggestReplacement(CRYPTOGRAM* data)
{
	system("cls");

	printLettersFrequencies(data->letter);
	printReplacementSuggestion(data->letter);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void printWordsInOrderByLength(char* text)
{
	//WORD* word = parseTextIntoWords(text); //указатель на первое слово
	//qsort(word, число слов, sizeof(WORD), cmpByFrequencyDesc);
	//TODO
	//выделить массив слов (слово содержит символы, свою длину, сколько разных букв капсом), получить наибольшую длину
	//вывести от наибольшей длины к наименьшей
}

void printWordsInOrderByUndeciphered(char* text)
{
	//TODO
	//выделить массив слов (слово содержит символы, свою длину, сколько разных букв капсом), получить наибольшее количество букв капсом
	//вывести от наибольшей длины к наименьшей
}

void printCryptogram(CRYPTOGRAM* data)
{
	system("cls");
	printf("Текущий ключ шифрования:\n");
	printEncryptionKey(data->letter);

	printf("Текст криптограммы с выполненными заменами:\n\n");
	printText(data);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	//TODO
	//Вывести текущий ключ шифрования и текст криптограммы
	//попросить ввести букву для замены
	//на какую букву
	//замена
	//успешная замена - ещё одна или главное меню
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	//TODO
	//Вывести текущий ключ шифрования и текст криптограммы
	//нет замен - сообщить
	//последняя замена - предложить откат
	//не последняя замена - предложить откат или возврат
	//при выборе действия меняем curChange на эл-т вперёд или назад
	//ещё раз или главное меню
}

void replaceLettersAutomatically(CRYPTOGRAM* data)
{
	//TODO
	//сортировать буквы letters по частоте (по убыванию) в тексте
	//от 0 до 31: уникальная частота - сменить на i-ую из сортированного списка для русского алфавита
	//            не уникальная частота - прекратить автозамену
}

void handleMainCycle(CRYPTOGRAM* data)
{
	OPERATION_CODE operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT: analyseFrequencyAndSuggestReplacement(data); break;
		case PRINT_WORDS_BY_LENGTH: printWordsInOrderByLength(data->text); break;
		case PRINT_WORDS_BY_UNDECIPHERED: printWordsInOrderByUndeciphered(data->text); break;
		case PRINT_CRYPTOGRAM: printCryptogram(data); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: replaceLettersAutomatically(data); break;
		//TODO: функция, выводящая частоты букв исходного текста
		default: break;
		}
	} while (operationCode != EXIT);
}

int main(void)
{
	setlocale(LC_ALL, "rus");

	CRYPTOGRAM* data = initCryptogram();
	if (data->numOfLetters == NO_LETTERS_IN_TEXT)
	{
		printf(INVALID_DATA_MESSAGE);
		_getch();
	}
	else handleMainCycle(data);
	//TODO: добавить функцию, подчищающую память
	free(data);
	return 0;
}