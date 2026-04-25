#include "stdafx.h"
#include "Steam.h"
#include "Win32MiniDump.h"

CWin32MiniDump::CWin32MiniDump(const char* cszName, _se_translator_function fnSETranslatorFunction)
{
	strncpy(m_szName, cszName, MAX_PATH);
	m_hDbgHelp = NULL;
	m_fnMiniDumpWriteDump = NULL;

	m_hDbgHelp = LoadLibraryA("DbgHelp.dll");
	if (m_hDbgHelp)
	{
		FARPROC fpMiniDumpWriteDump = GetProcAddress(m_hDbgHelp, "MiniDumpWriteDump");

		if (fpMiniDumpWriteDump)
		{
			m_fnMiniDumpWriteDump = fnMiniDumpWriteDump(fpMiniDumpWriteDump);
			_set_se_translator(fnSETranslatorFunction);
		}
		else
		{
			FreeLibrary(m_hDbgHelp);
			m_hDbgHelp = NULL;
		}
	}
}

CWin32MiniDump::~CWin32MiniDump()
{
	if (m_hDbgHelp)
		FreeLibrary(m_hDbgHelp);

	ClearComments();
}

void CWin32MiniDump::SetComment(const char* cszComment)
{
	return;
}

void CWin32MiniDump::AddComment(const char* cszComment)
{
	return;
}

void CWin32MiniDump::ClearComments()
{
	return;
}

void CWin32MiniDump::WriteUsingExceptionInfo(DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers)
{
	if (!m_fnMiniDumpWriteDump)
		return;

	SYSTEMTIME systemtime;
	GetSystemTime(&systemtime);

	char szFileName[MAX_PATH];
	sprintf(szFileName, "%s_%04u_%02u_%02u__%02u_%02u_%02u_%03u.mdmp", m_szName,
		systemtime.wYear, systemtime.wMonth, systemtime.wDay, systemtime.wHour,
		systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds);

	HANDLE hMiniDumpFile = CreateFileA(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hMiniDumpFile == INVALID_HANDLE_VALUE)
		return;

	__try
	{
		MINIDUMP_EXCEPTION_INFORMATION mdmpExceptionInfo;
		mdmpExceptionInfo.ThreadId = GetCurrentThreadId();
		mdmpExceptionInfo.ExceptionPointers = pStructuredExceptionPointers;
		mdmpExceptionInfo.ClientPointers = 0;

		//AddComments as User Stream!

		m_fnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hMiniDumpFile,
			(_MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithHandleData | MiniDumpWithProcessThreadData),
			&mdmpExceptionInfo, NULL, NULL);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	CloseHandle(hMiniDumpFile);
}
