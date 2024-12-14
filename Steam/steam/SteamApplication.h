extern BOOL bSteamStartup;
extern unsigned int  rootAppID;

const char * GetNameById(unsigned int id) {

	for(unsigned int i = 0; i < CDR->ApplicationRecords.size(); i++) {
		if(id == CDR->ApplicationRecords[i]->AppId) {
			return CDR->ApplicationRecords[i]->InstallDirName;
		}
	}
	return NULL;
}

int GetFiles(const char *szSource, const char *szDest, int mode, const char *szMount, bool OverwriteConfigFiles)
{

	TManifestEntriesInCache* TestFile;
	//char FileToOpen[MAX_PATH];
	char szFileName[MAX_PATH];
	int retval = 0;
	int iBitResult;
	bool bExtract;


	char szCWD[MAX_PATH];

	strcpy(szCWD, szDest);

	/*
	if(_getcwd(szCWD, sizeof(szCWD)))
	{
		_strlwr(szCWD);
	}
	*/

	unsigned int cHandle;

	cHandle = CacheManager->MountCache(szSource,0,szMount);
	if(cHandle != NULL) 
	{


		CCache *CacheFile = CacheManager->Caches[0]->hCacheFile;

		for (unsigned int x = 1; x < CacheFile->Manifest->Header->ItemCount ; x++)
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

					TFileInCacheHandle* hFile = new TFileInCacheHandle();
					memset(hFile, 0, sizeof(TFileInCacheHandle));
					hFile->FileInCache = TestFile;
					strcpy(hFile->mode, "rb");

					iBitResult = TestFile->Type & 0x00000001; 

					if (iBitResult != 0)
					{

						if (OverwriteConfigFiles == false)
						{
							strcpy(szFileName,szCWD);
							strcat(szFileName,"//");
							strcpy(szFileName,TestFile->FullName);
							V_FixSlashes(szFileName);

							struct _stat buf;	
							retval = _stat(szFileName, &buf);
						}
						else
						{
							retval = -1;
						}
									
						if(!retval)
						{
							retval = CacheManager->CacheExtractFile(hFile, szDest);		
						}
					}
					else
					{
						retval = CacheManager->CacheExtractFile(hFile, szDest);		
					}

					delete hFile;
				}
			}
		}

		CacheManager->UnmountCache(cHandle);

		return 1;

	}

	return 0;

}

STEAM_API int SteamEnumerateApp(unsigned int uAppID, TSteamApp *pApp, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamEnumerateApp (%u)\n", uAppID);

	if (bSteamBlobSystem)
	{
		int uAppRecord = GetAppRecordID(uAppID);

		if (uAppRecord)
		{
			strcpy(pApp->szName, CDR->ApplicationRecords[uAppRecord]->Name);
			pApp->uMaxNameChars = 255;

			//pApp->uLatestVersionId = CDR->ApplicationRecords[uAppRecord]->VersionsRecord[CDR->ApplicationRecords[uAppRecord]->VersionsRecord.size()-1]->VersionId;
			pApp->uLatestVersionId = CDR->ApplicationRecords[uAppRecord]->TrickleVersionId;
			pApp->uMaxLatestVersionLabelChars = 255;

			pApp->uCurrentVersionId = CDR->ApplicationRecords[uAppRecord]->CurrentVersionId;
			pApp->uMaxCurrentVersionLabelChars = 255;

			strcpy(pApp->szCacheFile, CDR->ApplicationRecords[uAppRecord]->InstallDirName);
			pApp->uMaxCacheFileChars = 255;

			pApp->uId = CDR->ApplicationRecords[uAppRecord]->AppId;

			_itoa(CDR->ApplicationRecords[uAppRecord]->CurrentVersionId, pApp->szLatestVersionLabel, 10);

			_itoa(CDR->ApplicationRecords[uAppRecord]->CurrentVersionId, pApp->szCurrentVersionLabel, 10);

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

STEAM_API int SteamEnumerateAppDependency(unsigned int uAppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamEnumerateAppDependency (%u, %u)\n", uAppId, uDependency);

	rootAppID = uAppId;

	if (bSteamBlobSystem)
	{
		int uAppRecord = GetAppRecordID(uAppId);

		if (uAppRecord)
		{
			if (uDependency <= CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord.size())
			{
				pDependencyInfo->AppId = CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[uDependency]->AppId;
				pDependencyInfo->IsRequired = (CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[uDependency]->IsOptional ? 0 : 1);
				strcpy(pDependencyInfo->szMountName, CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[uDependency]->MountName);
				SteamClearError(pError);
				return 1;
			}
		}
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}


STEAM_API int SteamEnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamEnumerateAppLaunchOption\n");

	if (bSteamBlobSystem)
	{

		int uAppRecord = GetAppRecordID(uAppId);
			
		if (uAppRecord)
		{

			CAppLaunchOptionRecord* LaunchOption = CDR->ApplicationRecords[uAppRecord]->LaunchOptionsRecord[uLaunchOptionIndex];
			
			if (LaunchOption)
			{
				strcpy(pLaunchOption->szDesc, LaunchOption->Description);
				pLaunchOption->uMaxDescChars = strlen(LaunchOption->Description);
				strcpy(pLaunchOption->szCmdLine, LaunchOption->CommandLine);
				pLaunchOption->uMaxCmdLineChars = strlen(LaunchOption->CommandLine);

				pLaunchOption->uIndex = uLaunchOptionIndex;
				pLaunchOption->iIconIndex = LaunchOption->IconIndex;

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

STEAM_API SteamCallHandle_t SteamRefreshMinimumFootprintFiles(unsigned int uAppId, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamRefreshMinimumFootprintFiles\n");
	
	char szGCF[MAX_PATH];
	char szPath[MAX_PATH];
	char szDestination[MAX_PATH];

	if (bSteamBlobSystem)
	{
		int uAppRecord = GetAppRecordID(uAppId);
			
		if (uAppRecord)
		{

			int x = CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord.size() - 1;

					while( x > -1)
					{

						for (unsigned int uIndex = 0; uIndex < CacheLocations.size(); uIndex++)
						{

							strcpy(szPath, CacheLocations[uIndex]);
							strcpy(szGCF, GetNameById(CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[x]->AppId));
							strcat(szGCF, ".gcf");
							
							//GetCurrentDirectoryA(MAX_PATH, szDestination);
							strcpy(szDestination,CacheLocations[0]);
							strcat(szDestination,"\\");
							strcat(szDestination,szSteamUser);
							strcat(szDestination,"\\");
							strcat(szDestination,CDR->ApplicationRecords[uAppRecord]->InstallDirName);
							strcat(szDestination,"\\");

							//strcat(szDestination,CDR->ApplicationRecords[i]->FilesystemsRecord[x]->MountName);
							V_FixSlashes(szDestination);
							

							V_ComposeFileName( szPath, szGCF, szPath, MAX_PATH );

							int retval = GetFiles(szPath, szDestination, 2, CDR->ApplicationRecords[uAppRecord]->FilesystemsRecord[x]->MountName ,false);

						}

						x--;
					}
		}

	}

	SteamClearError(pError);
	return 1;
}



STEAM_API SteamCallHandle_t SteamLaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamLaunchApp\n");

	int retval = 0;

	//Launch the process by launchoption - add args if specified

	TSteamAppLaunchOption *LaunchOption = new TSteamAppLaunchOption;
	LaunchOption->szCmdLine = new char[MAX_PATH];
	LaunchOption->szDesc = new char[MAX_PATH];
	SteamEnumerateAppLaunchOption(uAppId, uLaunchOption, LaunchOption, pError );


	char szLaunchString[MAX_PATH*2];

	strcpy(szLaunchString,CacheLocations[0]);
	strcat(szLaunchString,"\\");
	strcat(szLaunchString,szSteamUser);
	strcat(szLaunchString,"\\");
	strcat(szLaunchString,GetNameById(uAppId));
	strcat(szLaunchString,"\\");


	V_FixSlashes(szLaunchString);

	int PathtoExe = strlen(szLaunchString);

	//if (directoryexists)
	//{
		SteamRefreshMinimumFootprintFiles(uAppId, pError);
	//}
	
	SetEnvironmentVariableA("SteamUser", szSteamUser);


	strcat(szLaunchString, LaunchOption->szCmdLine);
	strcat(szLaunchString, " ");
	strcat(szLaunchString, cszArgs);

	if (bLogging) Logger->Write("\tLaunching %s\n", szLaunchString);

	char FullPathToExe[MAX_PATH];
	char Parameters[MAX_PATH];
	char FullParameters[MAX_PATH];
 
	//split to path+exe

	int pos = PathtoExe + 1;

	while(szLaunchString[pos] != ' ' && pos < (int)strlen(szLaunchString))
	{
		pos++;
	}

	if (pos < (int)strlen(szLaunchString))
	{
		char testExe[10];
		V_StrLeft( szLaunchString, pos,FullPathToExe, MAX_PATH);

		V_StrRight( FullPathToExe, 2,testExe, 10);
		_strlwr(testExe);

		if (strcmp(testExe,"hl")==0)
		{		
			strcat(FullPathToExe,".exe");
		}
		else
		{
			V_StrRight( FullPathToExe, 3,testExe, 10);
			_strlwr(testExe);
			if (strcmp(testExe,"hl2")==0)
			{		
				strcat(FullPathToExe,".exe");
			}
		}
		
		V_StrRight( szLaunchString, strlen(szLaunchString) - pos,Parameters, MAX_PATH);
		strcpy(FullParameters,"\"");
		strcat(FullParameters,FullPathToExe);
		strcat(FullParameters,"\"");
		strcat(FullParameters,Parameters);

	}


     STARTUPINFO siStartupInfo;
     PROCESS_INFORMATION piProcessInfo;
     memset(&siStartupInfo, 0, sizeof(siStartupInfo));
     memset(&piProcessInfo, 0, sizeof(piProcessInfo));
     siStartupInfo.cb = sizeof(siStartupInfo);

     if (CreateProcess((LPCSTR)FullPathToExe,(LPSTR)FullParameters, 0, 0, false, CREATE_DEFAULT_ERROR_MODE, 0, 0, &siStartupInfo, &piProcessInfo) != false)
     {
          //retval = WaitForSingleObject(piProcessInfo.hProcess, INFINITE );
		  SteamClearError(pError);
		  retval = (SteamCallHandle_t)piProcessInfo.hProcess; //for check for handle completion
     }
     else
     {
	 	 pError->eSteamError = eSteamErrorUnknown;
		 retval = 0;
     }

     //CloseHandle(piProcessInfo.hProcess);
     //CloseHandle(piProcessInfo.hThread);


	delete [] LaunchOption->szCmdLine;
	delete [] LaunchOption->szDesc;
	delete LaunchOption;

	return retval;
}


STEAM_API int SteamCheckAppOwnership() {
	if (bLogging) Logger->Write("SteamCheckAppOwnership\n");
	return 1;
}

STEAM_API int SteamIsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbIsSubscriptionPending, TSteamError *pError) {
	if (bLogging && bLogAcc) Logger->Write("SteamIsSubscribed (%u)\n",uSubscriptionId);

	// IMPORTANT: Some builds of Garry's Mod have anti-piracy check that sees if SteamIsSubscribed starts with
	// "push ebp" instruction which depends on if and how local vars are used. Be careful when changing this function.
	SteamClearError(pError);

	if (!bSteamStartup)
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

	if (!bSteamFileSystem || (bSteamFileSystem && bSteamBlobSystem && !CDR))
	{
		*pbIsSubscribed = 1;
		*pbIsSubscriptionPending = 0;
		return 1;
	}

	if (bSteamBlobSystem)
	{
		for (unsigned int x = 0; x < CDR->SubscriptionsRecord.size(); x++)
		{
			if (CDR->SubscriptionsRecord[x]->SubscriptionId = uSubscriptionId)
			{
				*pbIsSubscriptionPending = 0;
				*pbIsSubscribed = 1;
				if (bLogging && bLogAcc)  Logger->Write("\tSubscribed!\n");
				return 1;
			}
		}
	}

	if (bLogging && bLogAcc)  Logger->Write("\tNot Subscribed!\n");
	*pbIsSubscribed = 0;
	*pbIsSubscriptionPending = 0;

	return 1;
}

STEAM_API int STEAM_CALL SteamIsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbIsSubscriptionPending, TSteamError *pError)
{
	if (bLogging && bLogAcc)  Logger->Write("SteamIsAppSubscribed: %u %u %u\n", uAppId, *pbIsAppSubscribed, *pbIsSubscriptionPending);

	SteamClearError(pError);

	if (!bSteamStartup)
	{
		SteamStartup(STEAM_USING_ALL, pError);
	}

	if (!bSteamFileSystem || (bSteamFileSystem && bSteamBlobSystem && !CDR))
	{
		//if (!bIsEnginePatched && !bSteamClient) PatchEngine();
		*pbIsSubscriptionPending = 0;
		*pbIsAppSubscribed = 1;
		return 1;
	}

	if (bSteamBlobSystem)
	{
		for (unsigned int x = 0; x < CDR->ApplicationRecords.size(); x++)
		{
			if (CDR->ApplicationRecords[x]->AppId == uAppId)
			{
				//if (!bIsEnginePatched && !bSteamClient) PatchEngine();
				*pbIsSubscriptionPending = 0;
				*pbIsAppSubscribed = 1;
				if (bLogging && bLogAcc)  Logger->Write("\tSubscribed!\n");
				return 1;
			}
		}
	}

	if (bLogging && bLogAcc)  Logger->Write("\tNot Subscribed!\n");
	*pbIsAppSubscribed = 0;
	*pbIsSubscriptionPending = 0;

	return 1;
}

STEAM_API int SteamGetAppPurchaseCountry(unsigned int uAppId, char* szCountryBuf, unsigned int uBufSize, unsigned int* pPurchaseTime, TSteamError* pError) {
	if (bLogging) Logger->Write("SteamGetAppPurchaseCountry\n");
	strcpy(szCountryBuf, "EN");
	if (pPurchaseTime)
		*pPurchaseTime = 0x44444444;
	
	SteamClearError(pError);
	return 1;
}

STEAM_API int SteamGetAppUserDefinedInfo(unsigned int uAppId, const char *cszPropertyName, char *szPropertyValue, unsigned int uBufSize, unsigned int *puPropertyValueLength, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppUserDefinedInfo\n");
	SteamClearError(pError);
	
	char testVal[MAX_PATH];
	char testVal2[MAX_PATH];
	strcpy(testVal2,cszPropertyName);
	_strlwr(testVal2);

	int uAppRecord = GetAppRecordID(uAppId);
		
	if (uAppRecord)
	{

		CAppRecord* AppRecord = CDR->ApplicationRecords[uAppRecord];

		if (AppRecord)
		{

			std::map<char*, char*> UserDefinedRecord = AppRecord->UserDefinedRecords;

			if (UserDefinedRecord.size() > 0)
			{


				std::map<char*, char*>::iterator UserDefinedRecordsIterator;

				for( UserDefinedRecordsIterator = UserDefinedRecord.begin(); UserDefinedRecordsIterator != UserDefinedRecord.end(); UserDefinedRecordsIterator++ ) 
				{

					strcpy(testVal,UserDefinedRecordsIterator->first);
					_strlwr(testVal);

					if (strcmp(testVal,testVal2) == 0)
					{

						if (uBufSize >= strlen(UserDefinedRecordsIterator->second))
						{
							strcpy(szPropertyValue,UserDefinedRecordsIterator->second);
	
							if (puPropertyValueLength)
								*puPropertyValueLength = strlen(szPropertyValue);

							return 1;

						}
					}
				}
			}

			if (puPropertyValueLength)
				*puPropertyValueLength = 0;
			return 0;
		}
	}

	if (puPropertyValueLength)
		*puPropertyValueLength = 0;
	return 0;
}

STEAM_API int SteamGetAppStats(TSteamAppStats *pAppStats, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppStats\n");

	if(CDR)
	{

		memset(pAppStats, 0, sizeof(TSteamAppStats));
		pAppStats->uNumApps = CDR->ApplicationRecords.size();

		for(unsigned int i = 0; i < pAppStats->uNumApps; i++)
		{

			int uAppRecord = GetAppRecordID(i);
			
			if (uAppRecord)
			{

				CAppRecord* AppRecord = CDR->ApplicationRecords[uAppRecord];

				if (AppRecord)
				{

					if(pAppStats->uMaxNameChars < strlen(AppRecord->Name))
					{
						pAppStats->uMaxNameChars = strlen(AppRecord->Name);
					}

					for(unsigned int j = 0; j < AppRecord->VersionsRecord.size(); j++ )
					{
						if(pAppStats->uMaxVersionLabelChars < strlen(AppRecord->VersionsRecord[j]->Description))
						{
							pAppStats->uMaxVersionLabelChars = strlen(AppRecord->VersionsRecord[j]->Description);
						}
					}

					if(pAppStats->uMaxLaunchOptions < AppRecord->LaunchOptionsRecord.size())
					{
						pAppStats->uMaxLaunchOptions = AppRecord->LaunchOptionsRecord.size();
					}

					for(unsigned int j = 0; j < AppRecord->LaunchOptionsRecord.size(); j++ )
					{
						
						if(pAppStats->uMaxLaunchOptionDescChars < strlen(AppRecord->LaunchOptionsRecord[j]->Description))
						{
							pAppStats->uMaxLaunchOptionDescChars = strlen(AppRecord->LaunchOptionsRecord[j]->Description);
						}

						if(pAppStats->uMaxLaunchOptionCmdLineChars < strlen(AppRecord->LaunchOptionsRecord[j]->CommandLine))
						{
							pAppStats->uMaxLaunchOptionCmdLineChars = strlen(AppRecord->LaunchOptionsRecord[j]->CommandLine);
						}

					}

					if(pAppStats->uMaxNumIcons < AppRecord->IconsRecord.size())
					{
						pAppStats->uMaxNumIcons = AppRecord->IconsRecord.size();
					}

					for(unsigned int j = 0; j < AppRecord->LaunchOptionsRecord.size(); j++ )
					{
						
						if(pAppStats->uMaxIconSize < 128)
						{
							pAppStats->uMaxIconSize = 128;
						}

					}

				}

				SteamClearError(pError);
				return 1;
			}
		}

	}
	pError->eSteamError = eSteamErrorUnknown;
    return 0;
}

STEAM_API int SteamGetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType uStatType, TSteamUpdateStats *pUpdateStats, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppUpdateStats %u, %u 0x%08x\n", uAppId, uStatType, pUpdateStats);
	SteamClearError(pError);

	
	switch(uStatType)
	{
		case ePhysicalBytesReceivedThisSession:
			break;
		case eAppReadyToLaunchStatus:
			break;
		case eAppPreloadStatus:
			break;
		case eAppEntireDepot:
			break;
		case eCacheBytesPresent://How much left to download from the specified cache
			pUpdateStats->uBytesTotal = 0;
			pUpdateStats->uBytesPresent = 0;
			break;
		default:
			pError->eSteamError = eSteamErrorUnknown;
	}


    return 0;
}


STEAM_API int SteamGetTotalUpdateStats(TSteamUpdateStats *pUpdateStats, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetTotalUpdateStats\n");
 	SteamClearError(pError);
	
    return 1;
}

STEAM_API int SteamGetAppIds(unsigned int *puAppIds, unsigned int uMaxIds, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppIds\n");
 	SteamClearError(pError);

	if (CDR)
	{
		if(puAppIds) {
			for(unsigned int i = 0;i < uMaxIds;i++) {
				puAppIds[i] = CDR->ApplicationRecords[i]->AppId;
			}
			SteamClearError(pError);
			return 1;
		}
	}

	pError->eSteamError = eSteamErrorUnknown;
	return 0;
}

STEAM_API int SteamEnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize, unsigned int *puSizeOfIconData, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamEnumerateAppIcon\n");
	SteamClearError(pError);
	return 0;
}

STEAM_API SteamCallHandle_t SteamWaitForAppReadyToLaunch(unsigned int uAppId, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamWaitForAppReadyToLaunch\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamEnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamEnumerateAppVersion\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamGetSponsorUrl(unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamGetSponsorUrl\n");
	strcpy(szUrl,"about:blank");
	SteamClearError(pError);
	
	return 1;
}


STEAM_API SteamCallHandle_t SteamMoveApp(unsigned int uAppId, const char *szPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamMoveApp\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API SteamCallHandle_t SteamSetAppCacheSize(unsigned int uAppId, unsigned int nCacheSizeInMb, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamSetAppCacheSize\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API SteamCallHandle_t SteamSetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamSetAppVersion\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamNumAppsRunning() {
	if (bLogging) Logger->Write("SteamNumAppsRunning\n");
	
	return 0;
}

STEAM_API int SteamInsertAppDependency() {
	if (bLogging) Logger->Write("SteamInsertAppDependency\n");
	
	return 1;
}

STEAM_API int SteamRemoveAppDependency() {
	if (bLogging) Logger->Write("SteamRemoveAppDependency\n");
	
	return 1;
}

STEAM_API int SteamFindApp() {
	if (bLogging) Logger->Write("SteamFindApp\n");
	
	return 1;
}

STEAM_API int SteamGetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppDependencies\n");
	
	return 1;
}

STEAM_API int SteamGetAppDir(unsigned int uAppId, char* szAppDir, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppDir\n");
	
	return 1;
}

STEAM_API int SteamGetAppUserDefinedRecord(unsigned int uAppId, AddEntryToKeyValueFn AddEntryToKeyValueFunc, void* pvCKeyValue, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamGetAppUserDefinedRecord\n");
	SteamClearError(pError);
	
	return 1;
}


//Resources

STEAM_API SteamCallHandle_t SteamWaitForResources(const char* cszMasterList, TSteamError* pError) {
	if (bLogging) Logger->Write("SteamWaitForResources: %s\n", cszMasterList);

	SteamClearError(pError);
	//return 1;
	return 0;
}

/*
STEAM_API int SteamHintResourceNeed(const char *cszMountPath, const char *cszMasterList, int bForgetEverything, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamHintResourceNeed %s, %s, %u\n",cszMountPath,cszMasterList,bForgetEverything);
	return 1;
}
*/

STEAM_API int SteamForgetAllHints(const char *cszMountPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamForgetAllHints %s\n", cszMountPath);
	SteamClearError(pError);
	
	return 0;
}




//Cache

STEAM_API int SteamCreateCachePreloaders(TSteamError *pError) {
	if (bLogging) Logger->Write("SteamCreateCachePreloaders\n");
 	SteamClearError(pError);
	
    return 1;
}

STEAM_API int SteamIsCacheLoadingEnabled(unsigned int uAppId, int *pbIsLoading, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamIsCacheLoadingEnabled\n");
	*pbIsLoading = 0;
	SteamClearError(pError);
	
	return 0;
}

STEAM_API SteamCallHandle_t SteamFlushCache(unsigned int uAppId, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamFlushCache\n");
	SteamClearError(pError);
	
	return 0;
}

STEAM_API SteamCallHandle_t SteamLoadCacheFromDir(unsigned int uAppId, const char *szPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamLoadCacheFromDir\n");
	SteamClearError(pError);
	
	return 0;
}

STEAM_API int SteamPauseCachePreloading(const char *cszMountPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamPauseCachePreloading\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamSetCacheDefaultDirectory(const char *szPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamSetCacheDefaultDirectory\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API SteamCallHandle_t SteamStartLoadingCache(unsigned int uAppId, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamStartLoadingCache\n");
	SteamClearError(pError);
	
	return 1;
}


STEAM_API SteamCallHandle_t SteamStopLoadingCache(unsigned int uAppId, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamStopLoadingCache\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamGetCacheDefaultDirectory(char *szPath, TSteamError *pError ) {
	if (bLogging) Logger->Write("SteamGetCacheDefaultDirectory\n");
	if (CacheLocations.size() > 0)
	{
		strcpy(szPath, CacheLocations[0]);
	}
	else
	{
		strcpy(szPath, szGCFPath);
	}

	return 1;
}

STEAM_API int SteamResumeCachePreloading(const char *cszMountPath, TSteamError *pError) {
	if (bLogging) Logger->Write("SteamResumeCachePreloading\n");
	SteamClearError(pError);
	
	return 1;
}

STEAM_API int SteamGetCacheFilePath() {
	if (bLogging) Logger->Write("SteamGetCacheFilePath\n");
	
	return 1;
}

STEAM_API int SteamRepairOrDecryptCaches() {
	if (bLogging) Logger->Write("SteamRepairOrDecryptCaches\n");
	
	return 1;
}

STEAM_API int SteamIsFileNeededByCache() {
	if (bLogging) Logger->Write("SteamIsFileNeededByCache\n");
	
	return 1;
}

STEAM_API int SteamLoadFileToCache() {
	if (bLogging) Logger->Write("SteamLoadFileToCache\n");
	
	return 1;
}

STEAM_API int SteamGetCacheDecryptionKey() {
	if (bLogging) Logger->Write("SteamGetCacheDecryptionKey\n");
	
	return 1;
}


STEAM_API SteamCallHandle_t SteamGetAppCacheSize(unsigned int uAppId, unsigned int *pCacheSizeInMb, TSteamError *pError ) {
	if (bLogging && bLogFS) Logger->Write("SteamGetAppCacheSize(%u, %u)\n",uAppId,pCacheSizeInMb);
	*pCacheSizeInMb = 256;
	SteamClearError(pError);
	return 1;
}