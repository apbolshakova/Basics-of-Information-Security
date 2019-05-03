#include "Header.h"

PACL readACE(char *fileName)
{
	PACL pDacl;
	ACCESS_ALLOWED_ACE *pAce = NULL;
	PSECURITY_DESCRIPTOR pSD;

	system("cls");
	DWORD dwRes = GetNamedSecurityInfo(fileName, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, 
		NULL, NULL, &pDacl, NULL, &pSD);
	if (ERROR_SUCCESS != dwRes) {
		printf("\nGetNamedSecurityInfo Error %u\n", dwRes);
		goto Cleanup;
	}

	for (int i = 0; i < pDacl->AceCount; i++) 
	{
		if (!GetAce(pDacl, i, (LPVOID *)&pAce))
		{
			printf("ERROR in GetAce function\n");
			return;
		}
		printName(pAce);
		if (ACCESS_ALLOWED_ACE_TYPE == pAce->Header.AceType) printf("is able to\n");
		if (ACCESS_DENIED_ACE_TYPE == pAce->Header.AceType)  printf("is not able to\n");

		if ((pAce->Mask & WRITE_DAC) == WRITE_DAC)
		    printf("- modify the control (security) information\n");
		if ((pAce->Mask & WRITE_OWNER) == WRITE_OWNER)
			printf("- modify the owner SID of the object\n");
		if ((pAce->Mask & DELETE) == DELETE)
			printf("- delete file\n");
		if ((pAce->Mask & FILE_GENERIC_READ) == FILE_GENERIC_READ)
			printf("- read the information maintained by the object\n");
		if ((pAce->Mask & FILE_GENERIC_WRITE) == FILE_GENERIC_WRITE)
			printf("- write the information maintained by the object\n");
		if ((pAce->Mask & FILE_GENERIC_EXECUTE) == FILE_GENERIC_EXECUTE)
			printf("- execute or alternatively look into the object\n");
		printf("\n");
	}
	return pDacl;

Cleanup:

	if (pSD != NULL)
		LocalFree((HLOCAL)pSD);
	if (pDacl != NULL)
		LocalFree((HLOCAL)pDacl);
}