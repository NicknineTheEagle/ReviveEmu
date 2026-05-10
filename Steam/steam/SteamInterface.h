#pragma once

#include "SteamInterfaceImpl.h"
#include "SteamDLLAppsystemImpl.h"

extern CLogFile* Logger;
extern bool bLogging;

extern void RevInitialize(const char* cszInitSource);

STEAM_API void* STEAM_CALL CreateInterface(const char* cszSteamDLLAppsystemInterfaceVersion, int* pReturnCode)
{
	if (bLogging) Logger->Write("CreateInterface(%s)\n", cszSteamDLLAppsystemInterfaceVersion);

	static CSteamDLLAppsystem SteamDLLAppsystem;

	if (cszSteamDLLAppsystemInterfaceVersion != NULL)
	{
		if (strcmp(cszSteamDLLAppsystemInterfaceVersion, "SteamDLLAppsystem001") == 0)
		{
			if (pReturnCode) *pReturnCode = 0;
			return static_cast<IAppSystem*>(&SteamDLLAppsystem);
		}
	}

	if (pReturnCode) *pReturnCode = 1;
	return NULL;
}

STEAM_API void* STEAM_CALL _f(const char* cszSteamInterfaceVersion)
{
	RevInitialize("_f");

	static CSteamInterface SteamInterface;

	if (cszSteamInterfaceVersion != NULL)
	{
		if (bLogging) Logger->Write("Using Interface %s\n", cszSteamInterfaceVersion);

		if (strcmp(cszSteamInterfaceVersion, "Steam001") == 0)
		{
			return static_cast<ISteamInterface001*>(&SteamInterface);
		}
		else if (strcmp(cszSteamInterfaceVersion, "Steam003") == 0)
		{
			return static_cast<ISteamInterface003*>(&SteamInterface);
		}
		else if (strcmp(cszSteamInterfaceVersion, "Steam004") == 0)
		{
			return static_cast<ISteamInterface004*>(&SteamInterface);
		}
		else if (strcmp(cszSteamInterfaceVersion, "Steam005") == 0)
		{
			return static_cast<ISteamInterface005*>(&SteamInterface);
		}
		else if (strcmp(cszSteamInterfaceVersion, "Steam006") == 0)
		{
			return static_cast<ISteamInterface006*>(&SteamInterface);
		}
	}

	return NULL;
}
