#include "Header.h"

char fileName_g[MAX_FILE_NAME] = { 0 };

int main()
{	
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	printf("Enter file name: ");
	scanf("%s", fileName_g);
	if (!exists(fileName_g))
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
		printf("2. Print all ACEs\n");
		printf("3. Edit existing ACE\n");
		printf("4. Exit\n");
		operationCode = _getch();
		switch (operationCode)
		{
		case '1': createAce(NULL); break;
		case '2': printAllAces(); break; 
		case '3': editAce(); break;
		default: break;
		}
		printf("Press enter to continue.\n");
		_getch();
	} while (operationCode != '4');
	return 0;
}
