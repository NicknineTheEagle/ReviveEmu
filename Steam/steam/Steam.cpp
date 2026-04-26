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

char g_szLanguage[32];
char g_szSteamUser[32];
char g_szOLDLanguage[32];
char g_szCDRFile[MAX_PATH];
char g_szBlobFile[MAX_PATH];
char g_szAppIni[MAX_PATH];

#define REV_MAX_ARGS 256

char* g_argv[REV_MAX_ARGS];
int g_argc = 0;

HMODULE g_hModule;
HMODULE g_hOrigSteamDll;

std::recursive_mutex g_GlobalMutex;

#ifdef VALIDATOR_DLL
#include "Steam.h"
#include "SteamUserIDValidation.h"
#else
#include "Steam.h"

#include "BlobSystem/CBlobSystem.h"		//Blob

CContentDescriptionRecord* CDR = NULL;

#include "CacheSystem/CCacheSystem.h"	//Cache

CCacheFileSystem* g_CacheManager = NULL;

#include "SteamFilesystem.h"			//Filesystem
#include "SteamApplication.h"			//App Functions
#include "SteamInterface.h"				//Interface
#include "SteamInit.h"					//Initialization
#include "SteamAsyncCallHandling.h"		//Asynchrounous call handling
#include "SteamLogging.h"				//Logging
#include "SteamAccount.h"				//Account
#include "SteamUserIDValidation.h"		//User ID validation
#ifdef _WIN32
#include "SteamMiniDump.h"				//Minidump
#endif
#include "SteamMisc.h"					//Misc
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

SteamHandle_t NewSteamHandle()
{
	static uint32 retval = 0;
	retval++;
	if (retval == 0) retval++;
	return retval;
}

void RevError(const char* cszError)
{
#ifdef _WIN32
	MessageBoxA(NULL, "REVive Error", cszError, MB_OK);
	ExitProcess(1);
#else
	printf("%s\n", cszError);
	_exit(1);
#endif
}

bool RevGetEnvVar(const char* cszVar, char* szOut, unsigned int nOutSize)
{
#ifdef _WIN32
	DWORD dwBufferLen = GetEnvironmentVariableA(cszVar, szOut, nOutSize);
	if (dwBufferLen != 0 && dwBufferLen <= nOutSize)
	{
		return true;
	}
#else
	const char* cszValue = getenv(cszVar);
	if (cszValue)
	{
		V_strncpy(szOut, cszValue, nOutSize);
		return true;
	}
#endif

	return false;
}

void RevSetEnvVar(const char* cszVar, const char* cszValue)
{
#ifdef _WIN32
	SetEnvironmentVariableA(cszVar, cszValue);
#else
	setenv(cszVar, cszValue, 1);
#endif
}

#ifdef _OSX
// Forward declare this rather than including crt_externs.h as not all SDKs provide it.
extern "C" char*** _NSGetArgv(void);
extern "C" int* _NSGetArgc(void);
#endif

void RevCreateCmdLine()
{
	memset(g_argv, 0, sizeof(g_argv));
	g_argc = 0;

#if defined(_WIN32)
	int nArgs;
	LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	int i;
	for (i = 0; i < nArgs && i < REV_MAX_ARGS; i++)
	{
		int len = WideCharToMultiByte(CP_UTF8, 0, szArgList[i], -1, NULL, 0, NULL, NULL);
		if (!len)
		{
			g_argv[i] = V_strdup("");
			continue;
		}
		g_argv[i] = new char[len];
		WideCharToMultiByte(CP_UTF8, 0, szArgList[i], -1, g_argv[i], len, NULL, NULL);
	}
	g_argc = i;
	LocalFree(szArgList);
#elif defined(_OSX)
	char** argv = *_NSGetArgv();
	int argc = *_NSGetArgc();
	int i;
	for (i = 0; i < argc && i < REV_MAX_ARGS; i++)
	{
		g_argv[i] = V_strdup(argv[i]);
	}
	g_argc = i;
#elif defined(_LINUX)
	FILE* f = fopen("/proc/self/cmdline", "rb");
	if (!f)
		return;

	char buf[4096];
	size_t filelen = fread(buf, 1, sizeof(buf), f);
	fclose(f);

	size_t pos = 0;
	while (pos < filelen && g_argc < REV_MAX_ARGS)
	{
		char* szArg = &buf[pos];
		g_argv[g_argc] = V_strdup(szArg);
		pos += strlen(szArg) + 1;
		g_argc++;
	}
#endif
}

void RevDestroyCmdLine()
{
	for (int i = 0; i < g_argc; i++)
	{
		delete[] g_argv[i];
		g_argv[i] = NULL;
	}
	g_argc = 0;
}

bool RevGetDllPath(char* szOut, unsigned int nOutSize)
{
#ifdef _WIN32
	DWORD dwBufferLen = GetModuleFileNameA(g_hModule, szOut, nOutSize);
	if (dwBufferLen != 0 && dwBufferLen <= nOutSize)
	{
		return true;
	}
#else
	Dl_info dlinfo;
	if (dladdr((void*)RevGetDllPath, &dlinfo))
	{
		V_strncpy(szOut, dlinfo.dli_fname, nOutSize);
		return true;
	}
#endif

	return false;
}

void RevInitialize(const char* cszInitSource)
{
	if (g_bConfigLoaded)
		return;

	g_bConfigLoaded = true;

	char szRunFromPath[MAX_PATH];
	V_GetCurrentDirectory(szRunFromPath, MAX_PATH);

	RevCreateCmdLine();

	g_uAppId = 0;

	char szEnvBuffer[128];
	if (RevGetEnvVar("SteamAppId", szEnvBuffer, sizeof(szEnvBuffer)))
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
		for (int i = 0; i < g_argc; i++)
		{
			if (V_stricmp(g_argv[i], "-appid") == 0 && i + 1 < g_argc)
			{
				g_uAppId = strtol(g_argv[i + 1], NULL, 10);
				break;
			}
		}
	}

	if (g_uAppId != 0) {
		V_sprintf_safe(szEnvBuffer, "%u", g_uAppId);
		RevSetEnvVar("SteamAppId", szEnvBuffer);
	}

	char szSteamDLLPath[MAX_PATH];
	if (!RevGetDllPath(szSteamDLLPath, MAX_PATH))
	{
		RevError("Unable to get REVive library path.");
	}

	V_StripFilename(szSteamDLLPath);

	// Attempt to load rev.ini from dll dir first.
	char szIniFile[MAX_PATH];
	V_ComposeFileName(szSteamDLLPath, "rev.ini", szIniFile, MAX_PATH);
	struct _stat filestat;
	if (_stat(szIniFile, &filestat) != 0)
	{
		// Then try the current dir.
		V_ComposeFileName(szRunFromPath, "rev.ini", szIniFile, MAX_PATH);
	}

	char szIniDir[MAX_PATH];
	V_ExtractFilePath(szIniFile, szIniDir, MAX_PATH);
	V_ComposeFileName(szIniDir, "revApps.ini", g_szAppIni, MAX_PATH);

	// Initialize and parse the INI file
	CSimpleIniA Ini;
	Ini.LoadFile(szIniFile);

	if (bLogging = Ini.GetBoolValue("Emulator", "Logging")) // Is logging enabled ?
	{
		char szLogPath[MAX_PATH];
		V_ComposeFileName(szRunFromPath, "rev.log", szLogPath, MAX_PATH);
		Logger = new CLogFile(szLogPath);;
		Logger->Write("Logging initialized.\n");
		Logger->Write("DLL initialized from %s\n", cszInitSource);
		Logger->Write("Run path initialized to %s\n", szRunFromPath);

		char szCmdLine[1024] = "";
		for (int i = 0; i < g_argc; i++)
		{
			V_strcat_safe(szCmdLine, g_argv[i]);
			if (i + 1 < g_argc)
			{
				V_strcat_safe(szCmdLine, " ");
			}
		}

		Logger->Write("Command line: %s\n", szCmdLine);
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

#ifndef VALIDATOR_DLL
	V_strcpy_safe(g_szSteamUser, Ini.GetValue("Emulator", "SteamUser", "RevUser"));
	RevSetEnvVar("SteamUser", g_szSteamUser);
	if (bLogging) Logger->Write("Steam User set to %s\n", g_szSteamUser);

#ifdef _WIN32
	V_strcpy_safe(g_szOLDLanguage, "unset");
	getRegistryU("Software\\Valve\\Steam", "Language", g_szOLDLanguage, sizeof(g_szOLDLanguage));
	V_strlower(g_szOLDLanguage);

	if (const char* CheckLang = Ini.GetValue("Emulator", "Language"))
	{
		V_strcpy_safe(g_szLanguage, CheckLang);
		if (V_strcmp(g_szOLDLanguage, "unset") == 0)
		{
			V_strcpy_safe(g_szOLDLanguage, "English");
		}
	}
	else
	{
		if (V_strcmp(g_szOLDLanguage, "unset") == 0)
		{
			V_strcpy_safe(g_szOLDLanguage, "English");
			V_strcpy_safe(g_szLanguage, g_szOLDLanguage);
		}
		else
		{
			V_strcpy_safe(g_szLanguage, g_szOLDLanguage);
		}
	}

	V_strlower(g_szLanguage);
#else
	// When Valve ported Source to Mac, they've also made it so it doesn't get the language from the registry anymore,
	// so we can simplify this whole thing.
	V_strcpy_safe(g_szLanguage, Ini.GetValue("Emulator", "Language", "English"));
	V_strlower(g_szLanguage);
#endif

	if (bLogging) Logger->Write("Steam language initialized (%s)\n", g_szLanguage);

	if (g_bSteamFileSystem = Ini.GetBoolValue("Emulator", "CacheEnabled"))
	{
		V_strcpy_safe(g_szGCFPath, Ini.GetValue("Emulator", "CachePath", ""));

		V_ComposeFileName(szIniDir, "cdr.bin", g_szCDRFile, MAX_PATH);
		V_strcpy_safe(g_szCDRFile, Ini.GetValue("Emulator", "CDRPath", g_szCDRFile));

		V_ComposeFileName(szIniDir, "ClientRegistry.blob", g_szBlobFile, MAX_PATH);

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
#endif

	char szOrigSteamDLLPath[MAX_PATH] = "";
	if (const char* SteamDll = Ini.GetValue("Emulator", "SteamDll"))
	{
		V_strcpy_safe(szOrigSteamDLLPath, SteamDll);
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
			char szErrMsg[512];
			V_sprintf_safe(szErrMsg, "Unable to load %s! Please edit or comment out SteamDll value in rev.ini.", szOrigSteamDLLPath);
			RevError(szErrMsg);
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

#ifdef _WIN32
	// Set the registry values required for steamclient.dll to be loaded
	if (g_bSteamClient = Ini.GetBoolValue("Emulator", "SteamClient")) // Should we enable steamclient loading ?
	{
		char chClientPath[MAX_PATH];
		V_strcpy_safe(chClientPath, szRunFromPath);

		char szExePath[MAX_PATH];
		GetModuleFileNameA(NULL, szExePath, MAX_PATH);
		const char* chProcName = V_GetFileName(szExePath);

		if (!V_stricmp(chProcName, "hlds.exe") || !V_stricmp(chProcName, "hl.exe"))
			V_strcat_safe(chClientPath, "steamclient.dll");
		else if (!V_stricmp(chProcName, "srcds.exe") || !V_stricmp(chProcName, "hl2.exe") || !V_stricmp(chProcName, "sdklauncher.exe"))
			V_strcat_safe(chClientPath, "bin\\steamclient.dll");
		else
			V_strcat_safe(chClientPath, "steamclient.dll");

		if (bLogging) Logger->Write("-- Using Steam Client: %s\n", chClientPath);
		setRegistry("Software\\Valve\\Steam\\ActiveProcess", "pid", GetCurrentProcessId());
		setRegistry("Software\\Valve\\Steam\\ActiveProcess", "SteamClientDll", chClientPath);
	}
#endif
	
#ifndef VALIDATOR_DLL
	// Initialize the unique User ID used to authenticate with game server
#ifdef _WIN32
	DWORD serialNumber;
	GetVolumeInformationA(NULL, NULL, NULL, &serialNumber, NULL, NULL, NULL, NULL);
	g_SteamID.Set(serialNumber, k_EUniversePublic, k_EAccountTypeIndividual);
#else
	// There are no Linux or Mac clients that actually use Steam2 auth, so there's no need to set this.
	g_SteamID.Clear();
#endif
#endif

	RevDestroyCmdLine();
}
