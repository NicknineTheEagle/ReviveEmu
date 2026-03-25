#include "stdafx.h"
#include "Steam.h"
#include "Win32MiniDump.h"

CWin32MiniDump::CWin32MiniDump( const char* cszName, _se_translator_function fnSETranslatorFunction )
{
	this->m_sName.assign( cszName );
	this->m_hDbgHelp = NULL;
	this->m_fnMiniDumpWriteDump = NULL;

	this->m_hDbgHelp = LoadLibraryA("DbgHelp.dll");
	if( this->m_hDbgHelp )
	{
		FARPROC fpMiniDumpWriteDump = GetProcAddress( this->m_hDbgHelp, "MiniDumpWriteDump" );

		if(fpMiniDumpWriteDump)
		{
			this->m_fnMiniDumpWriteDump = fnMiniDumpWriteDump( fpMiniDumpWriteDump );
			_set_se_translator( fnSETranslatorFunction );
		}
		else
		{
			FreeLibrary( this->m_hDbgHelp );
			this->m_hDbgHelp = NULL;
		}
	}
}

CWin32MiniDump::~CWin32MiniDump()
{
	if( this->m_hDbgHelp )
		FreeLibrary( this->m_hDbgHelp );

	this->ClearComments();
}

void CWin32MiniDump::SetComment( const char* cszComment )
{
	this->m_sComment.assign( cszComment );
}

void CWin32MiniDump::AddComment( const char* cszComment )
{
	this->m_vecComments.push_back( std::string( cszComment ) );
}

void CWin32MiniDump::ClearComments()
{
	this->m_vecComments.clear();
}

void CWin32MiniDump::WriteUsingExceptionInfo( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers )
{
	if( !this->m_fnMiniDumpWriteDump )
		return;

	SYSTEMTIME systemtime;
	GetSystemTime( &systemtime );

	char szFileName[1024];
	sprintf( szFileName, "%s_%04u_%02u_%02u__%02u_%02u_%02u_%03u.mdmp", this->m_sName.c_str(), systemtime.wYear, systemtime.wMonth, systemtime.wDay, systemtime.wHour, systemtime.wMinute, systemtime.wSecond, systemtime.wMilliseconds );

	HANDLE hMiniDumpFile = CreateFileA( szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if( hMiniDumpFile == INVALID_HANDLE_VALUE )
		return;

	__try
	{
		MINIDUMP_EXCEPTION_INFORMATION mdmpExceptionInfo;
		mdmpExceptionInfo.ThreadId = GetCurrentThreadId();
		mdmpExceptionInfo.ExceptionPointers = pStructuredExceptionPointers;
		mdmpExceptionInfo.ClientPointers = 0;

		//AddComments as User Stream!

		this->m_fnMiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hMiniDumpFile, (_MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithHandleData | MiniDumpWithProcessThreadData), &mdmpExceptionInfo, NULL, NULL );
	}
	__except( EXCEPTION_EXECUTE_HANDLER )
	{
	}

	CloseHandle( hMiniDumpFile );
}
