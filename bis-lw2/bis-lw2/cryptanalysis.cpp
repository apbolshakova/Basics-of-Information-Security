#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <conio.h>
#include <ctype.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NULL_OPERATION 0
#define NO_REPLACEMENT 0
#define NO_LETTERS_IN_TEXT 0
#define NO_LETTERS_IN_WORD 0
#define LETTER_IS_NOT_FOUND 0
#define INITIAL_FRQ 0.0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "оеаинтсрвлкмдпуяыьгзбчйчжшюцщэфъ"
#define RETURN_TO_MENU_MESSAGE "Нажмите пробел, чтобы вернуться в главное меню.\n"

typedef enum OperationCode
{
	PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT = '1',
	PRINT_WORDS,
	PRINT_CRYPTOGRAM,     
	REPLACE_LETTERS,
	REVERT,   
	AUTOREPLACEMENT, 
	EXIT 
} OPERATION_CODE;

typedef enum Bool
{ 
	FALSE, 
	TRUE 
} BOOL;

typedef enum PrintingOperationCode
{
	BY_LENGTH = '1',
	BY_UNDECIPHERED,
	DECLINE_PRINTING
} PRINTING_OPERATION_CODE;

typedef enum HistoryOperationCode
{
	DECLINE_REVERT = '1',
	UNDO,
	REDO
} HISTORY_OPERATION_CODE;

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

typedef struct wordListItem
{
	char* chars;
	int len;
	int numOfUndecipheredLetters;
	struct wordListItem* nextWord;
} WORD_LIST_ITEM;

typedef struct wordsInfo
{
	WORD_LIST_ITEM* firstWord;
	WORD_LIST_ITEM* lastWord;
} WORDS_INFO;

typedef struct Cryptogram
{
	char* text;
	int numOfLetters;
	LETTER* letter;
	CHANGES_LIST_ITEM* curChange;
	WORDS_INFO* words;
} CRYPTOGRAM;

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

BOOL isCapitalLetter(char item)
{
	if ('А' <= item && item <= 'Я') return TRUE;
	else return FALSE;
}

BOOL isLowercaseLetter(char item)
{
	if ('а' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

BOOL isLetter(char item)
{
	if ('А' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

char fixCodeForCyrillicCharFromInput(char item) //исправление считанных кодов символов, сдвиги найдены экспериментально
{
	if (-128 <= item && item <= -81) return item + 64;
	if (-32 <= item && item <= -17) return item + 16;
	return item;
}

int moveTextPtrToTheEndAndGetSizeOfText(char* text)
{
	int sizeOfText = 0;
	while (*(text))
	{
		sizeOfText++;
		text++;
	}
	return sizeOfText;
}

void copyStringAndCalculateLettersEncounters(CRYPTOGRAM* data, char* str)
{
	while (*str)
	{
		*(data->text) = *str;
		if (isCapitalLetter(*(data->text)))
		{
			data->numOfLetters++;
			(data->letter + (*(data->text) - 'А'))->encounteredInSrcText++;
		}
		(data->text)++;
		str++;
	}
	*(data->text) = '\0';
}

void handleDataFromNewString(CRYPTOGRAM* data, char* str)
{
	char* sav = data->text;
	int sizeOfOldText = moveTextPtrToTheEndAndGetSizeOfText(data->text);

	data->text = sav;
	data->text = (char*) realloc(data->text, (sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char) + 1);
	sav = data->text;

	data->text += sizeOfOldText;
	copyStringAndCalculateLettersEncounters(data, str);
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
	if (aSymbol > bSymbol) return 1;
	if (aSymbol < bSymbol) return -1;
	return 0;
}

BOOL areSameWords(char* a, char* b)
{
	BOOL areSame = TRUE;
	while (*a && *b)
	{
		if (*a != *b)
		{
			areSame = FALSE;
			break;
		}
		a++;
		b++;
	}
	if (*a || *b) areSame = FALSE; //должны были закончиться одновременно
	return areSame;
}

BOOL wordIsUnique(WORD_LIST_ITEM* newWord, WORDS_INFO* wordsInfo)
{
	WORD_LIST_ITEM* item = wordsInfo->firstWord;
	BOOL isUnique = TRUE;
	while (item != NULL && isUnique)
	{
		if (item->len == newWord->len && 
			areSameWords(item->chars, newWord->chars)) isUnique = FALSE;
		item = item->nextWord;
	}
	return isUnique;
}

void addNewWordToList(WORD_LIST_ITEM* newWord, WORDS_INFO* wordsInfo)
{
	if (wordsInfo->lastWord == NULL) //список пуст
	{
		wordsInfo->firstWord = newWord;
		wordsInfo->lastWord = wordsInfo->firstWord;
	}
	else
	{
		wordsInfo->lastWord->nextWord = newWord;
		wordsInfo->lastWord = newWord;
	}
	wordsInfo->lastWord->nextWord = NULL;
}

char* handleWordAndMovePtrToTheEndOfIt(WORDS_INFO* wordsInfo, char* text) //TODO: рефакторинг
{
	char* textSav = text;
	WORD_LIST_ITEM* newWord = (WORD_LIST_ITEM*)malloc(sizeof(WORD_LIST_ITEM));
	newWord->len = 0;
	newWord->numOfUndecipheredLetters = 0; 
	while (isLetter(*text))
	{
		newWord->len++;
		text++;
	}
	newWord->chars = (char*)malloc(newWord->len * sizeof(char) + 1);
	
	text = textSav;
	char* charsSav = newWord->chars;
	while (isLetter(*text))
	{
		*(newWord->chars) = *text;
		text++;
		newWord->chars++;
	}
	*(newWord->chars) = '\0';
	newWord->chars = charsSav;
	if (wordIsUnique(newWord, wordsInfo)) addNewWordToList(newWord, wordsInfo);
	return text;
}

WORDS_INFO* parseTextIntoWords(char* text)
{
	char* sav = text;
	WORDS_INFO* wordsInfo = (WORDS_INFO*)malloc(sizeof(WORDS_INFO));
	wordsInfo->firstWord = NULL;
	wordsInfo->lastWord = NULL;
	while (*text)
	{
		while (*text && !isLetter(*text)) text++;
		if (isLetter(*text)) text = handleWordAndMovePtrToTheEndOfIt(wordsInfo, text);
	}
	text = sav;
	return wordsInfo;
}

CRYPTOGRAM* initCryptogram()
{
	CRYPTOGRAM* data = (CRYPTOGRAM*) malloc(sizeof(CRYPTOGRAM));
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
	if (data->numOfLetters != NO_LETTERS_IN_TEXT)
	{
		calculateFrequencies(data);
		data->words = parseTextIntoWords(data->text);
	}
	return data;
}

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика. Возможные действия:\n");
	printf("1. Предложить замену на основе частотного анализа\n");
	printf("2. Вывести слова из криптограммы\n");
	printf("3. Отобразить криптограмму\n");
	printf("4. Заменить буквы\n");
	printf("5. Отменить или вернуть последнее изменение\n");
	printf("6. Произвести автоматическую замену\n");
	printf("7. Выход\n");
	printf("Введите код нужной команды (любой код, кроме перечисленных, будет проигнорирован): ");
}

void printText(CRYPTOGRAM* data)
{
	printf("Текст криптограммы с выполненными заменами:\n\n");
	char* ptr = data->text;
	while (*ptr)
	{
		if (isLetter(*ptr))
		{
			int curSymbolIndex = *(ptr)-'А';
			if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *ptr);
			else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		}
		else printf("%c", *ptr);
		ptr++;
	}
	printf("\n\n");
}

void printEncryptionKey(LETTER* letter)
{
	printf("Текущий ключ шифрования:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> ", (letter + i)->symbol);
		if ((letter + i)->replacedTo == NO_REPLACEMENT) printf("замена не определена");
		else printf("%c", (letter + i)->replacedTo);
		printf("\n");
	}
	printf("\n");
}

LETTER* findLetterWithMaxFrequencyFromUndesiphered(LETTER* letter)
{
	LETTER* letterWithMaxFrq = LETTER_IS_NOT_FOUND;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if ((letter + i)->replacedTo == NO_REPLACEMENT
			&& (letterWithMaxFrq == LETTER_IS_NOT_FOUND ||
				letterWithMaxFrq->frequencyInSrcText < (letter + i)->frequencyInSrcText))
		{
			letterWithMaxFrq = letter + i;
		}
	}
	return letterWithMaxFrq;
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

char findLetterWithMaxFrequencyFromUnusedAsReplacement(LETTER* letter)
{
	int index = 0;
	while (index < ALPHABET_SIZE)
	{
		if (isUsedAsReplacement(LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index], letter)) index++;
		else break;
	}
	if (index == ALPHABET_SIZE) return LETTER_IS_NOT_FOUND;
    else return LETTERS_IN_ORDER_BY_FREQUENCY_DESC[index];
}

void printLettersFrequencies(LETTER* letter)
{
	printf("Частоты встреч букв во входной криптограмме:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.4f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(LETTER* letter)
{
	LETTER* srcLetter = findLetterWithMaxFrequencyFromUndesiphered(letter);
	//TODO: добавить проверку, не вернулся ли ноль в srcLetter
	char letterForReplacement = findLetterWithMaxFrequencyFromUnusedAsReplacement(letter);
	printf("Исходя из частотного анализа сделан вывод, что букву %c, возможно, следует поменять на %c.\n", 
		   srcLetter->symbol, letterForReplacement);
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

void calculateNumOfUndesipheredLetters(CRYPTOGRAM* data)
{
	WORD_LIST_ITEM* word = data->words->firstWord;
	while (word != NULL)
	{
		word->numOfUndecipheredLetters = 0;
		char* letter = word->chars;
		while (*letter) if (isCapitalLetter(*letter)) word->numOfUndecipheredLetters++;
		word->nextWord;
	}
}

void insertByLen(WORD_LIST_ITEM** newFirstWord, WORD_LIST_ITEM** itemToInsert)
{
	if (*newFirstWord == NULL || (*itemToInsert)->numOfUndecipheredLetters < (*newFirstWord)->numOfUndecipheredLetters)
	{
		(*itemToInsert)->nextWord = *newFirstWord;
		*newFirstWord = *itemToInsert;
	}
	else
	{
		WORD_LIST_ITEM** currentToCompareWith = *newFirstWord;
		while (currentToCompareWith != NULL && 
			   !((*itemToInsert)->numOfUndecipheredLetters < (*currentToCompareWith)->numOfUndecipheredLetters)) //если нарушится - пихаем в следующий
		{
			*currentToCompareWith = (*currentToCompareWith)->nextWord;
		}
		(*itemToInsert)->nextWord = (*currentToCompareWith)->nextWord;
		(*currentToCompareWith)->nextWord = *itemToInsert;
	}
}

void insertByUndeciphered(WORD_LIST_ITEM** newFirstWord, WORD_LIST_ITEM* item)
{
	if (*newFirstWord == NULL || item->len < (*newFirstWord)->len)
	{
		item->nextWord = *newFirstWord;
		*newFirstWord = item;
	}
	else
	{
		WORD_LIST_ITEM* current = *newFirstWord;
		while (current->nextWord != NULL && !(item->len < current->nextWord->len))
		{
			current = current->nextWord;
		}
		item->nextWord = current->nextWord;
		current->nextWord = item;
	}
}

WORD_LIST_ITEM* sortWordsByLen(WORD_LIST_ITEM* firstWord)
{
	WORD_LIST_ITEM* newfirstWord = NULL;

	while (firstWord != NULL)
	{
		WORD_LIST_ITEM* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL || item->len < newfirstWord->len)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			WORD_LIST_ITEM* current = newfirstWord;
			while (current->nextWord != NULL && !(item->len < current->nextWord->len))
			{
				current = current->nextWord;
			}

			item->nextWord = current->nextWord;
			current->nextWord = item;
		}
	}
	return newfirstWord;
}

WORD_LIST_ITEM* sortWordsByUndeciphered(WORD_LIST_ITEM* firstWord)
{
	WORD_LIST_ITEM* newfirstWord = NULL;

	while (firstWord != NULL)
	{
		WORD_LIST_ITEM* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL || item->numOfUndecipheredLetters < newfirstWord->numOfUndecipheredLetters)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			WORD_LIST_ITEM* current = newfirstWord;
			while (current->nextWord != NULL && !(item->numOfUndecipheredLetters < current->nextWord->numOfUndecipheredLetters))
			{
				current = current->nextWord;
			}

			item->nextWord = current->nextWord;
			current->nextWord = item;
		}
	}
	return newfirstWord;
}

void printWords(WORD_LIST_ITEM* word, PRINTING_OPERATION_CODE order)
{
	system("cls");

	int prevValue = 0;
	if (order == BY_LENGTH) prevValue = word->len;
	if (order == BY_UNDECIPHERED) prevValue = word->numOfUndecipheredLetters;
	printf("По %i:\n", prevValue);
	printf("%s\n", word->chars);

	word = word->nextWord;
	while (word != NULL)
	{
		int nextValue = 0;
		if (order == BY_LENGTH) nextValue = word->len;
		if (order == BY_UNDECIPHERED) nextValue = word->numOfUndecipheredLetters;

		if (nextValue != prevValue) printf("\nПо %i:\n", nextValue);
		printf("%s\n", word->chars);

		word = word->nextWord;
		prevValue = nextValue;
	}

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleWordsPrintingMenu(CRYPTOGRAM* data)
{
	PRINTING_OPERATION_CODE operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Вывести слова, сгрупированные по длине\n");
		printf("2. Вывести слова, сгрупированные по количеству букв без замен\n");
		printf("3. Вернуться в главное меню\n");
		printf("Введите код нужной команды (любой код, кроме перечисленных, будет проигнорирован): ");
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case BY_LENGTH: printWords(sortWordsByLen(data->words->firstWord), BY_LENGTH); break; //длина слов фиксирована => поссчитали на инициализации
		case BY_UNDECIPHERED: calculateNumOfUndesipheredLetters(data);
			                  printWords(sortWordsByUndeciphered(data->words->firstWord), BY_UNDECIPHERED); 
							  break;
		default: break;
		}
	} while (operationCode != DECLINE_PRINTING);
}


void printCryptogram(CRYPTOGRAM* data)
{
	system("cls");
	printEncryptionKey(data->letter);
	printText(data);

	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

char getCyrrilicLetter()
{
	char letter = '\n'; //для исправления проблемы с энтером во входном потоке
	while (letter == '\n') scanf("%c", &letter);
	letter = fixCodeForCyrillicCharFromInput(letter);
	while (!isLetter(letter))
	{
		printf("Недопустимый символ. Введите букву русского алфавита: ");
		scanf("%c", &letter);
		while (letter == '\n') scanf("%c", &letter);
		letter = fixCodeForCyrillicCharFromInput(letter);
	}
	return letter;
}

CHANGES_LIST_ITEM* addNewElementToHistory(char srcLetter, char letterForReplacement, CRYPTOGRAM* data)
{
	CHANGES_LIST_ITEM* newItem = (CHANGES_LIST_ITEM*)malloc(2 * sizeof(CHANGES_LIST_ITEM*) + 2 * sizeof(char));
	newItem->prev = data->curChange;
	newItem->next = NULL;
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;
	data->curChange = newItem;
}

void replaceLetter(char srcLetter, char letterForReplacement, CRYPTOGRAM* data)
{
	(data->letter + srcLetter - 'А')->replacedTo = letterForReplacement;
}

void handleReplacementMenu(CRYPTOGRAM* data)
{
	system("cls");
	printEncryptionKey(data->letter);

	printf("Введите букву, замену которой необходимо провести: ");
	char srcLetter = getCyrrilicLetter();
	if (isLowercaseLetter(srcLetter)) srcLetter -= ALPHABET_SIZE; //сдвиг с маленьких букв на большие
	
	printf("Введите букву, на которую её необходимо заменить: ");
	char letterForReplacement = getCyrrilicLetter();
	if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	while (isUsedAsReplacement(letterForReplacement, data->letter))
	{
		printf("Эта буква уже использована в качестве замены. Введите другую букву русского алфавита: ");
		letterForReplacement = getCyrrilicLetter();
		if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	} 
	replaceLetter(srcLetter, letterForReplacement, data);
	addNewElementToHistory(srcLetter, letterForReplacement, data);

	system("cls");
	printf("Замена проведена успешно.\n");
	printEncryptionKey(data->letter);
	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
	//TODO: несколько замен без выхода в главное меню
}

void undoCurChange(CRYPTOGRAM* data)
{
	(data->letter + data->curChange->originalLetter - 'А')->replacedTo = NO_REPLACEMENT;
	CHANGES_LIST_ITEM* oldCurChange = data->curChange;
	data->curChange = data->curChange->prev;
	data->curChange->next = oldCurChange;
}

void redoCurChange(CRYPTOGRAM* data)
{
	(data->letter + data->curChange->originalLetter - 'А')->replacedTo = data->curChange->next->replacedTo;
	CHANGES_LIST_ITEM* oldCurChange = data->curChange;
	data->curChange = data->curChange->next;
	data->curChange->prev = oldCurChange;
}

void handleRevertMenu(CRYPTOGRAM* data)
{
	HISTORY_OPERATION_CODE operationCode = NULL_OPERATION;
	{
		system("cls");

		printf("1. Выход в главное меню\n");
		if (data->curChange->prev == NULL && data->curChange->next == NULL)
		{
			printf("История замен пуста.\n");
		}
		else
		{
			if (data->curChange->prev != NULL) printf("2. Отменить последнее действие: замену %c на %c\n",
				data->curChange->originalLetter,
				data->curChange->replacedTo);
			if (data->curChange->next != NULL) printf("3. Восстановить последнее отменённое действие: замену %c на %c\n",
				data->curChange->next->originalLetter,
				data->curChange->next->replacedTo);
		}

		printf("Введите код нужной команды (любой код, кроме перечисленных, будет проигнорирован): ");
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case UNDO: if (data->curChange->prev != NULL) undoCurChange(data); break;
		case REDO: if (data->curChange->next != NULL) redoCurChange(data); break;
		default: break;
		}
	} while (operationCode != DECLINE_REVERT);
}

void replaceLettersAndUpdateHistoryAutomatically(CRYPTOGRAM* data)
{
	float prevUndesipheredLetterFrq = INITIAL_FRQ;
	float curUndesipheredLetterFrq = INITIAL_FRQ;
	LETTER* srcLetter = LETTER_IS_NOT_FOUND;
	do
	{
		prevUndesipheredLetterFrq = curUndesipheredLetterFrq;
		srcLetter = findLetterWithMaxFrequencyFromUndesiphered(data->letter);
		if (srcLetter != LETTER_IS_NOT_FOUND)
		{
			curUndesipheredLetterFrq = srcLetter->frequencyInSrcText;
			if (curUndesipheredLetterFrq != prevUndesipheredLetterFrq)
			{
				//здесь проверка данных не нужна, т.к. кол-во символов в ключе шифрования одинаковое кол-во
				char letterForReplacement = findLetterWithMaxFrequencyFromUnusedAsReplacement(data->letter);
				replaceLetter(srcLetter->symbol, letterForReplacement, data);
				addNewElementToHistory(srcLetter->symbol, letterForReplacement, data);
				printf("Символ %c был заменён на %c.\n", srcLetter->symbol, letterForReplacement);
			}
			else break;
		}
		else break;
	} while (srcLetter != LETTER_IS_NOT_FOUND);
}

void handleAutoreplacement(CRYPTOGRAM* data)
{
	system("cls");
	replaceLettersAndUpdateHistoryAutomatically(data);
	printf(RETURN_TO_MENU_MESSAGE);
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
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
		case PRINT_WORDS: handleWordsPrintingMenu(data); break;
		case PRINT_CRYPTOGRAM: printCryptogram(data); break;
		case REPLACE_LETTERS: handleReplacementMenu(data); break;
		case REVERT: handleRevertMenu(data); break;
		case AUTOREPLACEMENT: handleAutoreplacement(data); break;
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
		printf("Полученные данные не подходят для расшифровки: отсутствуют буквы.\n");
		_getch();
	}
	else handleMainCycle(data);
	//TODO: добавить функцию, подчищающую память
	free(data);
	return 0;
}