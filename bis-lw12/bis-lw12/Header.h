#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include <Aclapi.h>
#include <conio.h>
#include "Constants.h"

extern char fileName_g[MAX_FILE_NAME];

int exists(const char *fname);
PACL getDacl();
char* getUsername();
DWORD getAccessMask();
ACCESS_MODE getAccessMode();
int getAceIndex();

DWORD AddAceToObjectsSecurityDescriptor(
	LPTSTR pszObjName,          // name of object
	SE_OBJECT_TYPE ObjectType,  // type of object
	LPTSTR pszTrustee,          // trustee for new ACE
	TRUSTEE_FORM TrusteeForm,   // format of trustee structure
	DWORD dwAccessRights,       // access mask for new ACE
	ACCESS_MODE AccessMode,     // type of ACE
	DWORD dwInheritance         // inheritance flags for new ACE
);
LPTSTR getName(ACCESS_ALLOWED_ACE *pAce);

void printAllAces();
void createAce(LPTSTR username);
void editAce();