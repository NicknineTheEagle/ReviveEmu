#pragma once

extern CLogFile* Logger;
extern bool bLogging;

class CSteamLogging
{
public:
	CSteamLogging(){}
	virtual ~CSteamLogging(){}
};

/*
** Logging
*/

STEAM_API SteamHandle_t STEAM_CALL SteamCreateLogContext(const char *cszName)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamCreateLogContext\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamLog(SteamHandle_t hContext, const char *cszMsg)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamLog\n");
// #endif
	return 1;
}

STEAM_API void STEAM_CALL SteamLogResourceLoadStarted(const char *cszMsg)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamLogResourceLoadStarted: %s\n", cszMsg);
// #endif
}

STEAM_API void STEAM_CALL SteamLogResourceLoadFinished(const char *cszMsg)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamLogResourceLoadFinished %s\n", cszMsg);
// #endif
}