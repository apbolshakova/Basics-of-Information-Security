#include "Header.h"

void changeACE(char *fileName)
{
	PACL pDacl = readACE(fileName);
	PSID pSid = NULL;
	PSID pSavedSid = NULL;
	ACCESS_ALLOWED_ACE *pAce = NULL;

	printf("Enter which ACE you would like to edit: ");
	char input[MAX_LEN] = { 0 };
	scanf("%s", input);
	char* p = input;
	int key = 0;
	while (*p) 
	{
		if (isdigit(*p)) key = strtol(p, &p, 10); 
		else p++;
	}

	if (!GetAce(pDacl, key, &pAce))
	{
		printf("Incorrect ACE.\n");
		goto Cleanup;
	}

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

	if (!DeleteAce(pDacl, key))
	{
		printf("Error while deleting sid.\n");
		goto Cleanup;
	}

	DWORD accessMask = getAccessMask();

	ACCESS_MODE accessMode = 0;
	do {
		system("cls");
		printf("Enter access mode (2 for setting access, 3 for denying): ");
		accessMode = _getch();
	} while (!strchr("23", accessMode));
	accessMode -= '0';

	AddAceToObjectsSecurityDescriptor(
		fileName,
		SE_FILE_OBJECT,
		username, TRUSTEE_IS_NAME,
		accessMask,
		accessMode,
		SUB_CONTAINERS_AND_OBJECTS_INHERIT
	);
	system("cls");
	printf("Successfully edited\n");

Cleanup:

	if (pSid != NULL)
		LocalFree((HLOCAL)pSid);
	if (pSavedSid != NULL)
		LocalFree((HLOCAL)pSavedSid);
	if (pDacl != NULL)
		LocalFree((HLOCAL)pDacl);
}