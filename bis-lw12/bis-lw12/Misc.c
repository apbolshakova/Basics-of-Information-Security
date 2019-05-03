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

void printName(ACCESS_ALLOWED_ACE *pAce)
{
	PSID sid = &pAce->SidStart;
	SID_NAME_USE sidNameUse;
	DWORD usernameLen = 0;
	DWORD domainLen = 0;

	LookupAccountSid(NULL, sid, 0, &usernameLen, 0, &domainLen, &sidNameUse);
	LPSTR username = (LPSTR)malloc(usernameLen * sizeof(TCHAR));
	if (!LookupAccountSid(NULL, sid, username, &usernameLen, 0, &domainLen, &sidNameUse))
	{
		free(username);
		return;
	}

	char *sidType = NULL;
	switch (sidNameUse) {

	case SidTypeUser:
		sidType = "User"; break;
	case SidTypeGroup:
		sidType = "Group"; break;
	case SidTypeDomain:
		sidType = "Domain"; break;
	case SidTypeAlias:
		sidType = "Alias"; break;
	case SidTypeWellKnownGroup:
		sidType = "Well known group"; break;
	case SidTypeDeletedAccount:
		sidType = "Deleted account"; break;
	case SidTypeInvalid:
		sidType = "Invalid"; break;
	case SidTypeUnknown:
		sidType = "Unknown"; break;
	case SidTypeComputer:
		sidType = "Computer"; break;
	}

	printf("%s %s ", sidType, username);
	free(username);
}

DWORD getAccessMask()
{
	char choice = 0;
	do {
		system("cls");
		printf("Enter number of the right to edit:\n");
		printf("1. To read, write, and execute the object (generic all)\n");
		printf("2. To modify the control (security) information\n");
		printf("3. To modify the owner SID of the object\n");
		printf("4. To delete file\n");
		printf("5. To read the information maintained by the object\n");
		printf("6. To write the information maintained by the object\n");
		printf("7. To execute or alternatively look into the object\n");
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