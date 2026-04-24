#include "stdafx.h"

// Constructor, open the logfile
CLogFile::CLogFile(const char* cszFileName)
{
	m_pLogFile = fopen(cszFileName, "w");
}

// Destructor, close if logfile is opened
CLogFile::~CLogFile()
{
	if (m_pLogFile)
	{
		fclose(m_pLogFile);
	}
}

// Write log info into the logfile, with printf like parameters support
void CLogFile::Write(const char* cszFormat, ...)
{
	std::lock_guard<std::mutex> lock(m_LogMutex);

	if (!m_pLogFile)
		return;

	//Trancate if the file grow too large
	//		long	lLength = ftell(m_pLogFile);
	//		if (lLength > m_lTruncate)
	//			rewind(m_pLogFile);

	// Write the timestamp.
#if defined (_WIN32)
	SYSTEMTIME time;
	GetLocalTime(&time);

	fprintf(m_pLogFile, "%02d:%02d:%02d:%03d [%u\\%u]\t", 
		time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
		GetCurrentProcessId(), GetCurrentThreadId());
#elif defined(_OSX)
	struct timeval tv;
	gettimeofday(&tv, NULL);

	struct tm tm;
	localtime_r(&tv.tv_sec, &tm);

	uint64 tid;
	pthread_threadid_np(NULL, &tid);

	fprintf(m_pLogFile, "%02d:%02d:%02d:%03d [%d\\%llu]\t", 
		tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec / 1000),
		(int)getpid(), tid);
#else
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);

	struct tm tm;
	localtime_r(&ts.tv_sec, &tm);

	fprintf(m_pLogFile, "%02d:%02d:%02d:%03d [%d\\%d]\t", 
		tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(ts.tv_nsec / 1000000),
		(int)getpid(), (int)syscall(SYS_gettid));
#endif

	// Write the actual log line.
	va_list ap;
	va_start(ap, cszFormat);
	vfprintf(m_pLogFile, cszFormat, ap);
	va_end(ap);

	fflush(m_pLogFile);
}
