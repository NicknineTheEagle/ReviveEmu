#pragma once

#include "registry.h"

extern CLogFile* Logger;
extern bool bLogging;
extern bool g_bSteamStartup;

extern void InitGlobalVariables(const char* cszInitSource);

/*
** Initialization
*/

STEAM_API int STEAM_CALL SteamStartEngine(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamStartEngine\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamStartup(unsigned int uUsingMask, TSteamError *pError)
{
	if (bLogging) Logger->Write("SteamStartup\n");

	SteamClearError(pError);

	if (!g_bSteamStartup)
	{
		g_bSteamStartup = true;

		InitGlobalVariables("SteamStartup");
		setRegistry("Software\\Valve\\Steam", "Language", g_szLanguage);

		if (g_bSteamFileSystem)
		{
			g_CacheManager = new CCacheFileSystem();

			if (g_bSteamBlobSystem)
			{
				if (g_bRawCDR)
				{
					CDR = CContentDescriptionRecord::LoadFromFile(g_szCDRFile);
				}
				else
				{
					CBlobFileSystem ClientRegistryBlob;
					if (ClientRegistryBlob.Open(g_szBlobFile))
					{
						CBlobNode* CDRNode = ClientRegistryBlob.GetNodeByPath("ContentDescriptionRecord");
						if (CDRNode)
						{
							CDR = new CContentDescriptionRecord(CDRNode->KeyValue->Value);
						}
					}
				}

				if (!CDR)
				{
					if (bLogging) Logger->Write("	Error Locating CDR ... Advanced Steam Functions Disabled!\n");
					if (bLogging) Logger->Write("	GCF Support from Ini file only!\n");
					g_bSteamBlobSystem = false;
				}
			}
		}
	}

	return 1;
}

STEAM_API int STEAM_CALL SteamCleanup(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamCleanup\n");
// #endif
	SteamClearError(pError);

	if (g_bSteamStartup)
	{
		if (g_bSteamFileSystem)
		{
			if (g_CacheManager)
			{
				delete g_CacheManager;
				g_CacheManager = NULL;
			}

			if (CDR)
			{
				delete CDR;
				CDR = NULL;
			}
		}

		setRegistry("Software\\Valve\\Steam", "Language", g_szOLDLanguage);

		g_bSteamStartup = false;
	}

	return 1;
}

STEAM_API int STEAM_CALL SteamGetVersion(char *szVersion, unsigned int uVersionBufSize)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetVersion\n");
// #endif

	if (!szVersion)
		return 0;

	strncpy(szVersion, "2.0.0.0", uVersionBufSize);
	return 1;
}

STEAM_API int STEAM_CALL SteamShutdownEngine(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamShutdownEngine\n");
// #endif
	return 1;
}