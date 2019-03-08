#ifndef TYPES
#define TYPES

typedef enum OperationCode_
{
	SUGGEST_REPLACEMENT = '1',
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
	struct ChangesListItem_* head;
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

#endif