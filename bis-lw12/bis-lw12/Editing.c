#include "Header.h"

void editAce()
{
	PACL pDacl = getDacl();
	ACCESS_ALLOWED_ACE* pAce = NULL;
	PSECURITY_DESCRIPTOR pSD;
	
	printAllAces();
	long aceIndex = getAceIndex();
	
	if (aceIndex == ERR || !GetAce(pDacl, aceIndex, &pAce))
	{
		printf("Incorrect ACE.\n");
		goto Cleanup;
	}

	LPTSTR name = getName(pAce);
	if (!DeleteAce(pDacl, aceIndex))
	{
		printf("Error while deleting sid.\n");
		goto Cleanup;
	}
	DWORD dwRes = SetNamedSecurityInfo(fileName_g, SE_FILE_OBJECT,
		DACL_SECURITY_INFORMATION, NULL, NULL, pDacl, NULL);
	if (ERROR_SUCCESS != dwRes) 
	{
		printf("SetNamedSecurityInfo Error %u\n", dwRes);
		goto Cleanup;
	}

	createAce(name, pAce);
	system("cls");
	printf("Successfully edited\n");

Cleanup:

	if (pDacl != NULL)
		LocalFree((HLOCAL)pDacl);
}