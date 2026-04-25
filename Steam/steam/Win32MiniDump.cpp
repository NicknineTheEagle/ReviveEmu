#include "stdafx.h"
#include "Steam.h"
#include "Win32MiniDump.h"

CWin32MiniDump::CWin32MiniDump(const char* cszName, _se_translator_function fnSETranslatorFunction)
{
	V_strcpy_safe(m_szName, cszName);
	_set_se_translator(fnSETranslatorFunction);
}

CWin32MiniDump::~CWin32MiniDump()
{
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
	SYSTEMTIME systemtime;
	GetSystemTime(&systemtime);

	char szFileName[MAX_PATH];
	V_sprintf_safe(szFileName, "%s_%04u_%02u_%02u__%02u_%02u_%02u_%03u.mdmp", m_szName,
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
		mdmpExceptionInfo.ClientPointers = FALSE;

		//AddComments as User Stream!

		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hMiniDumpFile,
			(_MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithHandleData | MiniDumpWithProcessThreadData),
			&mdmpExceptionInfo, NULL, NULL);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
	}

	CloseHandle(hMiniDumpFile);
}
