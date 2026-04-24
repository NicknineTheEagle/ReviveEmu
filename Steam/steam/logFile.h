#pragma once

class CLogFile
{
private:
	FILE* m_pLogFile;
	std::mutex m_LogMutex;

public:
	CLogFile(const char* cszFileName);
	~CLogFile();

	void Write(const char* cszFormat, ...);
};
