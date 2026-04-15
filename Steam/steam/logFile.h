#pragma once

class CLogFile
{
private:
	FILE* m_pLogFile;
	char m_filename[MAX_PATH];
	std::mutex m_LogMutex;

public:
	CLogFile(const char* strFile);
	~CLogFile();

	void Write(const char* pszFormat, ...);
	void Clear();
};
