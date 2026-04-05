#pragma once

extern CLogFile* Logger;
extern bool bLogging;

/*
** Asynchrounous call handling
*/

STEAM_API int STEAM_CALL SteamProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamProcessCall\n");
// #endif
	SteamClearError(pError);

	if (pProgress)
	{
		pProgress->bValid = 1;
		pProgress->uPercentDone = 100;
		strcpy(pProgress->szProgress, "100%");
	}

	return 1;
}

STEAM_API int STEAM_CALL SteamAbortCall(SteamCallHandle_t handle, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamAbortCall\n");
// #endif
	SteamClearError(pError);
	return 1;
}

STEAM_API int STEAM_CALL SteamBlockingCall(SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamBlockingCall\n");
// #endif
	SteamClearError(pError);
	return 1;
}

STEAM_API int STEAM_CALL SteamSetMaxStallCount(unsigned int uNumStalls, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamSetMaxStallCount\n");
// #endif
	SteamClearError(pError);
	return 1;
}