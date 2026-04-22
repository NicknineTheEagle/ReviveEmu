#pragma once

extern bool g_bSteamStartup;
extern unsigned int g_uRootAppId;

extern TFileInCacheHandle* NewSteamFileHandle();

const char* GetNameById(unsigned int id)
{
	for (CAppRecord* pRecord : CDR->ApplicationRecords)
	{
		if (id == pRecord->AppId)
		{
			return pRecord->InstallDirName;
		}
	}
	return NULL;
}

int GetFiles(const char* szSource, const char* szDest, int mode, const char* szMount, bool OverwriteConfigFiles)
{
	TManifestEntriesInCache* TestFile;
	//char FileToOpen[MAX_PATH];
	char szFileName[MAX_PATH];
	int retval = 0;
	int iBitResult;
	bool bExtract;

	char szCWD[MAX_PATH];

	strcpy(szCWD, szDest);

#if 0
	if(V_GetCurrentDirectory(szCWD, sizeof(szCWD)))
	{
		V_strlower(szCWD);
	}
#endif

	intptr_t cHandle = g_CacheManager->MountCache(szSource, szMount);
	if (cHandle)
	{
		CCache* CacheFile = g_CacheManager->Caches[0]->hCacheFile;
		for (unsigned int x = 1; x < CacheFile->Manifest->Header->ItemCount; x++)
		{
			TestFile = &(CacheFile->DirectoryTable[x]);
			if (TestFile->Type != 0)
			{
				bExtract = false;

				if (mode == 2)
				{
					iBitResult = TestFile->Type & 0x00000002; // Test for MFP flag may need locked as well: 0x00000008
					if (iBitResult != 0)
					{
						bExtract = true;
					}
				}
				else if (mode == 1)
				{
					bExtract = true;
				}

				if (bExtract == true)
				{
					TFileInCacheHandle* hFile = NewSteamFileHandle();
					hFile->FileInCache = TestFile;
					strcpy(hFile->mode, "rb");

					iBitResult = TestFile->Type & 0x00000001;

					if (iBitResult != 0)
					{
						if (OverwriteConfigFiles == false)
						{
							V_ComposeFileName(szCWD, TestFile->FullName, szFileName, MAX_PATH);

							struct _stat buf;
							retval = _stat(szFileName, &buf);
						}
						else
						{
							retval = -1;
						}

						if (!retval)
						{
							retval = g_CacheManager->CacheExtractFile(hFile, szDest);
						}
					}
					else
					{
						retval = g_CacheManager->CacheExtractFile(hFile, szDest);
					}

					g_CacheManager->CacheCloseFile(hFile);
				}
			}
		}

		g_CacheManager->UnmountCache(cHandle);

		return 1;
	}

	return 0;
}

STEAM_API int SteamEnumerateApp(unsigned int uAppID, TSteamApp* pApp, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamEnumerateApp (%u)\n", uAppID);

	if (g_bSteamBlobSystem)
	{
		unsigned int uAppRecord = GetAppRecordID(uAppID);

		if (uAppRecord != UINT_MAX)
		{
			strcpy(pApp->szName, CDR->ApplicationRecords[uAppRecord]->Name);
			pApp->uMaxNameChars = 255;

			//pApp->uLatestVersionId = CDR->ApplicationRecords[uAppRecord]->VersionsRecord[CDR->ApplicationRecords[uAppRecord]->VersionsRecord.size()-1]->VersionId;
			pApp->uLatestVersionId = CDR->ApplicationRecords[uAppRecord]->TrickleVersionId;
			pApp->uMaxLatestVersionLabelChars = 255;

			pApp->uCurrentVersionId = CDR->ApplicationRecords[uAppRecord]->CurrentVersionId;
			pApp->uMaxCurrentVersionLabelChars = 255;

			strcpy(pApp->szInstallDirName, CDR->ApplicationRecords[uAppRecord]->InstallDirName);
			pApp->uMaxInstallDirNameChars = 255;

			pApp->uId = CDR->ApplicationRecords[uAppRecord]->AppId;

			V_snprintf(pApp->szLatestVersionLabel, pApp->uMaxLatestVersionLabelChars, "%u",
				CDR->ApplicationRecords[uAppRecord]->CurrentVersionId);

			V_snprintf(pApp->szCurrentVersionLabel, pApp->uMaxCurrentVersionLabelChars, "%u",
				CDR->ApplicationRecords[uAppRecord]->CurrentVersionId);

			pApp->uMinCacheFileSizeMB = CDR->ApplicationRecords[uAppRecord]->MinCacheFileSizeMB;

			pApp->uMaxCacheFileSizeMB = CDR->ApplicationRecords[uAppRecord]->MaxCacheFileSizeMB;

			pApp->uNumLaunchOptions = CDR->ApplicationRecords[uAppRecord]->LaunchOptionsRecord.size();

			pApp->uNumIcons = 0;

			pApp->uNumVersions = CDR->ApplicationRecords[uAppRecord]->VersionsRecord.size();

			pApp->uNumDependencies = CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord.size();

			SteamClearError(pError);
			return 1;
		}
	}

	//pError->eSteamError = eSteamErrorUnknown;
	pApp->uNumDependencies = 0;
	SteamClearError(pError);
	return 0;
}

STEAM_API int SteamEnumerateAppDependency(unsigned int uAppId, unsigned int uDependency, TSteamAppDependencyInfo* pDependencyInfo, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamEnumerateAppDependency (%u, %u)\n", uAppId, uDependency);

	g_uRootAppId = uAppId;

	if (g_bSteamBlobSystem)
	{
		unsigned int uAppRecord = GetAppRecordID(uAppId);

		if (uAppRecord != UINT_MAX)
		{
			if (uDependency < CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord.size())
			{
				pDependencyInfo->uAppId = CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[uDependency]->AppId;
				pDependencyInfo->bIsSystemDefined = 1;
				strcpy(pDependencyInfo->szMountPath, CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[uDependency]->MountName);
				SteamClearError(pError);
				return 1;
			}
		}
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}

STEAM_API int SteamEnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption* pLaunchOption, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamEnumerateAppLaunchOption\n");

	if (g_bSteamBlobSystem)
	{
		unsigned int uAppRecord = GetAppRecordID(uAppId);

		if (uAppRecord != UINT_MAX)
		{
			if (uLaunchOptionIndex < CDR->ApplicationRecords[uAppRecord]->LaunchOptionsRecord.size())
			{
				CAppLaunchOptionRecord* LaunchOption = CDR->ApplicationRecords[uAppRecord]->LaunchOptionsRecord[uLaunchOptionIndex];

				strcpy(pLaunchOption->szDesc, LaunchOption->Description);
				pLaunchOption->uMaxDescChars = strlen(LaunchOption->Description);
				strcpy(pLaunchOption->szCmdLine, LaunchOption->CommandLine);
				pLaunchOption->uMaxCmdLineChars = strlen(LaunchOption->CommandLine);

				pLaunchOption->uIndex = uLaunchOptionIndex;
				pLaunchOption->uIconIndex = LaunchOption->IconIndex;

				pLaunchOption->bNoDesktopShortcut = (LaunchOption->NoDesktopShortcut ? 1 : 0);
				pLaunchOption->bNoStartMenuShortcut = (LaunchOption->NoStartMenuShortcut ? 1 : 0);
				pLaunchOption->bIsLongRunningUnattended = (LaunchOption->LongRunningUnattended ? 1 : 0);

				SteamClearError(pError);
				return 1;
			}
		}
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}

STEAM_API SteamCallHandle_t SteamRefreshMinimumFootprintFiles(unsigned int uAppId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamRefreshMinimumFootprintFiles\n");

	char szGCF[MAX_PATH];
	char szPath[MAX_PATH];
	char szDestination[MAX_PATH];

	if (g_bSteamBlobSystem)
	{
		unsigned int uAppRecord = GetAppRecordID(uAppId);

		if (uAppRecord != UINT_MAX)
		{
			int x = CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord.size() - 1;

			while (x > -1)
			{
				strcpy(szPath, g_szGCFPath);
				strcpy(szGCF, GetNameById(CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[x]->AppId));
				V_strlower(szGCF);
				strcat(szGCF, ".gcf");

				//GetCurrentDirectoryA(MAX_PATH, szDestination);
				strcpy(szDestination, g_szGCFPath);
				strcat(szDestination, CORRECT_PATH_SEPARATOR_S);
				strcat(szDestination, g_szSteamUser);
				strcat(szDestination, CORRECT_PATH_SEPARATOR_S);
				strcat(szDestination, CDR->ApplicationRecords[uAppRecord]->InstallDirName);
				strcat(szDestination, CORRECT_PATH_SEPARATOR_S);

				//strcat(szDestination,CDR->ApplicationRecords[i]->FilesystemsRecord[x]->MountName);

				V_ComposeFileName(szPath, szGCF, szPath, MAX_PATH);

				int retval = GetFiles(szPath, szDestination, 2, CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[x]->MountName, false);

				x--;
			}
		}
	}

	SteamClearError(pError);
	return 1;
}

STEAM_API SteamCallHandle_t SteamLaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char* cszArgs, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamLaunchApp\n");

	int retval = 0;

#ifdef _WIN32
	//Launch the process by launchoption - add args if specified

	TSteamAppLaunchOption* LaunchOption = new TSteamAppLaunchOption;
	LaunchOption->szCmdLine = new char[MAX_PATH];
	LaunchOption->szDesc = new char[MAX_PATH];
	SteamEnumerateAppLaunchOption(uAppId, uLaunchOption, LaunchOption, pError);

	char szLaunchString[MAX_PATH * 2];

	strcpy(szLaunchString, g_szGCFPath);
	strcat(szLaunchString, "\\");
	strcat(szLaunchString, g_szSteamUser);
	strcat(szLaunchString, "\\");
	strcat(szLaunchString, GetNameById(uAppId));
	strcat(szLaunchString, "\\");

	V_FixSlashes(szLaunchString);

	int PathtoExe = strlen(szLaunchString);

	//if (directoryexists)
	//{
	SteamRefreshMinimumFootprintFiles(uAppId, pError);
	//}

	SetEnvironmentVariableA("SteamUser", g_szSteamUser);

	strcat(szLaunchString, LaunchOption->szCmdLine);
	strcat(szLaunchString, " ");
	strcat(szLaunchString, cszArgs);

	if (bLogging) Logger->Write("\tLaunching %s\n", szLaunchString);

	char FullPathToExe[MAX_PATH];
	char Parameters[MAX_PATH];
	char FullParameters[MAX_PATH];

	//split to path+exe

	int pos = PathtoExe + 1;

	while (szLaunchString[pos] != ' ' && pos < (int)strlen(szLaunchString))
	{
		pos++;
	}

	if (pos < (int)strlen(szLaunchString))
	{
		char testExe[10];
		V_StrLeft(szLaunchString, pos, FullPathToExe, MAX_PATH);

		V_StrRight(FullPathToExe, 2, testExe, 10);
		V_strlower(testExe);

		if (strcmp(testExe, "hl") == 0)
		{
			strcat(FullPathToExe, ".exe");
		}
		else
		{
			V_StrRight(FullPathToExe, 3, testExe, 10);
			V_strlower(testExe);
			if (strcmp(testExe, "hl2") == 0)
			{
				strcat(FullPathToExe, ".exe");
			}
		}

		V_StrRight(szLaunchString, strlen(szLaunchString) - pos, Parameters, MAX_PATH);
		strcpy(FullParameters, "\"");
		strcat(FullParameters, FullPathToExe);
		strcat(FullParameters, "\"");
		strcat(FullParameters, Parameters);
	}

	STARTUPINFO siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	if (CreateProcess(FullPathToExe, FullParameters, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, &siStartupInfo, &piProcessInfo) != false)
	{
		//retval = WaitForSingleObject(piProcessInfo.hProcess, INFINITE );
		SteamClearError(pError);
		retval = piProcessInfo.hProcess ? 1 : 0;
	}
	else
	{
		pError->eSteamError = eSteamErrorUnknown;
		retval = 0;
	}

	//CloseHandle(piProcessInfo.hProcess);
	//CloseHandle(piProcessInfo.hThread);

	delete[] LaunchOption->szCmdLine;
	delete[] LaunchOption->szDesc;
	delete LaunchOption;
#endif

	return retval;
}

STEAM_API int SteamCheckAppOwnership(unsigned int uAppId, int* pbOwned, TSteamGlobalUserID* pSteamID, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamCheckAppOwnership\n");

	SteamClearError(pError);

#if 0
	TSteamGlobalUserID mySteamID;
	if (!SteamGetUser(NULL, 0, NULL, &mySteamID, pError))
	{
		*pbOwned = false;
		return 0;
	}

	if (memcmp(&mySteamID, pSteamID, sizeof(TSteamGlobalUserID)) != 0)
	{
		*pbOwned = false;
		return 1;
	}
#endif

	int pending = false;
	return SteamIsAppSubscribed(uAppId, pbOwned, &pending, pError);
}

STEAM_API int SteamIsSubscribed(unsigned int uSubscriptionId, int* pbIsSubscribed, int* pbIsSubscriptionPending, TSteamError* pError)
{
	if (bLogging && bLogAcc) Logger->Write("SteamIsSubscribed (%u, 0x%p, 0x%p)\n", uSubscriptionId, pbIsSubscribed, pbIsSubscriptionPending);

	// IMPORTANT: Some builds of Garry's Mod have anti-piracy check that sees if SteamIsSubscribed starts with
	// "push ebp" instruction which depends on if and how local vars are used. Be careful when changing this function.
	SteamClearError(pError);

	if (!g_bSteamStartup)
	{
		SteamStartup(STEAM_USING_ALL, pError);
	}

	*pbIsSubscriptionPending = 0;

	switch (uSubscriptionId)
	{
	case 4: //Cyber Cafe Subscription
		*pbIsSubscribed = 0;
		return 1;
	case 67: //VTT Subscription
		*pbIsSubscribed = 0;
		return 1;
	case 183: // Dark Messiah Might and Magic DE
		*pbIsSubscribed = 0;
		return 1;
	case 198: // Dark Messiah Might and Magic Limited Retail DE
		*pbIsSubscribed = 0;
		return 1;
	default:
		break;
	}

	if (!CDR)
	{
		*pbIsSubscribed = 1;
		*pbIsSubscriptionPending = 0;
		return 1;
	}

	for (CSubscriptionRecord* pRecord : CDR->SubscriptionsRecord)
	{
		if (pRecord->SubscriptionId == uSubscriptionId)
		{
			*pbIsSubscribed = 1;
			*pbIsSubscriptionPending = 0;
			if (bLogging && bLogAcc) Logger->Write("\tSubscribed!\n");
			return 1;
		}
	}

	if (bLogging && bLogAcc) Logger->Write("\tNot Subscribed!\n");
	*pbIsSubscribed = 0;
	*pbIsSubscriptionPending = 0;

	return 1;
}

STEAM_API int STEAM_CALL SteamIsAppSubscribed(unsigned int uAppId, int* pbIsAppSubscribed, int* pbIsSubscriptionPending, TSteamError* pError)
{
	if (bLogging && bLogAcc) Logger->Write("SteamIsAppSubscribed (%u, 0x%p, 0x%p)\n", uAppId, pbIsAppSubscribed, pbIsSubscriptionPending);

	SteamClearError(pError);

	if (!g_bSteamStartup)
	{
		SteamStartup(STEAM_USING_ALL, pError);
	}

	if (!CDR)
	{
		*pbIsAppSubscribed = 1;
		*pbIsSubscriptionPending = 0;
		return 1;
	}

	for (CAppRecord* pRecord : CDR->ApplicationRecords)
	{
		if (pRecord->AppId == uAppId)
		{
			*pbIsAppSubscribed = 1;
			*pbIsSubscriptionPending = 0;
			if (bLogging && bLogAcc) Logger->Write("\tSubscribed!\n");
			return 1;
		}
	}

	if (bLogging && bLogAcc) Logger->Write("\tNot Subscribed!\n");
	*pbIsAppSubscribed = 0;
	*pbIsSubscriptionPending = 0;

	return 1;
}

STEAM_API int SteamGetAppPurchaseCountry(unsigned int uAppId, char* szCountryBuf, unsigned int uBufSize, unsigned int* pPurchaseTime, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppPurchaseCountry (%u, 0x%p, %u, 0x%p)\n", uAppId, szCountryBuf, uBufSize, pPurchaseTime);
	strcpy(szCountryBuf, "EN");
	if (pPurchaseTime) *pPurchaseTime = 0x44444444;

	SteamClearError(pError);
	return 1;
}

STEAM_API int SteamGetAppUserDefinedInfo(unsigned int uAppId, const char* cszPropertyName, char* szPropertyValue, unsigned int uBufSize, unsigned int* puPropertyValueLength, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppUserDefinedInfo\n");
	SteamClearError(pError);

	unsigned int uAppRecord = GetAppRecordID(uAppId);

	if (uAppRecord != UINT_MAX)
	{
		CAppRecord* AppRecord = CDR->ApplicationRecords[uAppRecord];

		if (AppRecord)
		{
			std::map<char*, char*>& records = AppRecord->UserDefinedRecords;

			for (auto& record : records)
			{
				const char* recordKey = record.first;
				const char* recordVal = record.second;

				if (V_stricmp(cszPropertyName, recordKey) == 0)
				{
					size_t len = strlen(recordVal);

					if (uBufSize >= len + 1)
					{
						strcpy(szPropertyValue, recordVal);

						if (puPropertyValueLength)
							*puPropertyValueLength = len;

						return 1;
					}
				}
			}
		}
	}

	if (puPropertyValueLength) *puPropertyValueLength = 0;
	return 0;
}

STEAM_API int SteamGetAppStats(TSteamAppStats* pAppStats, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppStats\n");

	if (CDR)
	{
		memset(pAppStats, 0, sizeof(TSteamAppStats));
		pAppStats->uNumApps = CDR->ApplicationRecords.size();

		for (CAppRecord* pAppRecord : CDR->ApplicationRecords)
		{
			pAppStats->uMaxNameChars = max(pAppStats->uMaxNameChars, (uint32)strlen(pAppRecord->Name));
			pAppStats->uMaxInstallDirNameChars = max(pAppStats->uMaxInstallDirNameChars, (uint32)strlen(pAppRecord->InstallDirName));

			for (CAppVersionRecord* pVersionRecord : pAppRecord->VersionsRecord)
			{
				pAppStats->uMaxVersionLabelChars = max(pAppStats->uMaxVersionLabelChars, (uint32)strlen(pVersionRecord->Description));
			}

			pAppStats->uMaxLaunchOptions = max(pAppStats->uMaxLaunchOptions, (uint32)pAppRecord->LaunchOptionsRecord.size());

			for (CAppLaunchOptionRecord* pOptionRecord : pAppRecord->LaunchOptionsRecord)
			{
				pAppStats->uMaxLaunchOptionDescChars = max(pAppStats->uMaxLaunchOptionDescChars, (uint32)strlen(pOptionRecord->Description));
				pAppStats->uMaxLaunchOptionCmdLineChars = max(pAppStats->uMaxLaunchOptionCmdLineChars, (uint32)strlen(pOptionRecord->CommandLine));
			}

			pAppStats->uMaxNumIcons = max(pAppStats->uMaxNumIcons, (uint32)pAppRecord->IconsRecord.size());

			for (CAppIconRecord* pOptionRecord : pAppRecord->IconsRecord)
			{
				// TODO
				pAppStats->uMaxIconSize = max(pAppStats->uMaxIconSize, 128u);
			}
		}

		SteamClearError(pError);
		return 1;
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}

STEAM_API int SteamGetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType uStatType, TSteamUpdateStats* pUpdateStats, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppUpdateStats (%u, %u, 0x%p)\n", uAppId, uStatType, pUpdateStats);
	SteamClearError(pError);

	switch (uStatType)
	{
	case ePhysicalBytesReceivedThisSession:
		break;
	case eAppReadyToLaunchStatus:
		break;
	case eAppPreloadStatus:
		break;
	case eAppEntireDepot:
		break;
	case eCacheBytesPresent: //How much left to download from the specified cache
		pUpdateStats->uBytesTotal = 0;
		pUpdateStats->uBytesPresent = 0;
		break;
	default:
		pError->eSteamError = eSteamErrorUnknown;
	}

	return 0;
}

STEAM_API int SteamGetTotalUpdateStats(TSteamUpdateStats* pUpdateStats, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetTotalUpdateStats\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamGetAppIds(unsigned int* puAppIds, unsigned int uMaxIds, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppIds\n");
	SteamClearError(pError);

	if (CDR)
	{
		if (puAppIds)
		{
			for (unsigned int i = 0; i < uMaxIds; i++)
			{
				if (i >= CDR->ApplicationRecords.size())
					break;

				puAppIds[i] = CDR->ApplicationRecords[i]->AppId;
			}
			SteamClearError(pError);
			return 1;
		}
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}

STEAM_API int SteamEnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char* pIconData, unsigned int uIconDataBufSize, unsigned int* puSizeOfIconData, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamEnumerateAppIcon\n");
	SteamClearError(pError);
	return 0;
}

STEAM_API SteamCallHandle_t SteamWaitForAppReadyToLaunch(unsigned int uAppId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamWaitForAppReadyToLaunch\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamEnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion* pAppVersion, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamEnumerateAppVersion\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamGetSponsorUrl(unsigned int uAppId, char* szUrl, unsigned int uBufSize, unsigned int* pUrlChars, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetSponsorUrl\n");
	strcpy(szUrl, "about:blank");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamMoveApp(unsigned int uAppId, const char* szPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamMoveApp\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamSetAppCacheSize(unsigned int uAppId, unsigned int nCacheSizeInMb, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamSetAppCacheSize\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamSetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamSetAppVersion\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamNumAppsRunning()
{
	if (bLogging) Logger->Write("SteamNumAppsRunning\n");

	return 0;
}

STEAM_API int SteamInsertAppDependency()
{
	if (bLogging) Logger->Write("SteamInsertAppDependency\n");

	return 1;
}

STEAM_API int SteamRemoveAppDependency()
{
	if (bLogging) Logger->Write("SteamRemoveAppDependency\n");

	return 1;
}

STEAM_API int SteamFindApp()
{
	if (bLogging) Logger->Write("SteamFindApp\n");

	return 1;
}

STEAM_API int SteamGetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppDependencies\n");

	return 1;
}

STEAM_API int SteamGetAppDir(unsigned int uAppId, char* szAppDir, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppDir\n");

	return 1;
}

STEAM_API int SteamGetAppUserDefinedRecord(unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void* pvCKeyValue, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetAppUserDefinedRecord\n");
	SteamClearError(pError);

	return 1;
}

//Resources

STEAM_API SteamCallHandle_t SteamWaitForResources(const char* cszMasterList, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamWaitForResources (%s)\n", cszMasterList);
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamForgetAllHints(const char* cszMountPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamForgetAllHints (%s)\n", cszMountPath);
	SteamClearError(pError);

	return 1;
}

//Cache

STEAM_API int SteamCreateCachePreloaders(TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamCreateCachePreloaders\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamIsCacheLoadingEnabled(unsigned int uAppId, int* pbIsLoading, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamIsCacheLoadingEnabled\n");
	*pbIsLoading = 0;
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamFlushCache(unsigned int uAppId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamFlushCache\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamLoadCacheFromDir(unsigned int uAppId, const char* szPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamLoadCacheFromDir\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamPauseCachePreloading(const char* cszMountPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamPauseCachePreloading\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamSetCacheDefaultDirectory(const char* szPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamSetCacheDefaultDirectory\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamStartLoadingCache(unsigned int uAppId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamStartLoadingCache\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API SteamCallHandle_t SteamStopLoadingCache(unsigned int uAppId, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamStopLoadingCache\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamGetCacheDefaultDirectory(char* szPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamGetCacheDefaultDirectory\n");
	SteamClearError(pError);

	if (!g_bSteamFileSystem)
	{
		pError->eSteamError = eSteamErrorUnknown;
		return 0;
	}

	strcpy(szPath, g_szGCFPath);
	return 1;
}

STEAM_API int SteamResumeCachePreloading(const char* cszMountPath, TSteamError* pError)
{
	if (bLogging) Logger->Write("SteamResumeCachePreloading\n");
	SteamClearError(pError);

	return 1;
}

STEAM_API int SteamGetCacheFilePath()
{
	if (bLogging) Logger->Write("SteamGetCacheFilePath\n");

	return 1;
}

STEAM_API int SteamRepairOrDecryptCaches()
{
	if (bLogging) Logger->Write("SteamRepairOrDecryptCaches\n");

	return 1;
}

STEAM_API int SteamIsFileNeededByCache()
{
	if (bLogging) Logger->Write("SteamIsFileNeededByCache\n");

	return 1;
}

STEAM_API int SteamLoadFileToCache()
{
	if (bLogging) Logger->Write("SteamLoadFileToCache\n");

	return 1;
}

STEAM_API int SteamGetCacheDecryptionKey()
{
	if (bLogging) Logger->Write("SteamGetCacheDecryptionKey\n");

	return 1;
}

STEAM_API SteamCallHandle_t SteamGetAppCacheSize(unsigned int uAppId, unsigned int* pCacheSizeInMb, TSteamError* pError)
{
	if (bLogging && bLogFS) Logger->Write("SteamGetAppCacheSize (%u, 0x%p)\n", uAppId, pCacheSizeInMb);
	*pCacheSizeInMb = 256;
	SteamClearError(pError);
	return 1;
}
