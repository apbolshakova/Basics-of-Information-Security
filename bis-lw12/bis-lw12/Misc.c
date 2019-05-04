#include "Header.h"

int exists(const char *fname)
{
	FILE *file;
	if ((file = fopen(fname, "r")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

char* getUsername()
{
	system("cls");
	printf("Enter username: ");
	char* username = (char*)malloc(sizeof(char)* MAX_USERNAME);
	scanf("%s", username);
	return username;
}

DWORD getAccessMask()
{
	char choice = 0;
	do {
		system("cls");
		printf("Enter number of the access to edit:\n");
		printf("1. To read, write, and execute the object (generic all)\n");
		printf("2. To modify the control (security) information\n");
		printf("3. To modify the owner SID of the object\n");
		printf("4. To delete file\n");
		printf("5. To read the information maintained by the object\n");
		printf("6. To write the information maintained by the object\n");
		printf("7. To execute or alternatively look into the object\n\n");
		choice = _getch();
	} while (!strchr("1234567", choice));
	DWORD mask = 0;
	switch (choice)
	{
	case '1': mask |= GENERIC_ALL; break;
	case '2': mask |= WRITE_DAC; break;
	case '3': mask |= WRITE_OWNER; break;
	case '4': mask |= DELETE; break;
	case '5': mask |= FILE_GENERIC_READ; break;
	case '6': mask |= FILE_GENERIC_WRITE; break;
	case '7': mask |= FILE_GENERIC_EXECUTE; break;
	default: break;
	}
	return mask;
}

ACCESS_MODE getAccessMode()
{
	ACCESS_MODE accessMode = 0;
	do {
		system("cls");
		printf("Enter access mode (2 for setting access, 3 for denying): ");
		accessMode = _getch();
	} while (!strchr("23", accessMode));
	accessMode -= '0';
	return accessMode;
}

PACL getDacl()
{
	PACL pDacl = NULL;
	PSECURITY_DESCRIPTOR pSD = 0;
	DWORD dwRes = GetNamedSecurityInfo(fileName_g, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION,
		NULL, NULL, &pDacl, NULL, &pSD);

	if (ERROR_SUCCESS != dwRes)
	{
		if (pSD != NULL)
			LocalFree((HLOCAL)pSD);
		if (pDacl != NULL)
			LocalFree((HLOCAL)pDacl);
		printf("\nGetNamedSecurityInfo Error %u\n", dwRes);
	}
	return pDacl;
}

int getAceIndex()
{
	printf("Enter which ACE you would like to edit: ");
	char input[MAX_LEN] = { 0 };
	scanf("%s", input);
	char* p = input;
	int i = ERR;
	while (*p)
	{
		if (isdigit(*p)) i = strtol(p, &p, BASE);
		else p++;
	}
	return i;
}