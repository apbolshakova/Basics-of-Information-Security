#include "Common.h"

int main(void)
{
	FILE* fSrc;
	FILE* fRes;
	char* srcFileName = getSrcFileName();
	char* resFileName = getResFileName();
	if (!srcFileName || !resFileName)
	{
		printf("ERROR: unable to get file name.\n");
		return FAIL;
	}
	if (!(fSrc = fopen(srcFileName, "r")))
	{
		printf("ERROR: unable to open file.\n");
		return FAIL;
	}
	if (!(fRes = fopen(resFileName, "w")))
	{
		printf("ERROR: unable to open file.\n");
		return FAIL;
	}
	int* config = getConfigs();
	if (!config || handleObfuscation(fSrc, fRes, config) == FAIL)
	{
		fclose(fSrc);
		fclose(fRes);
		printf("ERROR: obfuscating wasn't complete.\n");
		return 0;
	}
	fclose(fSrc);
	fclose(fRes); 
	return 0;
}

char* getSrcFileName()
{
	char* fileName = (char*)calloc(LEN, sizeof(char));
	if (!fileName)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	printf("Enter name of file with source code (up to 255 characters):\n");
	scanf_s(LEN_CODE, fileName, LEN);
	return fileName;
}

char* getResFileName()
{
	char* fileName = (char*)calloc(LEN, sizeof(char));
	if (!fileName)
	{
		printf("ERROR: memory allocation problem.\n");
		return FAIL;
	}
	printf("Enter name of file for result saving (up to 255 characters):\n");
	scanf_s(LEN_CODE, fileName, LEN);
	return fileName;
}

int* getConfigs()
{
	int* configs = (int*)calloc(4, sizeof(int));
	if (!configs) return NULL;
	FILE* f = fopen(CONF, "r");
	if (!f)
	{
		free(configs);
		return NULL;
	}
	for (int i = 0; i < CONFIGS_NUM; i++)
		fscanf(f, "%i", &(configs[i]));
	return configs;
}