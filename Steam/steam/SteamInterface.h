#pragma once

#include "SteamInterface001.h"
#include "SteamInterface003.h"
#include "SteamInterface004.h"
#include "SteamInterface005.h"
#include "SteamInterface006.h"
#include "SteamDLLAppsystem001.h"

extern CLogFile* Logger;
extern bool bLogging;

STEAM_API void* STEAM_CALL CreateInterface(const char* cszSteamDLLAppsystemInterfaceVersion, int* pReturnCode)
{
	if(bLogging) Logger->Write("CreateInterface(%s)\n", cszSteamDLLAppsystemInterfaceVersion); 
	static CSteamDLLAppsystem001 SteamDLLAppsystem001;

	if(cszSteamDLLAppsystemInterfaceVersion != NULL)
	{
		if(strcmp(cszSteamDLLAppsystemInterfaceVersion,"SteamDLLAppsystem001") == 0)
		{
			if (pReturnCode)
				*pReturnCode = 0;
			return (void*)&SteamDLLAppsystem001;
		}
	}

	if (pReturnCode)
		*pReturnCode = 1;
	return NULL;
}

STEAM_API void* STEAM_CALL _f(const char* cszSteamInterfaceVersion)
{
	if(cszSteamInterfaceVersion != NULL)
	{
		if(bLogging) Logger->Write("Using Interface %s\n", cszSteamInterfaceVersion);
		if(strcmp(cszSteamInterfaceVersion,"Steam001") == 0)
		{
			static CSteamInterface001 SteamInterface001;
			return (void*)&SteamInterface001;
		}
		else if(strcmp(cszSteamInterfaceVersion,"Steam003") == 0)
		{
			static CSteamInterface003 SteamInterface003;
			return (void*)&SteamInterface003;
		}
		else if(strcmp(cszSteamInterfaceVersion,"Steam004") == 0)
		{
			static CSteamInterface004 SteamInterface004;
			return (void*)&SteamInterface004;
		}
		else if(strcmp(cszSteamInterfaceVersion,"Steam005") == 0)
		{
			static CSteamInterface005 SteamInterface005;
			return (void*)&SteamInterface005;
		}
		else if(strcmp(cszSteamInterfaceVersion,"Steam006") == 0)
		{
			static CSteamInterface006 SteamInterface006;
			return (void*)&SteamInterface006;
		}
	}

	return NULL;
}
