#include "Header.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char fileName[MAX_FILE_NAME] = { 0 };
	printf("Enter file name: ");
	scanf("%s", fileName);
	if (!exists(fileName))
	{
		printf("ERROR: file doesn't exist.\n");
		_getch();
		return 0;
	}

	int operationCode = 0;
	do
	{
		system("cls");
		printf("Enter number of required operation:\n");
		printf("1. Create new ACE\n");
		printf("2. Read all ACEs\n");
		printf("3. Change existing ACE\n");
		printf("4. Exit\n");
		operationCode = _getch();
		switch (operationCode)
		{
		case '1': createACE(fileName); break;
		case '2': readACE(fileName); break; 
		case '3': changeACE(fileName); break;
		default: break;
		}
		printf("Press enter to continue.\n");
		_getch();
	} while (operationCode != '4');
	return 0;
}
