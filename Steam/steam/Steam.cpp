// Steam.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "windows.h"

#include "CriticalSection.h"

#include <iostream>
#include <conio.h>
#include <fstream>
#include <sys/stat.h>
#include <map>
#include <MMSystem.h>
#include <io.h>
#include <cstdio>
#include <process.h>
#include <shellapi.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <direct.h>

unsigned int rootAppID = 0;
CLogFile* Logger;
char szSteamDLLPath[MAX_PATH];
BOOL bSteamDll = false;
char szOrigSteamDll[MAX_PATH];
BOOL bLogging = false;
BOOL bLogAcc = false;
BOOL bLogFS = false;
BOOL bLogUserId = false;
BOOL bSteamClient = false;
bool bAllowNonRev = true;

int nArgs;
bool bSteamFileSystem = false;
bool bSteamBlobSystem = false;
bool bRawCDR = false;
std::vector<intptr_t> vecGCF;
unsigned int appid = 0;
char szGCFPath[MAX_PATH * 5];
static std::vector<const char*> CacheLocations;

char szLanguage[MAX_PATH];
char szSteamUser[MAX_PATH];
char szOLDLanguage[MAX_PATH];
char szCDRFile[MAX_PATH];
char szBlobFile[MAX_PATH];
char szAppIni[MAX_PATH];
LPWSTR *szArglist;

HMODULE g_hModule;

CRITICAL_SECTION g_CriticalSection;
static CInitCriticalSection s_InitCriticalSection(&g_CriticalSection);

typedef struct
{
	void* pCache;
	char* FullName;
	//char FullName[MAX_PATH];
	unsigned int Index;
}TGlobalDirectory;

unsigned int GlobalDirectoryTableSize = 0;
unsigned int GlobalIndexCounter = 0;

#include "Steam.h"
#include "RevCommon.h"

#include "BlobSystem\CBlobSystem.h"		//Blob

static CContentDescriptionRecord* CDR = NULL;
std::vector<TGlobalDirectory> GlobalDirectoryTable;
static std::map<uint32_t, TGlobalDirectory> HashTable;

#include "CacheSystem\CCacheSystem.h"	//Cache

static CCacheFileSystem* CacheManager = NULL;
CSteamID g_SteamID;

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

#include "registry.h"


bool inArgs(LPWSTR arg) {
	for(int i=0; i<nArgs; i++) {
		if(_wcsicmp(szArglist[i],arg) == 0) {
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
			strcpy(szOLDLanguage,"unset");
			getRegistryU("Software\\Valve\\Steam","Language",szOLDLanguage,MAX_PATH); 
			_strlwr(szOLDLanguage);
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
			setRegistry("Software\\Valve\\Steam","Language",szOLDLanguage); 
			g_hModule = NULL;
			break;
	
	}
	return TRUE;
}

void InitGlobalVaribles()
{
	szArglist = CommandLineToArgvW(GetCommandLineW(), &nArgs);

	char szRunFromPath[MAX_PATH];
	_getcwd(szRunFromPath, MAX_PATH);

	appid = 0;

	char szEnvBuffer[128];
	int envBufferLen = GetEnvironmentVariableA("SteamAppId", szEnvBuffer, sizeof(szEnvBuffer));
	if (envBufferLen && envBufferLen < sizeof(szEnvBuffer))
	{
		appid = strtol(szEnvBuffer, NULL, 10);
	}
	else if (FILE* fp = fopen("steam_appid.txt", "rb"))
	{
		char szFileBuffer[256];
		if (fgets(szFileBuffer, sizeof(szFileBuffer), fp))
		{
			appid = strtol(szFileBuffer, NULL, 10);
		}

		fclose(fp);
	}
	else if (inArgs(L"-appid"))
	{
		for (int i = 0; i < nArgs; i++)
		{
			if (_wcsicmp(szArglist[i], L"-appid") == 0)
			{
				appid = wcstol(szArglist[i + 1], NULL, 10);
			}
		}
	}

		if (!appid)
		{
			char szExePath[MAX_PATH];
			GetModuleFileNameA(NULL, szExePath, MAX_PATH);
			const char *chProcName = V_GetFileName(szExePath);

			if(inArgs(L"-game")) 
			{
				char szGameDir[MAX_PATH];

				for (int j = 0; j < nArgs; j++)
				{
					if (_wcsicmp(szArglist[j], L"-game") == 0)
					{
						wcstombs(szGameDir, szArglist[j + 1], MAX_PATH);
					}
				}

				if (!_stricmp(szGameDir, "cstrike"))
				{
					if (!_stricmp(chProcName, "hl.exe")) appid = 10;
					if (!_stricmp(chProcName, "hl2.exe")) appid = 240;
				}
				else if (!_stricmp(szGameDir, "dod"))
				{
					if (!_stricmp(chProcName, "hl.exe")) appid = 30;
					if (!_stricmp(chProcName, "hl2.exe")) appid = 300;
				}
				else if (!_stricmp(szGameDir, "garrysmod"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 4000;
				}
				else if (!_stricmp(szGameDir, "hl2mp"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 320;
				}
				else if (!_stricmp(szGameDir, "tf"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 440;
				}
				else if (!_stricmp(szGameDir, "episodic"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 380;
				}
				else if (!_stricmp(szGameDir, "ep2"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 420;
				}
				else if (!_stricmp(szGameDir, "portal"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 400;
				}
				else if (!_stricmp(szGameDir, "lostcoast"))
				{
					if (!_stricmp(chProcName, "hl2.exe")) appid = 340;
				}
				else if (!_stricmp(szGameDir, "launcher"))
				{
					if (!_stricmp(chProcName, "SourceSDK.exe")) appid = 211;
				}
				else
					appid = 0;
			}
			else if(!_stricmp(chProcName, "hl.exe")) appid = 70;
			else if(!_stricmp(chProcName, "hl2.exe")) appid = 220;
		}

			char chLogFile[MAX_PATH];
			char chIniFile[MAX_PATH];
			char chClientPath[MAX_PATH];

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
				strcpy(szAppIni, szSteamDLLPath);
				strcat(szAppIni, "revApps.ini");

						char chCmdLine[MAX_PATH];
						char temp[MAX_PATH];
						strcpy(chCmdLine, "");
						for (int i = 0; i<nArgs; i++)
						{
							wcstombs(temp, szArglist[i], 255);
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
						strcpy(szSteamUser,SteamUSR);
					}
					else
					{
						strcpy(szSteamUser,"RevUser");
					}
					delete[] SteamUSR;
				}
				else
				{
					strcpy(szSteamUser,"RevUser");
				}

				SetEnvironmentVariableA("SteamUser", szSteamUser);
				if (bLogging) Logger->Write("Steam User set to %s\n",szSteamUser);

				if(char* CheckLang = Ini->IniReadValue("Emulator", "Language")) 
				{
					strcpy(szLanguage,CheckLang);
					if (strcmp(szOLDLanguage,"unset") == 0)
					{
						strcpy(szOLDLanguage,"English");
					}

					delete[] CheckLang;
				}
				else
				{
					if (strcmp(szOLDLanguage,"unset") == 0)
					{
						strcpy(szOLDLanguage,"English");
						strcpy(szLanguage,szOLDLanguage);
					}
					else
					{
						strcpy(szLanguage,szOLDLanguage);
					}
				}

				_strlwr(szLanguage);

				setRegistry("Software\\Valve\\Steam","Language",szLanguage); 

				if (bLogging) Logger->Write("Steam language initialized (%s)\n", szLanguage);

				if(char* GCFEnable = Ini->IniReadValue("Emulator", "CacheEnabled")) 
				{
					if (_stricmp(GCFEnable, "True") == 0) 
					{
						if (char* Path = Ini->IniReadValue("Emulator", "CachePath"))
						{
							strcpy(szGCFPath, Path);
							delete[] Path;
						}
						else
						{
							szGCFPath[0] = '\0';
						}

						if (char* Path = Ini->IniReadValue("Emulator", "CDRPath"))
				        {
					        strcpy(szCDRFile, Path);
					        delete[] Path;
				        }
				        else
				        {
					        strcpy(szCDRFile, szSteamDLLPath);
					        strcat(szCDRFile, "cdr.bin");
				        }

				        strcpy(szBlobFile, szSteamDLLPath);
				        strcat(szBlobFile, "ClientRegistry.Blob");

						struct _stat filestat;
				        if (_stat(szCDRFile, &filestat) == 0)
				        {
					        if (bLogging) Logger->Write("Cache support initialized via %s\n", szCDRFile);
					        bSteamFileSystem = true;
					        bSteamBlobSystem = true;
							bRawCDR = true;
						}
						else if (_stat(szBlobFile, &filestat) == 0)
						{		
							if (bLogging) Logger->Write("Cache support initialized via %s\n",szBlobFile);
							bSteamFileSystem = true;
							bSteamBlobSystem = true;
						}
						else
						{
							bSteamBlobSystem = false;

							if (_stat(szAppIni, &filestat) == 0)
							{
								if (bLogging) Logger->Write("Cache support initialized via revApps.Ini\n");
								bSteamFileSystem = true;
							}
							else
							{
								bSteamFileSystem = false;
								if (bLogging) Logger->Write("Cache support was not initialized. No ClientRegistry.Blob and No RevSteamApps.ini were found!\n");
							}
						} 

						if (bSteamFileSystem == true)
						{
							char *pch = strtok(szGCFPath, " ");
							while (pch != NULL)
							{
								CacheLocations.push_back(pch);
								pch = strtok(NULL, " ");
							}

							if (CacheLocations.size() > 0)
							{
								if (bLogging) Logger->Write("-- Multiple cache locations detected\n");
							}
							else
							{
								bSteamFileSystem = false;
								if (bLogging) Logger->Write("Cache support was not enabled as no valid GCF path was specified! Using extracted content only!\n");
							}
						}
					}
					else
					{
						bSteamFileSystem = false;
						if (bLogging) Logger->Write("Cache support was not enabled. Using extracted content only!\n");
					}
					delete[] GCFEnable;
				}


				if(char* SteamDll = Ini->IniReadValue("Emulator", "SteamDll"))
				{
					strcpy(szOrigSteamDll, SteamDll);
					bSteamDll = TRUE;
					delete[] SteamDll;
				}
				if(char* Misc = Ini->IniReadValue("Emulator", "ForceRevClient")) // Is other client emu allowed ?
				{
					if (_stricmp(Misc, "True") == 0) 
					{
						bAllowNonRev = false;
						if (bLogging) Logger->Write("Non-REVive clients will not be allowed to join the server.\n");
					} 
					delete[] Misc;
				}
				if(bSteamDll == TRUE) // is Original Steam DLL set ?
				{
					char buffer[255];
					sprintf(buffer, "%x", (int)LoadLibraryA(szOrigSteamDll));
					if (!atoi(buffer)) 
					{
						char szErrMsg[255] = "Unable to load ";
						strcat(szErrMsg, szOrigSteamDll);
						strcat(szErrMsg, ". \nPlease edit or comment out SteamDll value in rev.ini");
						MessageBoxA(0, szErrMsg, "Error", 0);
						ExitProcess(1);
					}
					if (bLogging) Logger->Write("-- Original Steam.dll set: %s (0x%s)\n", szOrigSteamDll, buffer);
				}

		//
		// Set the registry values required for steamclient.dll to be loaded
		//
				if(char* SteamClient = Ini->IniReadValue("Emulator", "SteamClient")) // Should we enable steamclient loading ?
				{
					if (_stricmp(SteamClient, "True") == 0) 
					{
						bSteamClient = true;
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
				return;
			}
			
			ExitProcess(0);

}
