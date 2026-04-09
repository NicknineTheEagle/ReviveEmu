// Steam.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

bool g_bSteamStartup = false;
CSteamID g_SteamID;
unsigned int g_uAppId = 0;
unsigned int g_uRootAppId = 0;
bool g_bSteamDll = false;
char g_szOrigSteamDll[MAX_PATH];
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
char g_szGCFPath[MAX_PATH * 5];
std::vector<const char*> g_CacheLocations;

char g_szLanguage[MAX_PATH];
char g_szSteamUser[MAX_PATH];
char g_szOLDLanguage[MAX_PATH];
char g_szCDRFile[MAX_PATH];
char g_szBlobFile[MAX_PATH];
char g_szAppIni[MAX_PATH];

HMODULE g_hModule;

#include "CriticalSection.h"

CRITICAL_SECTION g_CriticalSection;
static CInitCriticalSection s_InitCriticalSection(&g_CriticalSection);

#include "Steam.h"

#include "BlobSystem\CBlobSystem.h"		//Blob

CContentDescriptionRecord* CDR = NULL;

#include "CacheSystem\CCacheSystem.h"	//Cache

CCacheFileSystem *g_CacheManager;

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


static bool inArgs(LPWSTR szArg, LPWSTR* cszArgList, int nArgs)
{
	for (int i = 0; i < nArgs; i++)
	{
		if (_wcsicmp(cszArgList[i], szArg) == 0)
		{
			return true;
		}
	}
	return false;
}

void InitGlobalVaribles();

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			strcpy(g_szOLDLanguage,"unset");
			getRegistryU("Software\\Valve\\Steam","Language",g_szOLDLanguage,MAX_PATH); 
			_strlwr(g_szOLDLanguage);
			g_hModule = hModule;
			InitGlobalVaribles();
			if (bLogging)Logger->Write("DllMain: DLL_PROCESS_ATTACH\n");
			break;

		case DLL_THREAD_ATTACH:
		
			if (bLogging)Logger->Write("DllMain: DLL_THREAD_ATTACH\n");
			break;
	
		case DLL_THREAD_DETACH:
		
			if (bLogging)Logger->Write("DllMain: DLL_THREAD_DETACH\n");
			break;
	
		case DLL_PROCESS_DETACH:
		
			if (bLogging)Logger->Write("DllMain: DLL_PROCESS_DETACH\n");
			setRegistry("Software\\Valve\\Steam","Language",g_szOLDLanguage); 
			g_hModule = NULL;
			break;
	
	}
	return TRUE;
}

void InitGlobalVaribles()
{
	int nArgs;
	LPWSTR* szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	char szRunFromPath[MAX_PATH];
	_getcwd(szRunFromPath, MAX_PATH);

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
	else if (inArgs(L"-appid", szArgList, nArgs))
	{
		for (int i = 0; i < nArgs; i++)
		{
			if (_wcsicmp(szArgList[i], L"-appid") == 0)
			{
				g_uAppId = wcstol(szArgList[i + 1], NULL, 10);
			}
		}
	}

		if (!g_uAppId)
		{
			char szExePath[MAX_PATH];
			GetModuleFileNameA(NULL, szExePath, MAX_PATH);
			const char *chProcName = V_GetFileName(szExePath);

			if (inArgs(L"-game", szArgList, nArgs)) 
			{
				char szGameDir[MAX_PATH];

				for (int j = 0; j < nArgs; j++)
				{
					if (_wcsicmp(szArgList[j], L"-game") == 0)
					{
						wcstombs(szGameDir, szArgList[j + 1], MAX_PATH);
					}
				}

				if (!_stricmp(szGameDir, "cstrike"))
				{
					if (!_stricmp(chProcName, "hl.exe")) g_uAppId = 10;
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 240;
				}
				else if (!_stricmp(szGameDir, "dod"))
				{
					if (!_stricmp(chProcName, "hl.exe")) g_uAppId = 30;
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 300;
				}
				else if (!_stricmp(szGameDir, "garrysmod"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 4000;
				}
				else if (!_stricmp(szGameDir, "hl2mp"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 320;
				}
				else if (!_stricmp(szGameDir, "tf"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 440;
				}
				else if (!_stricmp(szGameDir, "episodic"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 380;
				}
				else if (!_stricmp(szGameDir, "ep2"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 420;
				}
				else if (!_stricmp(szGameDir, "portal"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 400;
				}
				else if (!_stricmp(szGameDir, "lostcoast"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) g_uAppId = 340;
				}
				else if (!_stricmp(szGameDir, "launcher"))
				{
					if (!_stricmp(chProcName, "SourceSDK.exe")) g_uAppId = 211;
				}
				else
					g_uAppId = 0;
			}
			else if(!_stricmp(chProcName, "hl.exe")) g_uAppId = 70;
			else if(!_stricmp(chProcName, "hl2.exe")) g_uAppId = 220;
		}

			char chLogFile[MAX_PATH];
			char chIniFile[MAX_PATH];
			char chClientPath[MAX_PATH];
	        char szSteamDLLPath[MAX_PATH];

			if(GetModuleFileNameA(g_hModule, szSteamDLLPath, MAX_PATH))
			{
				char szRelDLLPath[MAX_PATH];
				bool bGotRelPath = V_MakeRelativePath(szSteamDLLPath, szRunFromPath, szRelDLLPath, MAX_PATH);

				// HACK: If Steam.dll is in "bin" subdirectory, use the working dir for our config files.
				if (bGotRelPath && _stricmp(szRelDLLPath, "bin\\steam.dll") == 0)
				{
					strcpy(szSteamDLLPath, szRunFromPath);
				}
				else
				{
					V_StripFilename(szSteamDLLPath);
				}

				V_AppendSlash(szSteamDLLPath, MAX_PATH);

				strcpy(chIniFile, szSteamDLLPath);
				strcat(chIniFile, "rev.ini");
				strcpy(g_szAppIni, szSteamDLLPath);
				strcat(g_szAppIni, "revApps.ini");

				char chCmdLine[MAX_PATH];
				char temp[MAX_PATH];
				strcpy(chCmdLine, "");
		        for (int i = 0; i < nArgs; i++)
		        {
			        wcstombs(temp, szArgList[i], 255);
			        strcat(temp, " ");
			        strcat(chCmdLine, temp);
		        }


				//
				// Initialize and parse the INI file
				//
				CIniFile* Ini = new CIniFile(chIniFile);

				if(char* Logging = Ini->IniReadValue("Emulator", "Logging")) // Is logging enabled ?
				{
					if (_stricmp(Logging, "True") == 0) 
					{
						bLogging = true;

						char szExePath[MAX_PATH];
						GetModuleFileNameA(NULL, szExePath, MAX_PATH);
						const char* chProcName = V_GetFileName(szExePath);

						strcpy(chLogFile, szSteamDLLPath);
						strcat(chLogFile, chProcName);
						strcat(chLogFile, "_REVive.log");
						Logger = new CLogFile(chLogFile);
						Logger->Clear();
						Logger->Write("Logging initialized.\n");
						Logger->Write("Run path initialized to %s\n",szRunFromPath);
						Logger->Write("Command line: %s\n", chCmdLine);

					} 
					delete[] Logging;
				}
				if (bLogging)
				{
					if(char* Logging = Ini->IniReadValue("Log", "FileSystem")) // Is FS logging enabled ?
					{
						if (_stricmp(Logging, "True") == 0) 
						{
							bLogFS = true;
							if (bLogging) Logger->Write("FileSystem logging initialized.\n");
						} 
						delete[] Logging;
					}
					if(char* Logging = Ini->IniReadValue("Log", "Account")) // Is Acc logging enabled ?
					{
						if (_stricmp(Logging, "True") == 0) 
						{
							bLogAcc = true;
							if (bLogging) Logger->Write("Account logging initialized.\n");
						} 
						delete[] Logging;
					}
					if(char* Logging = Ini->IniReadValue("Log", "UserID")) // Is UserID logging enabled ?
					{
						if (_stricmp(Logging, "True") == 0) 
						{
							bLogUserId = true;
							if (bLogging) Logger->Write("UserID logging initialized.\n");
						} 
						delete[] Logging;
					}
				}

				/*
				if (bLogging)
				{
					Logger->Write("Processing passed arguments:\n");
					char * szTEMP = new char[MAX_PATH];
					for(int j=0; j<nArgs; j++) 
					{
						wcstombs(szTEMP, szArglist[j], MAX_PATH);
						Logger->Write("\tArg[%u]: %s\n",j, szTEMP);

					}
					delete [] szTEMP;

				}
				*/

				if(char* SteamUSR = Ini->IniReadValue("Emulator", "SteamUser"))
				{
					if (strlen(SteamUSR)>0)
					{
						strcpy(g_szSteamUser,SteamUSR);
					}
					else
					{
						strcpy(g_szSteamUser,"RevUser");
					}
					delete[] SteamUSR;
				}
				else
				{
					strcpy(g_szSteamUser,"RevUser");
				}

				SetEnvironmentVariableA("SteamUser", g_szSteamUser);
				if (bLogging) Logger->Write("Steam User set to %s\n",g_szSteamUser);

				if(char* CheckLang = Ini->IniReadValue("Emulator", "Language")) 
				{
					strcpy(g_szLanguage,CheckLang);
					if (strcmp(g_szOLDLanguage,"unset") == 0)
					{
						strcpy(g_szOLDLanguage,"English");
					}

					delete[] CheckLang;
				}
				else
				{
					if (strcmp(g_szOLDLanguage,"unset") == 0)
					{
						strcpy(g_szOLDLanguage,"English");
						strcpy(g_szLanguage,g_szOLDLanguage);
					}
					else
					{
						strcpy(g_szLanguage,g_szOLDLanguage);
					}
				}

				_strlwr(g_szLanguage);

				setRegistry("Software\\Valve\\Steam","Language",g_szLanguage); 

				if (bLogging) Logger->Write("Steam language initialized (%s)\n", g_szLanguage);

				if(char* GCFEnable = Ini->IniReadValue("Emulator", "CacheEnabled")) 
				{
					if (_stricmp(GCFEnable, "True") == 0) 
					{
						if (char* Path = Ini->IniReadValue("Emulator", "CachePath"))
						{
							strcpy(g_szGCFPath, Path);
							delete[] Path;
						}
						else
						{
							g_szGCFPath[0] = '\0';
						}

						if (char* Path = Ini->IniReadValue("Emulator", "CDRPath"))
				        {
					        strcpy(g_szCDRFile, Path);
					        delete[] Path;
				        }
				        else
				        {
					        strcpy(g_szCDRFile, szSteamDLLPath);
					        strcat(g_szCDRFile, "cdr.bin");
				        }

				        strcpy(g_szBlobFile, szSteamDLLPath);
				        strcat(g_szBlobFile, "ClientRegistry.Blob");

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
							if (bLogging) Logger->Write("Cache support initialized via %s\n",g_szBlobFile);
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

						if (g_bSteamFileSystem == true)
						{
							char *pch = strtok(g_szGCFPath, " ");
							while (pch != NULL)
							{
								g_CacheLocations.push_back(pch);
								pch = strtok(NULL, " ");
							}

							if (g_CacheLocations.size() > 0)
							{
								if (bLogging) Logger->Write("-- Multiple cache locations detected\n");
							}
							else
							{
								g_bSteamFileSystem = false;
								if (bLogging) Logger->Write("Cache support was not enabled as no valid GCF path was specified! Using extracted content only!\n");
							}
						}
					}
					else
					{
						g_bSteamFileSystem = false;
						if (bLogging) Logger->Write("Cache support was not enabled. Using extracted content only!\n");
					}
					delete[] GCFEnable;
				}


				if(char* SteamDll = Ini->IniReadValue("Emulator", "SteamDll"))
				{
					strcpy(g_szOrigSteamDll, SteamDll);
					g_bSteamDll = true;
					delete[] SteamDll;
				}
				if(char* Misc = Ini->IniReadValue("Emulator", "ForceRevClient")) // Is other client emu allowed ?
				{
					if (_stricmp(Misc, "True") == 0) 
					{
						g_bAllowNonRev = false;
						if (bLogging) Logger->Write("Non-REVive clients will not be allowed to join the server.\n");
					} 
					delete[] Misc;
				}
				if(g_bSteamDll) // is Original Steam DLL set ?
				{
					char buffer[255];
					sprintf(buffer, "%p", LoadLibraryA(g_szOrigSteamDll));
					if (!atoi(buffer)) 
					{
						char szErrMsg[255] = "Unable to load ";
						strcat(szErrMsg, g_szOrigSteamDll);
						strcat(szErrMsg, ". \nPlease edit or comment out SteamDll value in rev.ini");
						MessageBoxA(0, szErrMsg, "Error", 0);
						ExitProcess(1);
					}
					if (bLogging) Logger->Write("-- Original Steam.dll set: %s (0x%s)\n", g_szOrigSteamDll, buffer);
				}

				if (char* CompatMode = Ini->IniReadValue("Emulator", "CompatibilityMode"))
				{
					if (_stricmp(CompatMode, "2003") == 0)
					{
						g_eCompatMode = REV_COMPAT_2003;
					}
					delete[] CompatMode;
				}

		//
		// Set the registry values required for steamclient.dll to be loaded
		//
				if(char* SteamClient = Ini->IniReadValue("Emulator", "SteamClient")) // Should we enable steamclient loading ?
				{
					if (_stricmp(SteamClient, "True") == 0) 
					{
						g_bSteamClient = true;
						strcpy(chClientPath, szSteamDLLPath);

						char szExePath[MAX_PATH];
						GetModuleFileNameA(NULL, szExePath, MAX_PATH);
						const char* chProcName = V_GetFileName(szExePath);
				
						if (!_stricmp(chProcName, "hlds.exe") || !_stricmp(chProcName, "hl.exe")) strcat(chClientPath, "steamclient.dll");
						else if (!_stricmp(chProcName, "srcds.exe") || !_stricmp(chProcName, "hl2.exe") || !_stricmp(chProcName, "sdklauncher.exe")) strcat(chClientPath, "bin\\steamclient.dll");
						else strcat(chClientPath, "steamclient.dll");
						
						if (bLogging) Logger->Write("-- Using Steam Client: %s\n", chClientPath);
						setRegistry("Software\\Valve\\Steam\\ActiveProcess","pid",GetCurrentProcessId()); 
						setRegistry("Software\\Valve\\Steam\\ActiveProcess","SteamClientDll",chClientPath);

						/*HMODULE hSteamApi = GetModuleHandleA("steam_api.dll");
						if (hSteamApi && _stristr(chCmdLine, "left4dead"))
						{
							FARPROC SteamAPI_Init = GetProcAddress(hSteamApi, "SteamAPI_Init");
							SteamAPI_Init();
							if (bLogging) Logger->Write("-- Using Steam Client: %s\n", chClientPath);
						}*/
					} 
					delete[] SteamClient;
				}

				delete Ini;

				//
				// Initialize the unique User ID used to authenticate with game server
				//
				DWORD serialNumber;
				GetVolumeInformationA(NULL, NULL, NULL, &serialNumber, NULL, NULL, NULL, NULL);
				g_SteamID.Set(serialNumber, k_EUniversePublic, k_EAccountTypeIndividual);
			} else {
				ExitProcess(0);
			}

			LocalFree(szArgList);
}

SteamHandle_t NewSteamHandle()
{
	static uint32 retval = 0;
	retval++;
	if (retval == 0) retval++;
	return retval;
}
