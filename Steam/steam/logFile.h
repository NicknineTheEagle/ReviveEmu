#pragma once

class CLogFile
{
private:
	FILE* m_pLogFile;
	char m_filename[MAX_PATH];
	std::mutex m_LogMutex;

public:
	CLogFile(char* strFile);
	~CLogFile();

	void Write(char*  pszFormat, ...);
	void Clear();
};
