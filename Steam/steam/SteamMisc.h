#pragma once

extern CLogFile* Logger;
extern bool bLogging;

/*
** Misc
*/

STEAM_API void STEAM_CALL SteamClearError(TSteamError *pError)
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

STEAM_API int STEAM_CALL SteamGetLocalClientVersion(unsigned int* puArg1, unsigned int* puArg2, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetLocalClientVersion\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamChangeOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamChangeOfflineStatus\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetOfflineStatus\n");

	SteamClearError(pError);
	pSteamOfflineStatus->eOfflineNow = 0;
	pSteamOfflineStatus->eOfflineNextSession = 0;
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUninstall(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamUninstall\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamWeakVerifyNewValveCDKey()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamWeakVerifyNewValveCDKey\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetEncryptedNewValveCDKey()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetEncryptedNewValveCDKey\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamDecryptDataForThisMachine()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamDecryptDataForThisMachine\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamEncryptDataForThisMachine()
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamEncryptDataForThisMachine\n");
// #endif
	return 1;
}

STEAM_API const char* STEAM_CALL SteamFindServersGetErrorString()
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

STEAM_API int STEAM_CALL SteamFindServersIterateServer(ESteamServerType eSteamServerType, unsigned int uIndex, char* szServerAddress, int iServerAddressChars)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamFindServersIterateServer\n");
// #endif
	if (g_bSteamDll)
	{
		int retval = 1;
		strcpy(szServerAddress, "empty");
		int (*fptr)(int, int, char*, unsigned int);
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamFindServersIterateServer");
		retval = (*fptr)(eSteamServerType, uIndex, szServerAddress, iServerAddressChars);
		if (bLogging) Logger->Write("\t (%u, %u, %s, %u) %u\n", eSteamServerType, uIndex, szServerAddress, iServerAddressChars, retval);
		return retval;
	}

	const char** pServers = NULL;
	size_t iNumServers = 0;

	switch (eSteamServerType)
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

	if (pServers && uIndex < iNumServers)
	{
		strncpy(szServerAddress, pServers[uIndex], iServerAddressChars);
		return 0;
	}

	szServerAddress[0] = '\0';
	return -1;
}

STEAM_API int STEAM_CALL SteamFindServersNumServers(ESteamServerType eSteamServerType)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamFindServersNumServers\n");
// #endif
	if (g_bSteamDll)
	{
		int retval = 1;
		int (*fptr)(unsigned int);
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamFindServersNumServers");
		retval = (*fptr)(eSteamServerType);
		if (bLogging) Logger->Write("\t (%u) %u\n", eSteamServerType, retval);
		return retval;
	}

	switch (eSteamServerType)
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

STEAM_API int STEAM_CALL SteamGetContentServerInfo(unsigned int uAppId, unsigned int* pServerId, unsigned int* pServerIpAddress, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamGetContentServerInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamSetNotificationCallback(SteamNotificationCallback_t pCallbackFunction, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging) Logger->Write("SteamSetNotificationCallback\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamWasBlobRegistryDeleted(int* puWasDeleted, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamWasBlobRegistryDeleted\n");

	SteamClearError(pError);
	*puWasDeleted = 0;
	return 1;
}

int SteamGetCurrentAppId(unsigned int* puAppId, TSteamError *pError)
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

/*
.text:301C5210                 public SteamFindServersNumServers
.text:301C5210 SteamFindServersNumServers proc near    ; CODE XREF: sub_3005C520+5p
.text:301C5210
.text:301C5210 var_14          = dword ptr -14h
.text:301C5210 var_10          = dword ptr -10h
.text:301C5210 var_C           = dword ptr -0Ch
.text:301C5210 var_4           = dword ptr -4
.text:301C5210 arg_0           = dword ptr  8 

	var_4 = 0;
	switch(arg_0)
	{
	case 0x04:
		return 4;
	case 0x03:
		return 1;
	default:
		return 0;
	}
*/