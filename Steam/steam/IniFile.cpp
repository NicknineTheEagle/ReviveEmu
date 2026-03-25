#include "stdafx.h"

CIniFile::CIniFile(char* szIniFilePath)
{
	strcpy(this->szIniFilePath, szIniFilePath);
}

CIniFile::~CIniFile()
{
}

bool CIniFile::IniWriteValue(char* Section, char* Key, char* Value)
{
	return (WritePrivateProfileStringA(Section, Key, Value, this->szIniFilePath) == TRUE ? true : false);
}

char* CIniFile::IniReadValue(char* Section, char* Key)
{
	char* retval = new char[MAX_PATH * 5];
	if(GetPrivateProfileStringA(Section, Key, "", retval, MAX_PATH * 5, this->szIniFilePath))
	{
		return retval;
	}
	delete [] retval;
	return NULL;
}
