#pragma once

extern CLogFile* Logger;
extern bool bLogging;

/*
** Misc
*/

STEAM_API void STEAM_CALL SteamClearError( TSteamError *pError )
{
	if(pError)
	{
// #ifdef DEBUG
		//if (bLogging) Logger->Write("SteamClearError\n");
// #endif
		pError->eSteamError = eSteamErrorNone;
		pError->eDetailedErrorType = eNoDetailedErrorAvailable;
		pError->nDetailedErrorCode = 0;
		pError->szDesc[0] = 0;
	}
	return;
}

STEAM_API int STEAM_CALL InternalSteamNumClientsConnectedToEngine()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("InternalSteamNumClientsConnectedToEngine\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL InternalSteamShouldShutdownEngine2()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("InternalSteamShouldShutdownEngine2\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetLocalClientVersion( unsigned int *puBootstrapperVersion, unsigned int *puClientVersion, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetLocalClientVersion\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamChangeOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamChangeOfflineStatus\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError )
{
	if (bLogging) Logger->Write("SteamGetOfflineStatus\n");

	SteamClearError(pError);
	pStatus->eOfflineNow = 0;
	pStatus->eOfflineNextSession = 0;
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUninstall( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamUninstall\n");
// #endif
	return 1;
}

STEAM_API ESteamError STEAM_CALL SteamWeakVerifyNewValveCDKey( const char *pszCDKeyFormattedForCDLabel, unsigned int *pReceiveGameCode, unsigned int *pReceiveSalesTerritoryCode, unsigned int *pReceiveUniqueSerialNumber )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamWeakVerifyNewValveCDKey\n");
// #endif
	return eSteamErrorUnknown;
}

STEAM_API ESteamError STEAM_CALL SteamGetEncryptedNewValveCDKey( const char *pszCDKeyFormattedForCDLabel, unsigned int ClientLocalIPAddr, const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedNewValveCDKey )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetEncryptedNewValveCDKey\n");
// #endif
	return eSteamErrorUnknown;
}

STEAM_API ESteamError STEAM_CALL SteamEncryptDataForThisMachine( const char *pDataToEncrypt, unsigned int uSizeOfDataToEncrypt, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedData )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamDecryptDataForThisMachine\n");
// #endif
	return eSteamErrorUnknown;
}

STEAM_API ESteamError STEAM_CALL SteamDecryptDataForThisMachine( const char *pDataToDecrypt, unsigned int uSizeOfDataToDecrypt, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfDecryptedData )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamEncryptDataForThisMachine\n");
// #endif
	return eSteamErrorUnknown;
}

STEAM_API const char * STEAM_CALL SteamFindServersGetErrorString()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamFindServersGetErrorString\n");
// #endif
	return "";
}

// hl1master.steampowered.com
// These have been shut down but keeping for historical purposes.
const char* g_aHL1MasterServers[] =
{
	"208.78.164.208:27010",
	"208.78.164.209:27010"
};

// hl2master.steampowered.com
const char* g_aHL2MasterServers[] =
{
	"208.64.200.65:27011",
	"192.69.99.29:27011",
	"208.78.164.210:27011"
};

// cser.steampowered.com
const char* g_aCSERServers[] =
{
	"208.64.203.186:27013"
};

STEAM_API int STEAM_CALL SteamFindServersIterateServer( ESteamServerType eServerType, unsigned int nServer, char *szIpAddrPort, int szIpAddrPortLen )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamFindServersIterateServer (%d, %u, 0x%p, %d)\n", eServerType, nServer, szIpAddrPort, szIpAddrPortLen);
// #endif
	if (g_bSteamDll)
	{
		int retval = 1;
		strcpy(szIpAddrPort, "empty");
		int (*fptr)(ESteamServerType, unsigned int, char*, int);
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamFindServersIterateServer");
		retval = fptr(eServerType, nServer, szIpAddrPort, szIpAddrPortLen);
		if (bLogging) Logger->Write("\t (%d, %u, %s, %d) %d\n", eServerType, nServer, szIpAddrPort, szIpAddrPortLen, retval);
		return retval;
	}

	const char** pServers = NULL;
	size_t iNumServers = 0;

	switch (eServerType)
	{
		case eSteamHalfLifeMasterServer:
			pServers = g_aHL1MasterServers;
			iNumServers = ARRAYSIZE(g_aHL1MasterServers);
			break;
		case eSteamCSERServer:
			pServers = g_aCSERServers;
			iNumServers = ARRAYSIZE(g_aCSERServers);
			break;
		case eSteamHalfLife2MasterServer:
			pServers = g_aHL2MasterServers;
			iNumServers = ARRAYSIZE(g_aHL2MasterServers);
			break;
		default:
			break;
	}

	if (pServers && nServer < iNumServers)
	{
		strncpy(szIpAddrPort, pServers[nServer], szIpAddrPortLen);
		return 0;
	}

	szIpAddrPort[0] = '\0';
	return -1;
}

STEAM_API int STEAM_CALL SteamFindServersNumServers( ESteamServerType eServerType )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamFindServersNumServers (%d)\n", eServerType);
// #endif
	if (g_bSteamDll)
	{
		int retval = 1;
		int (*fptr)(ESteamServerType);
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamFindServersNumServers");
		retval = fptr(eServerType);
		if (bLogging) Logger->Write("\t (%d) %d\n", eServerType, retval);
		return retval;
	}

	switch (eServerType)
	{
		case eSteamHalfLifeMasterServer:
			return ARRAYSIZE(g_aHL1MasterServers);
			break;
		case eSteamCSERServer:
			return ARRAYSIZE(g_aCSERServers);
			break;
		case eSteamHalfLife2MasterServer:
			return ARRAYSIZE(g_aHL2MasterServers);
			break;
		default:
			break;
	}

	return 0;
}

STEAM_API int STEAM_CALL SteamGetContentServerInfo( unsigned int uAppId, unsigned int *puServerId, unsigned int *puServerIpAddress, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetContentServerInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamSetNotificationCallback( SteamNotificationCallback_t pCallbackFunction, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamSetNotificationCallback\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamWasBlobRegistryDeleted( int *puWasDeleted, TSteamError *pError )
{
	if (bLogging) Logger->Write("SteamWasBlobRegistryDeleted\n");

	SteamClearError(pError);
	*puWasDeleted = 0;
	return 1;
}

int SteamGetCurrentAppId( unsigned int *puAppId, TSteamError *pError )
{
	if (bLogging) Logger->Write("SteamGetCurrentAppId\n");

	SteamClearError(pError);

	if (!g_uAppId)
	{
		*puAppId = 0;
		pError->eSteamError = eSteamErrorUnknown;
		return 0;
	}

	*puAppId = g_uAppId;
	return 1;
}
