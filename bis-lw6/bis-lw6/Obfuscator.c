#include "Common.h"

status_t handleObfuscation(FILE* fSrc, FILE* fRes, int* config)
{
	char* src = getSrcCode(fSrc);
	if (!src)
	{
		printf("ERROR: unable to get source code.\n");
		return FAIL;
	}
	if (config[CHANGE_VAR_NAMES]) changeVarNames(src);
	fprintText(fRes, src, config);
	return SUCCESS;
}

char* getSrcCode(FILE* fSrc)
{
	char ch = '\0';
	char* res = (char*)calloc(1, sizeof(char));
	char* temp = (char*)calloc(LEN + 1, sizeof(char));
	if (!temp)
	{
		printf("ERROR: memory allocation problem.\n");
		return NULL;
	}
	while (fscanf_s(fSrc, LEN_CODE, temp, LEN) != EOF)
	{
		char* ptr = (char*)realloc(res, (strlen(res) + LEN + 1) * sizeof(char));
		res = ptr;
		if (!res)
		{
			printf("ERROR: memory allocation problem.\n");
			return NULL;
		}
		strcat(res, temp);
		char ch[2] = { 0 };
		fscanf_s(fSrc, "%c", &ch);
		strcat(res, &ch);
	}
	return res;
}

void fprintText(FILE* fRes, char* text, int* config)
{
	if (text == NULL) return;
	mode_t mode = CODE;
	while (*text)
	{
		if (config[DEL_COMMS])
		{
			if (!strncmp(text, "//", 2)) mode = INLINE_COMM;
			if (*text == '\n' && mode == INLINE_COMM) mode = CODE;
			if (!strncmp(text, "/*", 2)) mode = MULTILINE_COMM;
			if (!strncmp(text, "*/", 2))
			{
				text += 2;
				mode = CODE;
			}
		}
		if (config[DEL_SPACES] && 
			((*text == '\n' && *(text + 1) != '#') && *(text - 1) != '>') && 
			*(text - 1) != '"') text++;
		if (mode != INLINE_COMM && mode != MULTILINE_COMM)
		{
			if (config[ADD_DUMMIES] && !strncmp(text, "return", 6)) fprintf(fRes, DUMMY);
			fprintf(fRes, "%c", *text);
		}
		text++;
	}
	printf(fRes, "\n");
}

void changeVarNames(char* src)
{
	char** types[TYPES_NUM] = { "char", "int", "long", "float", "double" };
	for (int i = 0; i < TYPES_NUM; i++)
	{
		char* lastPos = src;
		while (lastPos = strstr(lastPos, types[i]))
		{
			if (lastPos != src && *(lastPos - 1) != ' ' && *(lastPos - 1) != '\n') //this is a part of word
			{
				lastPos++;
				continue;
			}
			lastPos += strlen(types[i]);
			while (*lastPos == '*' || *lastPos == ' ') lastPos++;
			char* sav = lastPos;
			while (!strchr(FUNC_NAME_MASK, *lastPos)) lastPos++;
			if (*lastPos == '(') continue; //this is a function declaration

			lastPos = sav;
			int len = 0;
			while (!strchr(VAR_NAME_MASK, *lastPos))
			{
				lastPos++;
				len++;
			}
			char* oldVarName = (char*)calloc(len + 1, sizeof(char));
			if (!oldVarName) return;
			strncpy(oldVarName, sav, len);
			
			lastPos = sav;
			while (lastPos = strstr(lastPos, oldVarName))
			{
				for (int i = 0; i < len; i++)
				{
					(*lastPos)++;
					lastPos++;
				}
			}
			lastPos = sav;
		}
	}
}