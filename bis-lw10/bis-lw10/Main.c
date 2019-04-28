#include "Header.h"

int main(void)
{
	int operationCode = 0;
	do
	{
		system("cls");
		printf("Enter 1 to encode, 2 to decode and 3 to exit: ");
		scanf("%i", &operationCode);
		switch (operationCode)
		{
		case 1: handleEncoding(); break;
		case 2: handleDecoding(); break;
		default: break;
		}
		printf("Press enter to continue.\n");
		_getch();
	} while (operationCode != 3);
	return 0;
}