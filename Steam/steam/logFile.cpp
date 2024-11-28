/////////////////////////////////////////////////////////////////
//features:	not depends on MFC ore ATL.
//			file name could use absolute path or just the name, in which case the 
//			file will be created at the same place with the binary module, no concern 
//			with curret directory, which always bring me truble.
//			every log line has a time stamp attached, with million seconds.
//			uses printf like format to write log lines
//			uses a preprocessor definition _DEBUG_LOG to switch if write log file
//			multi thread safe, finally added:)

////////////////////////////////////////////////////
#pragma warning(disable:4996)//disable deprecated warnings

#include "stdafx.h"
#include "CriticalSection.h"

extern CRITICAL_SECTION g_CriticalSection;
#define ENTER_CRITICAL_SECTION CEnterCriticalSection ECS(&g_CriticalSection)

//	Constructor, open the logfile
CLogFile::CLogFile(char* strFile)
{
	//AllocConsole();
	strcpy(m_filename, strFile);
}

//	Destructor, close if logfile is opened
CLogFile::~CLogFile()
{
	//FreeConsole();
	if (m_pLogFile)
		fflush(m_pLogFile);
		fclose(m_pLogFile);
}

//	Write log info into the logfile, with printf like parameters support
void CLogFile::Write(char*  pszFormat, ...)
{
	
	ENTER_CRITICAL_SECTION;

	m_pLogFile = fopen(m_filename, "a");
	if (!m_pLogFile)
		return;

	//write the formated log string to szLog
	char	szLog[1024];
	va_list argList;
	va_start( argList, pszFormat );
	vsprintf( szLog, pszFormat, argList );
	va_end( argList );

	//Trancate if the file grow too large
	//		long	lLength = ftell(m_pLogFile);
	//		if (lLength > m_lTruncate)
	//			rewind(m_pLogFile);

	//Get current time
	SYSTEMTIME	time;
	GetLocalTime(&time);
	char szLine[1024];

	// ido kijelzessel
	sprintf(szLine, "%02d:%02d:%02d:%03d [%u\\%u]\t%s", 
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
		GetCurrentProcessId(), GetCurrentThreadId(), szLog);

	//sprintf(szLine, "%04d/%02d/%02d %02d:%02d:%02d:%03d \t%s", 
	//	time.wYear, time.wMonth, time.wDay,
	//	time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
	//	szLog);

	fputs(szLine, m_pLogFile);
	fflush(m_pLogFile);
	fclose(m_pLogFile);
	//_cprintf(szLog);
	
}
void CLogFile::Clear()
{
	m_pLogFile = fopen(m_filename, "w");
	if (!m_pLogFile)
		return;
	fclose(m_pLogFile);

}
