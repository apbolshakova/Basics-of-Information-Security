#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>

#define SIZE_OF_STRING_TO_COPY 1000
#define ALPHABET_SIZE 32
#define NULL_OPERATION 0
#define NO_REPLACEMENT '\0'
#define NO_LETTERS '\0'
#define LETTER_IS_NOT_FOUND 0
#define INITIAL_FRQ 0.0
#define RETURN_TO_MENU_BTN_CODE 32
#define DATA_PATH "input.txt"
#define LETTERS_IN_ORDER_BY_FREQUENCY_DESC "оеаинтсрвлкмдпуяыьгзбчйчжшюцщэфъ"

typedef enum OperationCode_
{
	PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT = '1',
	PRINT_WORDS,
	PRINT_CRYPTOGRAM,     
	REPLACE_LETTERS,
	REVERT,   
	AUTOREPLACEMENT, 
	EXIT 
} operation_code_t;

typedef enum Bool_
{ 
	FALSE, 
	TRUE 
} bool_t;

typedef enum PrintingOperationCode_
{
	BY_LENGTH = '1',
	BY_UNDECIPHERED,
	DECLINE_PRINTING
} printing_operation_code_t;

typedef enum HistoryOperationCode_
{
	DECLINE_REVERT = '1',
	UNDO,
	REDO
} history_operation_code_t;

typedef struct Letter_
{
	char symbol;
	int encounteredInSrcText;
	float frequencyInSrcText;
	char replacedTo;
} letter_t;

typedef struct ChangesListItem_
{
	struct ChangesListItem_* root;
	struct ChangesListItem_* prev;
	struct ChangesListItem_* next;
	char originalLetter;
	char replacedTo;
} changes_list_item_t;

typedef struct WordListItem_
{
	char* chars;
	int len;
	int numOfUndecipheredLetters;
	struct WordListItem_* nextWord;
} word_list_item_t;

typedef struct WordsInfo_
{
	word_list_item_t* firstWord;
	word_list_item_t* lastWord;
} words_info_t;

typedef struct Cryptogram_
{
	char* text;
	int numOfLetters;
	letter_t* letter;
	changes_list_item_t* curChange;
	words_info_t* words;
} cryptogram_t;

void initLetters(letter_t* letter)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(letter + i)->symbol = 'А' + i;
		(letter + i)->encounteredInSrcText = 0;
		(letter + i)->frequencyInSrcText = 0;
		(letter + i)->replacedTo = NO_REPLACEMENT;
	}
}

static changes_list_item_t* initialItem = NULL;
changes_list_item_t* initChangesList()
{
	initialItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	initialItem->prev = NULL;
	initialItem->next = NULL;
	initialItem->originalLetter = NO_REPLACEMENT;
	initialItem->replacedTo = NO_REPLACEMENT;
	return initialItem;
}

bool_t isCapitalLetter(char item)
{
	if ('А' <= item && item <= 'Я') return TRUE;
	else return FALSE;
}

bool_t isLowercaseLetter(char item)
{
	if ('а' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

bool_t isLetter(char item)
{
	if ('А' <= item && item <= 'я') return TRUE;
	else return FALSE;
}

//исправление считанных кодов символов, сдвиги найдены экспериментально
char fixCodeForCyrillicCharFromInput(char item) 
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

void copyStringAndCalculateLettersEncounters(cryptogram_t* data, char* str)
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

void handleDataFromNewString(cryptogram_t* data, char* str)
{
	char* sav = data->text;
	int sizeOfOldText = moveTextPtrToTheEndAndGetSizeOfText(data->text);

	data->text = sav;
	data->text = (char*) realloc(data->text, 
		                         (sizeOfOldText + SIZE_OF_STRING_TO_COPY) * sizeof(char) + 1);
	sav = data->text;

	data->text += sizeOfOldText;
	copyStringAndCalculateLettersEncounters(data, str);
	data->text = sav;
}

void initTextAndCalculateEncounters(cryptogram_t* data, FILE* f)
{
	fclose(f);
	f = fopen(DATA_PATH, "r");
	char* temp = (char*) calloc(SIZE_OF_STRING_TO_COPY, sizeof(char));
	while (fgets(temp, SIZE_OF_STRING_TO_COPY * sizeof(char), f) != NULL) 
		handleDataFromNewString(data, temp);
	free(temp);
}

void calculateFrequencies(cryptogram_t* data)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		(data->letter + i)->frequencyInSrcText = 
			(float) (data->letter + i)->encounteredInSrcText / data->numOfLetters;
	}
}

bool_t areSameWords(char* a, char* b)
{
	bool_t areSame = TRUE;
	while (*a && *b)
	{
		if (*a != *b)
		{
			areSame = FALSE;
			break;
		}
		++a;
		++b;
	}
	if (*a || *b) areSame = FALSE; //должны были закончиться одновременно
	return areSame;
}

bool_t wordIsUnique(word_list_item_t* newWord, words_info_t* wordsInfo)
{
	word_list_item_t* item = wordsInfo->firstWord;
	bool_t isUnique = TRUE;
	while (item != NULL && isUnique)
	{
		if (item->len == newWord->len && 
			areSameWords(item->chars, newWord->chars)) isUnique = FALSE;
		item = item->nextWord;
	}
	return isUnique;
}

void addNewWordToList(word_list_item_t* newWord, words_info_t* wordsInfo)
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

char* handleWordAndMovePtrToTheEndOfIt(words_info_t* wordsInfo, char* text) //TODO: рефакторинг
{
	char* textSav = text;
	word_list_item_t* newWord = (word_list_item_t*)malloc(sizeof(word_list_item_t));
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

words_info_t* parseTextIntoWords(char* text)
{
	char* sav = text;
	words_info_t* wordsInfo = (words_info_t*)malloc(sizeof(words_info_t));
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

cryptogram_t* initCryptogram()
{
	cryptogram_t* data = (cryptogram_t*) malloc(sizeof(cryptogram_t));
	data->text = malloc(sizeof(char));
	*(data->text) = '\0';
	data->letter = (letter_t*) calloc(ALPHABET_SIZE, sizeof(letter_t));
	data->numOfLetters = 0;
	data->curChange = initChangesList();

	FILE *f = fopen(DATA_PATH, "r");
	if ((f != NULL) && (fgetc(f) != EOF) && !(feof(f)))
	{
		initLetters(data->letter);
		initTextAndCalculateEncounters(data, f);
	}
	fclose(f);
	if (data->numOfLetters != NO_LETTERS)
	{
		calculateFrequencies(data);
		data->words = parseTextIntoWords(data->text);
	}
	return data;
}

void printMainMenu()
{
	printf("Вас приветствует программа, реализующая функции инструмента криптоаналитика.");
	printf("Возможные действия: \n");
	printf("1. Предложить замену на основе частотного анализа\n");
	printf("2. Вывести слова из криптограммы\n");
	printf("3. Отобразить криптограмму\n");
	printf("4. Заменить буквы\n");
	printf("5. Отменить или вернуть последнее изменение\n");
	printf("6. Произвести автоматическую замену\n");
	printf("7. Выход\n");
	printf("Введите код нужной команды " );
	printf("(любой код, кроме перечисленных, будет проигнорирован): ");
}

void printText(cryptogram_t* data)
{
	printf("Текст криптограммы с выполненными заменами:\n\n");
	char* ptr = data->text;
	while (*ptr)
	{
		if (isLetter(*ptr))
		{
			int curSymbolIndex = *(ptr)-'А';
			if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) 
				printf("%c", *ptr);
			else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		}
		else printf("%c", *ptr);
		ptr++;
	}
	printf("\n\n");
}

void printEncryptionKey(letter_t* letter)
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

letter_t* findLetterWithMaxFrequencyFromUndesiphered(letter_t* letter)
{
	letter_t* letterWithMaxFrq = LETTER_IS_NOT_FOUND;
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

bool_t isUsedAsReplacement(char symbolToCheck, letter_t* letter)
{
	bool_t isUsed = FALSE;
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

char findLetterWithMaxFrequencyFromUnusedAsReplacement(letter_t* letter)
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

void printLettersFrequencies(letter_t* letter)
{
	printf("Частоты встреч букв во входной криптограмме:\n");
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		printf("%c -> %0.4f\n", (letter + i)->symbol, (letter + i)->frequencyInSrcText);
	}
	printf("\n");
}

void printReplacementSuggestion(letter_t* letter)
{
	letter_t* srcLetter = findLetterWithMaxFrequencyFromUndesiphered(letter);
	if (srcLetter == LETTER_IS_NOT_FOUND) 
		printf("Невозможно определить оптимальную замену.\n");
	else
	{
		char letterForReplacement = findLetterWithMaxFrequencyFromUnusedAsReplacement(letter);
		printf("Исходя из частотного анализа сделан вывод, ");
		printf("что букву %c, возможно, следует поменять на %c.\n", 
			srcLetter->symbol, letterForReplacement);
	}
}

void analyseFrequencyAndSuggestReplacement(cryptogram_t* data)
{
	system("cls");

	printLettersFrequencies(data->letter);
	printReplacementSuggestion(data->letter);

	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void calculateNumOfUndesipheredLetters(cryptogram_t* data)
{
	word_list_item_t* word = data->words->firstWord; 
	while (word != NULL)
	{
		word->numOfUndecipheredLetters = 0;
		char* curLetter = word->chars;
		while (*curLetter)
		{
			if ((data->letter + *curLetter - 'А')->replacedTo == NO_REPLACEMENT) 
				word->numOfUndecipheredLetters++;
			curLetter++;
		}
		word = word->nextWord;
	}
}

void insertByLen(word_list_item_t** newFirstWord, word_list_item_t** itemToInsert)
{
	if (*newFirstWord == NULL || 
		(*itemToInsert)->numOfUndecipheredLetters < (*newFirstWord)->numOfUndecipheredLetters)
	{
		(*itemToInsert)->nextWord = *newFirstWord;
		*newFirstWord = *itemToInsert;
	}
	else
	{
		word_list_item_t** currentToCompareWith = *newFirstWord;
		while (currentToCompareWith != NULL && 
			      !((*itemToInsert)->numOfUndecipheredLetters < 
			      (*currentToCompareWith)->numOfUndecipheredLetters))
		{
			*currentToCompareWith = (*currentToCompareWith)->nextWord;
		}
		(*itemToInsert)->nextWord = (*currentToCompareWith)->nextWord;
		(*currentToCompareWith)->nextWord = *itemToInsert;
	}
}

void insertByUndeciphered(word_list_item_t** newFirstWord, word_list_item_t* item)
{
	if (*newFirstWord == NULL || item->len < (*newFirstWord)->len)
	{
		item->nextWord = *newFirstWord;
		*newFirstWord = item;
	}
	else
	{
		word_list_item_t* current = *newFirstWord;
		while (current->nextWord != NULL && !(item->len < current->nextWord->len))
		{
			current = current->nextWord;
		}
		item->nextWord = current->nextWord;
		current->nextWord = item;
	}
}

word_list_item_t* sortWordsByLen(word_list_item_t* firstWord)
{
	word_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		word_list_item_t* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL || item->len < newfirstWord->len)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			word_list_item_t* current = newfirstWord;
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

word_list_item_t* sortWordsByUndeciphered(word_list_item_t* firstWord) //TODO: переписать в одну функцию с ByLen
{
	word_list_item_t* newfirstWord = NULL;
	while (firstWord != NULL)
	{
		word_list_item_t* item = firstWord;
		firstWord = firstWord->nextWord;

		if (newfirstWord == NULL || 
			item->numOfUndecipheredLetters < newfirstWord->numOfUndecipheredLetters)
		{
			item->nextWord = newfirstWord;
			newfirstWord = item;
		}
		else
		{
			word_list_item_t* current = newfirstWord;
			while (current->nextWord != NULL && 
				item->numOfUndecipheredLetters >= current->nextWord->numOfUndecipheredLetters)
			{
				current = current->nextWord;
			}
			item->nextWord = current->nextWord;
			current->nextWord = item;
		}
	}
	return newfirstWord;
}

void printCharsWithEncryption(char* ptr, cryptogram_t* data)
{
	while (*ptr)
	{
		int curSymbolIndex = *(ptr)-'А';
		if ((data->letter + curSymbolIndex)->replacedTo == NO_REPLACEMENT) printf("%c", *ptr);
		else printf("%c", (data->letter + curSymbolIndex)->replacedTo);
		ptr++;
	}
	printf("\n");
}

void printWords(printing_operation_code_t order, cryptogram_t* data)
{
	system("cls");

	word_list_item_t* word = data->words->firstWord;
	int prevValue = 0;
	if (order == BY_LENGTH) prevValue = word->len;
	if (order == BY_UNDECIPHERED) prevValue = word->numOfUndecipheredLetters;
	printf("По %i:\n", prevValue);
	printCharsWithEncryption(word->chars, data);

	word = word->nextWord;
	while (word != NULL)
	{
		int nextValue = 0;
		if (order == BY_LENGTH) nextValue = word->len;
		if (order == BY_UNDECIPHERED) nextValue = word->numOfUndecipheredLetters;

		if (nextValue != prevValue) printf("\nПо %i:\n", nextValue);
		printCharsWithEncryption(word->chars, data);

		word = word->nextWord;
		prevValue = nextValue;
	}

	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleWordsPrintingMenu(cryptogram_t* data)
{
	printing_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Вывести слова, сгрупированные по длине\n"); //TODO: вынести в функцию
		printf("2. Вывести слова, сгрупированные по количеству букв без замен\n");
		printf("3. Вернуться в главное меню\n");
		printf("Введите код нужной команды ");
		printf("(любой код, кроме перечисленных, будет проигнорирован) : ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case BY_LENGTH: data->words->firstWord = sortWordsByLen(data->words->firstWord);
			printWords(BY_LENGTH, data); break; //длины подсчитаны при инициализации
		case BY_UNDECIPHERED: calculateNumOfUndesipheredLetters(data);
			data->words->firstWord = sortWordsByUndeciphered(data->words->firstWord);
			printWords(BY_UNDECIPHERED, data);
			break;
		default: break;
		}
	} while (operationCode != DECLINE_PRINTING);
}

void printCryptogram(cryptogram_t* data)
{
	system("cls");
	printEncryptionKey(data->letter);
	printText(data);

	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

char getCyrrilicLetterOrExitSymbol()
{
	char letter = '\n'; //для исправления проблемы со считанным ENTER во входном потоке
	while (letter == '\n') scanf("%c", &letter);
	letter = fixCodeForCyrillicCharFromInput(letter);
	while (letter != RETURN_TO_MENU_BTN_CODE && !isLetter(letter))
	{
		printf("Недопустимый символ. Введите букву русского алфавита или пробел для выхода в главное меню: ");
		scanf("%c", &letter);
		while (letter == '\n') scanf("%c", &letter);
		letter = fixCodeForCyrillicCharFromInput(letter);
	}
	return letter;
}

void addNewElementToHistory(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	changes_list_item_t* newItem = (changes_list_item_t*)malloc(sizeof(changes_list_item_t));
	newItem->prev = data->curChange;
	newItem->next = NULL;
	newItem->root = data->curChange->root;
	newItem->originalLetter = srcLetter;
	newItem->replacedTo = letterForReplacement;
	data->curChange = newItem;
}

void replaceLetter(char srcLetter, char letterForReplacement, cryptogram_t* data)
{
	(data->letter + srcLetter - 'А')->replacedTo = letterForReplacement;
}

void handleReplacementMenu(cryptogram_t* data)
{
	system("cls");
	printEncryptionKey(data->letter);

	printf("Введите букву, замену которой необходимо провести или пробел для выхода в главное меню: ");
	char srcLetter = getCyrrilicLetterOrExitSymbol();
	if (srcLetter == RETURN_TO_MENU_BTN_CODE) return;
	if (isLowercaseLetter(srcLetter)) srcLetter -= ALPHABET_SIZE; //сдвиг на др. регистр
	
	printf("Введите букву, на которую её необходимо заменить или пробел для выхода в главное меню: ");
	char letterForReplacement = getCyrrilicLetterOrExitSymbol();
	if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
	if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	while (isUsedAsReplacement(letterForReplacement, data->letter))
	{
		printf("Эта буква уже использована в качестве замены. ");
		printf("Введите другую букву русского алфавита или введите пробел для выхода в главное меню: ");
		letterForReplacement = getCyrrilicLetterOrExitSymbol();
		if (letterForReplacement == RETURN_TO_MENU_BTN_CODE) return;
		if (isCapitalLetter(letterForReplacement)) letterForReplacement += ALPHABET_SIZE;
	} 
	replaceLetter(srcLetter, letterForReplacement, data);
	addNewElementToHistory(srcLetter, letterForReplacement, data);

	system("cls");
	printf("Замена проведена успешно.\n");
	printEncryptionKey(data->letter);
	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void undoCurChange(cryptogram_t* data)
{
	(data->letter + data->curChange->originalLetter - 'А')->replacedTo = NO_REPLACEMENT;
	changes_list_item_t* oldCurChange = data->curChange;
	data->curChange = data->curChange->prev;
	data->curChange->next = oldCurChange;
}

void redoCurChange(cryptogram_t* data)
{	
	changes_list_item_t* oldCurChange = data->curChange;
	data->curChange = data->curChange->next;
	data->curChange->prev = oldCurChange;
	(data->letter + data->curChange->originalLetter - 'А')->replacedTo = 
		data->curChange->replacedTo;
}

void handleRevertMenu(cryptogram_t* data)
{
	history_operation_code_t operationCode = NULL_OPERATION;
	do
	{
		system("cls");
		printf("1. Выход в главное меню\n");
		if (data->curChange->prev == NULL && data->curChange->next == NULL)
		{
			printf("История замен пуста.\n");
		}
		else
		{
			if (data->curChange->prev != NULL)
			{
				printf("2. Отменить последнее действие: ");
				printf("замену %c на %c\n", 
					   data->curChange->originalLetter, data->curChange->replacedTo);
			}
			if (data->curChange->next != NULL)
			{
				printf("3. Восстановить последнее отменённое действие: ");
				printf("замену %c на %c\n",
					   data->curChange->next->originalLetter,
					   data->curChange->next->replacedTo);
			}
		}

		printf("Введите код нужной команды ");
		printf("(любой код, кроме перечисленных, будет проигнорирован): ");
		scanf("\n%c", &operationCode);
		switch (operationCode)
		{
		case UNDO: if (data->curChange->prev != NULL) undoCurChange(data); break;
		case REDO: if (data->curChange->next != NULL) redoCurChange(data); break;
		default: break;
		}
	} while (operationCode != DECLINE_REVERT);
}

void deleteCurChange(cryptogram_t* data)
{
	undoCurChange(data);
	data->curChange->next = NULL;
	free(data->curChange->next);
}

void replaceLettersAndUpdateHistoryAutomatically(cryptogram_t* data) //TODO рефакторинг
{
	letter_t* srcLetter = findLetterWithMaxFrequencyFromUndesiphered(data->letter);
	if (srcLetter == LETTER_IS_NOT_FOUND)
	{
		printf("Невозможно определить оптимальную замену.\n");
		return;
	}
	while (srcLetter != LETTER_IS_NOT_FOUND)
	{
		char letterForReplacement =
			findLetterWithMaxFrequencyFromUnusedAsReplacement(data->letter);
		replaceLetter(srcLetter->symbol, letterForReplacement, data);
		addNewElementToHistory(srcLetter->symbol, letterForReplacement, data);

		letter_t* nextSrcLetter = findLetterWithMaxFrequencyFromUndesiphered(data->letter);
		if (nextSrcLetter == LETTER_IS_NOT_FOUND ||
			srcLetter->frequencyInSrcText != nextSrcLetter->frequencyInSrcText)
		{
			printf("Символ %c был заменён на %c.\n", srcLetter->symbol, letterForReplacement);
			srcLetter = nextSrcLetter;
		}
		else
		{
			deleteCurChange(data);
			printf("Невозможно определить следующую оптимальную замену.\n");
			break;
		}
	}
}

void handleAutoreplacement(cryptogram_t* data)
{
	system("cls");
	replaceLettersAndUpdateHistoryAutomatically(data);
	printf("Нажмите пробел, чтобы вернуться в главное меню.\n");
	while (_getch() != RETURN_TO_MENU_BTN_CODE);
}

void handleMainCycle(cryptogram_t* data)
{
	operation_code_t operationCode = NULL_OPERATION;
	do 
	{
		system("cls");
		printMainMenu();
		scanf("%c", &operationCode);
		switch (operationCode)
		{
		case PRINT_ANALYSIS_RESULT_AND_SUGGEST_REPLACEMENT: 
			analyseFrequencyAndSuggestReplacement(data); break;
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

	cryptogram_t* data = initCryptogram();
	if (data->numOfLetters == NO_LETTERS)
	{
		printf("Полученные данные не подходят для расшифровки: отсутствуют буквы.\n");
		_getch();
	}
	else handleMainCycle(data);

	/*CHANGES_LIST_ITEM* changesItem = data->curChange;
	if (changesItem->prev == NULL && changesItem->next == NULL)
	{
		data->curChange = NULL;
		free(data->curChange);
	}
	else
	{
		while (changesItem != NULL)
		{
			changesItem = data->curChange->root->next;
			if (changesItem != NULL) changesItem->prev = NULL;
			free(data->curChange->root);
		}
	}*/

	/* TODO: заставить работать
	WORD_LIST_ITEM* wordsItem = data->words->firstWord;
	while (wordsItem != NULL)
	{
		wordsItem = data->words->firstWord->nextWord;

		data->words->firstWord->chars = NULL;
		free(data->words->firstWord->chars);

		data->words->firstWord = NULL;
		free(data->words->firstWord);
	}*/
	
	data->words = NULL;
	free(data->words);

	data->text = NULL;
	free(data->text);

	data->curChange = NULL;
	free(data->curChange);

	data->letter = NULL;
	free(data->letter);

	data = NULL;
	free(data);
	return 0;
}