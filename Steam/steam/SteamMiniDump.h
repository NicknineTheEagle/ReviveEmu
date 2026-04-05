#pragma once

extern CLogFile* Logger;
extern bool bLogging;

#include "Win32MiniDump.h"

static CWin32MiniDump s_Win32MiniDump( "Steam", (_se_translator_function)SteamWriteMiniDumpUsingExceptionInfo );

/*
** MiniDump
*/

STEAM_API int STEAM_CALL SteamWriteMiniDumpFromAssert()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamWriteMiniDumpFromAssert\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamWriteMiniDumpSetComment( const char* cszComment )
{
	s_Win32MiniDump.SetComment( cszComment );
	return 1;
}

STEAM_API void STEAM_CALL SteamWriteMiniDumpUsingExceptionInfo( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers )
{
	s_Win32MiniDump.WriteUsingExceptionInfo( dwExceptionCode, pStructuredExceptionPointers );
	s_Win32MiniDump.ClearComments();
}

STEAM_API void STEAM_CALL SteamWriteMiniDumpUsingExceptionInfoWithBuildId( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers, unsigned int uSourceControlID )
{
	//Add BuildID as comment
	//s_Win32MiniDump.AddComment();
	s_Win32MiniDump.WriteUsingExceptionInfo( dwExceptionCode, pStructuredExceptionPointers );
	s_Win32MiniDump.ClearComments();
}