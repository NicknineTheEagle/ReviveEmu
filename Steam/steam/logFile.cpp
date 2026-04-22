#include "stdafx.h"

//	Constructor, open the logfile
CLogFile::CLogFile(const char* strFile)
{
	//AllocConsole();
	strcpy(m_filename, strFile);
	m_pLogFile = NULL;
}

//	Destructor, close if logfile is opened
CLogFile::~CLogFile()
{
	//FreeConsole();
	if (m_pLogFile)
	{
		fflush(m_pLogFile);
		fclose(m_pLogFile);
	}
}

//	Write log info into the logfile, with printf like parameters support
void CLogFile::Write(const char* pszFormat, ...)
{
	std::lock_guard<std::mutex> lock(m_LogMutex);

	m_pLogFile = fopen(m_filename, "a");
	if (!m_pLogFile)
		return;

	//write the formated log string to szLog
	char szLog[1024];
	va_list argList;
	va_start( argList, pszFormat );
	V_vsprintf_safe( szLog, pszFormat, argList );
	va_end( argList );

	//Trancate if the file grow too large
	//		long	lLength = ftell(m_pLogFile);
	//		if (lLength > m_lTruncate)
	//			rewind(m_pLogFile);

	char szLine[1024] = "";

#if defined (_WIN32)
	//Get current time
	SYSTEMTIME time;
	GetLocalTime(&time);

	V_sprintf_safe(szLine, "%02d:%02d:%02d:%03d [%u\\%u]\t%s", 
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
		GetCurrentProcessId(), GetCurrentThreadId(), szLog);

	//V_sprintf_safe(szLine, "%04d/%02d/%02d %02d:%02d:%02d:%03d \t%s", 
	//	time.wYear, time.wMonth, time.wDay,
	//	time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
	//	szLog);
#elif defined(_OSX)
	//Get current time
	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct tm tm;
	localtime_r(&tv.tv_sec, &tm);

	uint64 tid;
	pthread_threadid_np(NULL, &tid);

	V_sprintf_safe(szLine, "%02d:%02d:%02d:%03d [%d\\%llu]\t%s", 
		tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec / 1000),
		(int)getpid(), tid, szLog);
#else
	//Get current time
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	struct tm tm;
	localtime_r(&ts.tv_sec, &tm);

	V_sprintf_safe(szLine, "%02d:%02d:%02d:%03d [%d\\%d]\t%s", 
		tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(ts.tv_nsec / 1000000),
		(int)getpid(), (int)syscall(SYS_gettid), szLog);
#endif

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
