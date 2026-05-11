#pragma once

#include "SteamInterfaceImpl.h"
#include "SteamDLLAppsystemImpl.h"

extern CLogFile* Logger;
extern bool bLogging;

extern void RevInitialize(const char* cszInitSource);

STEAM_API void * STEAM_CALL CreateInterface( const char *pName, int *pReturnCode )
{
	if (bLogging) Logger->Write("CreateInterface(%s)\n", pName);

	static CSteamDLLAppsystem SteamDLLAppsystem;

	if (pName != NULL)
	{
		if (strcmp(pName, "SteamDLLAppsystem001") == 0)
		{
			if (pReturnCode) *pReturnCode = 0;
			return static_cast<IAppSystem*>(&SteamDLLAppsystem);
		}
	}

	if (pReturnCode) *pReturnCode = 1;
	return NULL;
}

STEAM_API void * STEAM_CALL _f( const char *szInterfaceVersionRequested )
{
	RevInitialize("_f");

	static CSteamInterface SteamInterface;

	if (szInterfaceVersionRequested != NULL)
	{
		if (bLogging) Logger->Write("Using Interface %s\n", szInterfaceVersionRequested);

		if (strcmp(szInterfaceVersionRequested, "Steam001") == 0)
		{
			return static_cast<ISteamInterface001*>(&SteamInterface);
		}
		else if (strcmp(szInterfaceVersionRequested, "Steam003") == 0)
		{
			return static_cast<ISteamInterface003*>(&SteamInterface);
		}
		else if (strcmp(szInterfaceVersionRequested, "Steam004") == 0)
		{
			return static_cast<ISteamInterface004*>(&SteamInterface);
		}
		else if (strcmp(szInterfaceVersionRequested, "Steam005") == 0)
		{
			return static_cast<ISteamInterface005*>(&SteamInterface);
		}
		else if (strcmp(szInterfaceVersionRequested, "Steam006") == 0)
		{
			return static_cast<ISteamInterface006*>(&SteamInterface);
		}
	}

	return NULL;
}
