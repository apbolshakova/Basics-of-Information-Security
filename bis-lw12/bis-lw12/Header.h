#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <locale.h>
#include <Windows.h>
#include <Aclapi.h>
#include <conio.h>
#include "Constants.h"

int exists(const char *fname);
void printName(ACCESS_ALLOWED_ACE *pAce);

DWORD AddAceToObjectsSecurityDescriptor(
	LPTSTR pszObjName,          // name of object
	SE_OBJECT_TYPE ObjectType,  // type of object
	LPTSTR pszTrustee,          // trustee for new ACE
	TRUSTEE_FORM TrusteeForm,   // format of trustee structure
	DWORD dwAccessRights,       // access mask for new ACE
	ACCESS_MODE AccessMode,     // type of ACE
	DWORD dwInheritance         // inheritance flags for new ACE
);

PACL readACE(char *fileName);
void createACE(char *fileName);
void changeACE(char *fileName);