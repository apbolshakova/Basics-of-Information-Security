#include "Header.h"

void encode()
{
	//открыть файл с сообщением
	FILE* srcFile = fopen(FILE_NAME, "rb");
	if (srcFile == NULL)
	{
		printf("ERROR: unable to open file.\n");
		return;
	}

	//Создать временный файл
	FILE* tempFile = fopen(TEMP_FILE_NAME, "wb");
	if (tempFile == NULL)
	{
		printf("ERROR: unable to create temp file.\n");
		return;
	}

	int blockSize = 0;
	printf("Enter size of information blocks to process: ");
	scanf("%i", &blockSize);
	while (blockSize < MIN_BLOCK_SIZE || blockSize > MAX_BLOCK_SIZE || blockSize % VALID_SIZE_DIVIDER)
	{
		printf("Incorrect block size. You must enter one of these - 8, 12, 16, 24, 32, 48, 64: ");
		scanf("%i", &blockSize);
	}

	//TODO: encoding in tempFile

	//удалить исходный файл с сообщением и переименовать temp
	if (!remove(FILE_NAME) || !rename(TEMP_FILE_NAME, FILE_NAME))
	{
		printf("ERROR: unable to write result in source file.\n");
		printf("Result can be found in temp.txt.\n");
		return;
	}
	printf("Successfully encoded.\n");
}