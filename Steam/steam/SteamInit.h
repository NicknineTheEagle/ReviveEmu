#pragma once

extern CLogFile* Logger;
extern bool bLogging;
extern bool g_bSteamStartup;
extern std::map<unsigned int, std::vector<TSteamAppDependencyInfo>> g_AppDependencies;

extern void RevInitialize(const char* cszInitSource);

/*
** Initialization
*/

STEAM_API int STEAM_CALL SteamStartEngine( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamStartEngine\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamShutdownEngine( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamShutdownEngine\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamStartup( unsigned int uUsingMask, TSteamError *pError )
{
	if (bLogging) Logger->Write("SteamStartup (0x%02X)\n", uUsingMask);

	SteamClearError(pError);

	if (!g_bSteamStartup)
	{
		g_bSteamStartup = true;

		RevInitialize("SteamStartup");
#ifdef _WIN32
		setRegistry("Software\\Valve\\Steam", "Language", g_szLanguage);
#endif

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

STEAM_API int STEAM_CALL SteamCleanup( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamCleanup\n");
// #endif
	SteamClearError(pError);

	if (g_bSteamStartup)
	{
		if (g_bSteamFileSystem)
		{
			g_AppDependencies.clear();

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

#ifdef _WIN32
		setRegistry("Software\\Valve\\Steam", "Language", g_szOLDLanguage);
#endif

		g_bSteamStartup = false;
	}

	return 1;
}

STEAM_API unsigned int STEAM_CALL SteamNumAppsRunning( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamNumAppsRunning\n");
// #endif
	return 1;
}

STEAM_API void STEAM_CALL SteamClearError( TSteamError *pError )
{
	if (pError)
	{
		pError->eSteamError = eSteamErrorNone;
		pError->eDetailedErrorType = eNoDetailedErrorAvailable;
		pError->nDetailedErrorCode = 0;
		pError->szDesc[0] = '\0';
	}
}

STEAM_API int STEAM_CALL SteamGetVersion( char *szVersion, unsigned int uVersionBufSize )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetVersion\n");
// #endif

	if (!szVersion)
		return 0;

	strncpy(szVersion, "2.0.0.0", uVersionBufSize);
	return 1;
}
