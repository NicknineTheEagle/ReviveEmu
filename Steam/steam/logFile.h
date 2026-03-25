#pragma once

class CLogFile
{
private:
	FILE*	m_pLogFile;
	char	m_filename[MAX_PATH];

public:
	CLogFile(char* strFile);
	~CLogFile();

	void Write(char*  pszFormat, ...);
	void Clear();
};
