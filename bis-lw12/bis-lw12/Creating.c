#include "Header.h"

void createAce(LPTSTR username)
{
	if (!username) username = getUsername();
	DWORD accessMask = getAccessMask();
	ACCESS_MODE accessMode = getAccessMode();

	AddAceToObjectsSecurityDescriptor(
		fileName_g,
		SE_FILE_OBJECT,
		username, TRUSTEE_IS_NAME,
		accessMask,
		accessMode,
		SUB_CONTAINERS_AND_OBJECTS_INHERIT
	);
	system("cls");
	printf("Successfully created\n");
}