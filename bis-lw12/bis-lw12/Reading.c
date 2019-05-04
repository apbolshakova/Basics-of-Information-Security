#include "Header.h"

void printAllAces()
{
	PACL pDacl = getDacl();
	ACCESS_ALLOWED_ACE* pAce = NULL;
	system("cls");
	for (int i = 0; i < pDacl->AceCount; i++) 
	{
		if (!GetAce(pDacl, i, &pAce))
		{
			printf("ERROR in GetAce function\n");
			goto Cleanup;
		}
		printf("%i. ", i);
		LPTSTR name = getName(pAce);
		printf("%s ", name);
		if (pAce->Header.AceType == ACCESS_ALLOWED_ACE_TYPE) printf("is able to\n");
		else printf("is not able to\n");
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
	
Cleanup:
	if (pDacl != NULL)
		LocalFree((HLOCAL)pDacl);
}