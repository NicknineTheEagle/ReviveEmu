#pragma once

class CIniFile
{
private:
	char szIniFilePath[MAX_PATH];
public:
	CIniFile(char* szIniFilePath);
	~CIniFile();

	bool IniWriteValue(char* Section, char* Key, char* Value);
	char* IniReadValue(char* Section, char* Key);
};
