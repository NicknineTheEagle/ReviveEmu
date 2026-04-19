// Steam.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

bool g_bConfigLoaded = false;
bool g_bSteamStartup = false;
CSteamID g_SteamID;
unsigned int g_uAppId = 0;
unsigned int g_uRootAppId = 0;
bool g_bSteamDll = false;
bool g_bSteamClient = false;
bool g_bAllowNonRev = true;
ERevCompatMode g_eCompatMode = REV_COMPAT_NONE;

// TODO: Merge these into CLogFile class.
bool bLogging = false;
bool bLogAcc = false;
bool bLogFS = false;
bool bLogUserId = false;
CLogFile* Logger;

bool g_bSteamFileSystem = false;
bool g_bSteamBlobSystem = false;
bool g_bRawCDR = false;
char g_szGCFPath[MAX_PATH];

char g_szLanguage[MAX_PATH];
char g_szSteamUser[MAX_PATH];
char g_szOLDLanguage[MAX_PATH];
char g_szCDRFile[MAX_PATH];
char g_szBlobFile[MAX_PATH];
char g_szAppIni[MAX_PATH];

HMODULE g_hModule;
HMODULE g_hOrigSteamDll;

std::recursive_mutex g_GlobalMutex;

#ifdef VALIDATOR_DLL
#include "Steam.h"
#include "SteamUserIDValidation.h"
#else
#include "Steam.h"

#include "BlobSystem\CBlobSystem.h"		//Blob

CContentDescriptionRecord* CDR = NULL;

#include "CacheSystem\CCacheSystem.h"	//Cache

CCacheFileSystem *g_CacheManager = NULL;

#include "SteamFilesystem.h"			//Filesystem
#include "SteamApplication.h"			//App Functions
#include "SteamInterface.h"				//Interface
#include "SteamInit.h"					//Initialization
#include "SteamAsyncCallHandling.h"		//Asynchrounous call handling
#include "SteamLogging.h"				//Logging
#include "SteamAccount.h"				//Account
#include "SteamUserIDValidation.h"		//User ID validation
#include "SteamMiniDump.h"				//Minidump
#include "SteamMisc.h"					//Misc

#include <shellapi.h>
#include "registry.h"
#endif

#ifdef _WIN32
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID /*lpvReserved*/)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hinstDLL;
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		g_hModule = NULL;
		break;
	}
	return TRUE;
}
#endif

void InitGlobalVariables()
{
	if (g_bConfigLoaded)
		return;

	g_bConfigLoaded = true;

#ifdef VALIDATOR_DLL
	char szRunFromPath[MAX_PATH];
	V_GetCurrentDirectory(szRunFromPath, MAX_PATH);

	const char* cszLogVar = getenv("REV_LOGGING");
	if (cszLogVar && atoi(cszLogVar))
	{
		bLogging = true;

		char szExePath[MAX_PATH];
		memset(szExePath, 0, sizeof(szExePath));
		ssize_t len = readlink("/proc/self/exe", szExePath, MAX_PATH - 1);
		if (len != -1)
		{
			const char* chProcName = V_GetFileName(szExePath);

			char chLogFile[MAX_PATH];
			strcpy(chLogFile, chProcName);
			strcat(chLogFile, "_REVive.log");
			Logger = new CLogFile(chLogFile);
			Logger->Clear();
			Logger->Write("Logging initialized.\n");
			Logger->Write("Run path initialized to %s\n", szRunFromPath);
			//Logger->Write("Command line: %s\n", chCmdLine);

			bLogUserId = true;
			if (bLogging) Logger->Write("UserID logging initialized.\n");
		}
	}
#else
	int nArgs;
	LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	char szRunFromPath[MAX_PATH];
	V_GetCurrentDirectory(szRunFromPath, MAX_PATH);

	g_uAppId = 0;

	char szEnvBuffer[128];
	int envBufferLen = GetEnvironmentVariableA("SteamAppId", szEnvBuffer, sizeof(szEnvBuffer));
	if (envBufferLen && envBufferLen < sizeof(szEnvBuffer))
	{
		g_uAppId = strtol(szEnvBuffer, NULL, 10);
	}
	else if (FILE* fp = fopen("steam_appid.txt", "rb"))
	{
		char szFileBuffer[256];
		if (fgets(szFileBuffer, sizeof(szFileBuffer), fp))
		{
			g_uAppId = strtol(szFileBuffer, NULL, 10);
		}

		fclose(fp);
	}
	else
	{
		for (int i = 0; i < nArgs; i++)
		{
			if (_wcsicmp(szArgList[i], L"-appid") == 0 && i + 1 < nArgs)
			{
				g_uAppId = wcstol(szArgList[i + 1], NULL, 10);
				break;
			}
		}
	}

	if (g_uAppId != 0) {
		V_sprintf_safe(szEnvBuffer, "%u", g_uAppId);
		SetEnvironmentVariableA("SteamAppId", szEnvBuffer);
	}

	char szSteamDLLPath[MAX_PATH];
	if (GetModuleFileNameA(g_hModule, szSteamDLLPath, MAX_PATH) == 0) {
		ExitProcess(0);
		return;
	}

	char szRelDLLPath[MAX_PATH];
	bool bGotRelPath = V_MakeRelativePath(szSteamDLLPath, szRunFromPath, szRelDLLPath, MAX_PATH);

	// HACK: If Steam.dll is in "bin" subdirectory, use the working dir for our config files.
	if (bGotRelPath && V_stricmp(szRelDLLPath, "bin\\steam.dll") == 0)
	{
		strcpy(szSteamDLLPath, szRunFromPath);
	}
	else
	{
		V_StripFilename(szSteamDLLPath);
	}

	V_AppendSlash(szSteamDLLPath, MAX_PATH);

	char chIniFile[MAX_PATH];
	strcpy(chIniFile, szSteamDLLPath);
	strcat(chIniFile, "rev.ini");
	strcpy(g_szAppIni, szSteamDLLPath);
	strcat(g_szAppIni, "revApps.ini");

	char chCmdLine[MAX_PATH];
	strcpy(chCmdLine, "");
	for (int i = 0; i < nArgs; i++)
	{
		char temp[MAX_PATH];
		wcstombs(temp, szArgList[i], 255);
		strcat(temp, " ");
		strcat(chCmdLine, temp);
	}

	LocalFree(szArgList);

	//
	// Initialize and parse the INI file
	//
	CSimpleIniA Ini;
	Ini.LoadFile(chIniFile);

	if (bLogging = Ini.GetBoolValue("Emulator", "Logging")) // Is logging enabled ?
	{
		char szExePath[MAX_PATH];
		GetModuleFileNameA(NULL, szExePath, MAX_PATH);
		const char* chProcName = V_GetFileName(szExePath);

		char chLogFile[MAX_PATH];
		strcpy(chLogFile, szSteamDLLPath);
		strcat(chLogFile, chProcName);
		strcat(chLogFile, "_REVive.log");
		Logger = new CLogFile(chLogFile);
		Logger->Clear();
		Logger->Write("Logging initialized.\n");
		Logger->Write("Run path initialized to %s\n", szRunFromPath);
		Logger->Write("Command line: %s\n", chCmdLine);
	}
	if (bLogging)
	{
		if (bLogFS = Ini.GetBoolValue("Log", "FileSystem")) // Is FS logging enabled ?
		{
			if (bLogging) Logger->Write("FileSystem logging initialized.\n");
		}
		if (bLogAcc = Ini.GetBoolValue("Log", "Account")) // Is Acc logging enabled ?
		{
			if (bLogging) Logger->Write("Account logging initialized.\n");
		}
		if (bLogUserId = Ini.GetBoolValue("Log", "UserID")) // Is UserID logging enabled ?
		{
			if (bLogging) Logger->Write("UserID logging initialized.\n");
		}
	}

	strcpy(g_szSteamUser, Ini.GetValue("Emulator", "SteamUser", "RevUser"));
	SetEnvironmentVariableA("SteamUser", g_szSteamUser);
	if (bLogging) Logger->Write("Steam User set to %s\n", g_szSteamUser);

	strcpy(g_szOLDLanguage, "unset");
	getRegistryU("Software\\Valve\\Steam", "Language", g_szOLDLanguage, MAX_PATH);
	V_strlower(g_szOLDLanguage);

	if (const char* CheckLang = Ini.GetValue("Emulator", "Language"))
	{
		strcpy(g_szLanguage, CheckLang);
		if (strcmp(g_szOLDLanguage, "unset") == 0)
		{
			strcpy(g_szOLDLanguage, "English");
		}
	}
	else
	{
		if (strcmp(g_szOLDLanguage, "unset") == 0)
		{
			strcpy(g_szOLDLanguage, "English");
			strcpy(g_szLanguage, g_szOLDLanguage);
		}
		else
		{
			strcpy(g_szLanguage, g_szOLDLanguage);
		}
	}

	V_strlower(g_szLanguage);

	if (bLogging) Logger->Write("Steam language initialized (%s)\n", g_szLanguage);

	if (g_bSteamFileSystem = Ini.GetBoolValue("Emulator", "CacheEnabled"))
	{
		strcpy(g_szGCFPath, Ini.GetValue("Emulator", "CachePath", ""));

		char szDefaultCDRFile[MAX_PATH];
		strcpy(szDefaultCDRFile, szSteamDLLPath);
		strcat(szDefaultCDRFile, "cdr.bin");
		strcpy(g_szCDRFile, Ini.GetValue("Emulator", "CDRPath", szDefaultCDRFile));

		strcpy(g_szBlobFile, szSteamDLLPath);
		strcat(g_szBlobFile, "ClientRegistry.blob");

		struct _stat filestat;
		if (_stat(g_szCDRFile, &filestat) == 0)
		{
			if (bLogging) Logger->Write("Cache support initialized via %s\n", g_szCDRFile);
			g_bSteamFileSystem = true;
			g_bSteamBlobSystem = true;
			g_bRawCDR = true;
		}
		else if (_stat(g_szBlobFile, &filestat) == 0)
		{
			if (bLogging) Logger->Write("Cache support initialized via %s\n", g_szBlobFile);
			g_bSteamFileSystem = true;
			g_bSteamBlobSystem = true;
		}
		else
		{
			g_bSteamBlobSystem = false;

			if (_stat(g_szAppIni, &filestat) == 0)
			{
				if (bLogging) Logger->Write("Cache support initialized via revApps.Ini\n");
				g_bSteamFileSystem = true;
			}
			else
			{
				g_bSteamFileSystem = false;
				if (bLogging) Logger->Write("Cache support was not initialized. No ClientRegistry.Blob and No RevSteamApps.ini were found!\n");
			}
		}

		if (g_bSteamFileSystem == true && g_szGCFPath[0] == '\0')
		{
			g_bSteamFileSystem = false;
			if (bLogging) Logger->Write("Cache support was not enabled as no valid GCF path was specified! Using extracted content only!\n");
		}
	}
	else
	{
		if (bLogging) Logger->Write("Cache support was not enabled. Using extracted content only!\n");
	}

	char szOrigSteamDLLPath[MAX_PATH] = "";
	if (const char* SteamDll = Ini.GetValue("Emulator", "SteamDll"))
	{
		strcpy(szOrigSteamDLLPath, SteamDll);
		g_bSteamDll = true;
	}
	if (bool ForceRevClient = Ini.GetBoolValue("Emulator", "ForceRevClient")) // Is other client emu allowed ?
	{
		g_bAllowNonRev = false;
		if (bLogging) Logger->Write("Non-REVive clients will not be allowed to join the server.\n");
	}
	if (g_bSteamDll) // is Original Steam DLL set ?
	{
		g_hOrigSteamDll = LoadLibraryA(szOrigSteamDLLPath);
		if (!g_hOrigSteamDll)
		{
			char szErrMsg[255];
			V_sprintf_safe(szErrMsg, "Unable to load %s\nPlease edit or comment out SteamDll value in rev.ini", szOrigSteamDLLPath);
			MessageBoxA(0, szErrMsg, "Error", 0);
			ExitProcess(1);
		}
		if (bLogging) Logger->Write("-- Original Steam.dll set: %s (0x%p)\n", szOrigSteamDLLPath, g_hOrigSteamDll);
	}

	if (const char* CompatMode = Ini.GetValue("Emulator", "CompatibilityMode"))
	{
		if (V_stricmp(CompatMode, "2003") == 0)
		{
			g_eCompatMode = REV_COMPAT_2003;
		}
	}

	//
	// Set the registry values required for steamclient.dll to be loaded
	//
	if (g_bSteamClient = Ini.GetBoolValue("Emulator", "SteamClient")) // Should we enable steamclient loading ?
	{
		char chClientPath[MAX_PATH];
		strcpy(chClientPath, szSteamDLLPath);

		char szExePath[MAX_PATH];
		GetModuleFileNameA(NULL, szExePath, MAX_PATH);
		const char* chProcName = V_GetFileName(szExePath);

		if (!V_stricmp(chProcName, "hlds.exe") || !V_stricmp(chProcName, "hl.exe"))
			strcat(chClientPath, "steamclient.dll");
		else if (!V_stricmp(chProcName, "srcds.exe") || !V_stricmp(chProcName, "hl2.exe") || !V_stricmp(chProcName, "sdklauncher.exe"))
			strcat(chClientPath, "bin\\steamclient.dll");
		else
			strcat(chClientPath, "steamclient.dll");

		if (bLogging) Logger->Write("-- Using Steam Client: %s\n", chClientPath);
		setRegistry("Software\\Valve\\Steam\\ActiveProcess", "pid", GetCurrentProcessId());
		setRegistry("Software\\Valve\\Steam\\ActiveProcess", "SteamClientDll", chClientPath);
	}

	//
	// Initialize the unique User ID used to authenticate with game server
	//
	DWORD serialNumber;
	GetVolumeInformationA(NULL, NULL, NULL, &serialNumber, NULL, NULL, NULL, NULL);
	g_SteamID.Set(serialNumber, k_EUniversePublic, k_EAccountTypeIndividual);
#endif
}

SteamHandle_t NewSteamHandle()
{
	static uint32 retval = 0;
	retval++;
	if (retval == 0) retval++;
	return retval;
}
