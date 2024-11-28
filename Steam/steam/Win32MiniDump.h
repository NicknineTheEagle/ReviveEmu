#pragma once

#include <string>
#include <vector>
#include "dbghelp.h"

using namespace std;

typedef BOOL ( WINAPI* fnMiniDumpWriteDump )( IN HANDLE hProcess, IN DWORD ProcessId, IN HANDLE hFile, IN MINIDUMP_TYPE DumpType, IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL );

class CWin32MiniDump
{
private:
	HMODULE m_hDbgHelp;
	fnMiniDumpWriteDump m_fnMiniDumpWriteDump;
	string m_sName;
	string m_sComment;
	vector<string> m_vecComments;
public:
	CWin32MiniDump( const char* cszName, _se_translator_function fnSETranslatorFunction );
	~CWin32MiniDump();

	void SetComment( const char* cszComment );
	void AddComment( const char* cszComment );
	void ClearComments();
	void WriteUsingExceptionInfo( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers );
};
