#pragma once

#include "ISteamInterface003.h"

class CSteamInterface003 : public ISteamInterface003
{
public:
	CSteamInterface003();
	virtual ~CSteamInterface003();

	virtual SteamCallHandle_t ChangePassword(const char* cszCurrentPassphrase, const char* cszNewPassphrase, TSteamError* pError){return 1;}
	virtual int GetCurrentEmailAddress(char* szEmailaddress, unsigned int uBufSize, unsigned int* puEmailaddressChars, TSteamError* pError){return 1;}
	virtual SteamCallHandle_t ChangePersonalQA(const char* cszCurrentPassphrase, const char* cszNewPersonalQuestion, const char* cszNewAnswerToQuestion, TSteamError* pError){return 1;}
	virtual SteamCallHandle_t ChangeEmailAddress(const char* cszNewEmailAddress, TSteamError* pError){return 1;}
	virtual SteamCallHandle_t VerifyEmailAddress(const char* cszEmailAddress, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RequestEmailAddressVerificationEmail(TSteamError *pError){return 1;}
	virtual SteamCallHandle_t ChangeAccountName(const char* cszCurrentAccountName, const char* cszNewAccountName, TSteamError* pError){return 1;}
	virtual int MountAppFilesystem(TSteamError *pError){return 1;}
	virtual int UnmountAppFilesystem(TSteamError* pError){return 1;}
	virtual int MountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError){return 1;}
	virtual int UnmountFilesystem(unsigned int uAppId, TSteamError *pError){return 1;}
	
	virtual int Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError);
	
	virtual int SetvBuf(SteamHandle_t hFile, void* pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError){return 1;}
	virtual int FlushFile(SteamHandle_t hFile, TSteamError *pError){return 1;}
	
	virtual SteamHandle_t OpenFile(const char *cszName, const char *cszMode, TSteamError *pError);
	
	virtual SteamHandle_t OpenFileEx(const char *cszFileName, const char *cszMode, unsigned int* puSize, TSteamError *pError){return 1;}
	virtual SteamHandle_t OpenTmpFile(TSteamError* pError){return 1;}
	virtual void ClearError(TSteamError* pError){return;}
	virtual int GetVersion(char* szVersion, unsigned int uVersionBufSize){return 1;}
	virtual int GetOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError){return 1;}
	virtual int ChangeOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError){return 1;}
	virtual int ProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError){return 1;}
	virtual int AbortCall(SteamCallHandle_t handle, TSteamError *pError){return 1;}
	virtual int BlockingCall(SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError){return 1;}
	virtual int SetMaxStallCount(unsigned int uNumStalls, TSteamError *pError){return 1;}
	
	virtual int CloseFile(SteamHandle_t hFile, TSteamError *pError);
	
	virtual unsigned int ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
	
	virtual unsigned int WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
	
	virtual int Getc(SteamHandle_t hFile, TSteamError *pError){return 1;}
	virtual int Putc(int cChar, SteamHandle_t hFile, TSteamError *pError){return 1;}
	virtual int SeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod sm, TSteamError *pError){return 1;}
	virtual long TellFile(SteamHandle_t hFile, TSteamError *pError){return 1;}
	virtual int SizeFile(SteamHandle_t hFile, TSteamError *pError){return 1;}
	
	virtual SteamHandle_t FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError);
	
	virtual int FindNext(SteamHandle_t hFind, TSteamElemInfo *pFindInfo, TSteamError *pError);
	
	virtual int FindClose(SteamHandle_t hFind, TSteamError *pError);
	
	virtual int GetLocalFileCopy(const char *cszName, TSteamError *pError);
	
	virtual int IsFileImmediatelyAvailable(const char *cszName, TSteamError *pError){return 1;}
	virtual int HintResourceNeed(const char *cszHintList, int bForgetEverything, TSteamError *pError){return 1;}
	virtual int ForgetAllHints(TSteamError *pError){return 1;}
	virtual int PauseCachePreloading(TSteamError *pError){return 1;}
	virtual int ResumeCachePreloading(TSteamError *pError){return 1;}
	virtual SteamCallHandle_t WaitForResources(const char *cszMasterList, TSteamError *pError){return 1;}
	virtual int StartEngine(TSteamError *pError){return 1;}
	virtual int ShutdownEngine(TSteamError *pError){return 1;}
	
	virtual int Startup(unsigned int uUsingMask, TSteamError *pError);
	
	virtual int Cleanup(TSteamError *pError);
	
	virtual int NumAppsRunning(TSteamError *pError){return 1;}
	virtual SteamCallHandle_t CreateAccount(const char *cszUser, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, const char* cszArg6, int *pbCreated, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t GenerateSuggestedAccountNames(const char* cszArg1, const char* cszArg2, char* szArg3, unsigned int uArg4, unsigned int* puArg5, TSteamError *pError){return 1;}
	
	virtual int IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError);
	
	virtual SteamCallHandle_t Logout(TSteamError *pError){return 1;}
	
	virtual int IsSecureComputer(int *pbIsSecure, TSteamError *pError);
	
	virtual SteamHandle_t CreateLogContext(const char *cszName){return 1;}
	virtual int Log(SteamHandle_t hContext, const char *cszMsg){return 1;}
	virtual void LogResourceLoadStarted(const char *cszMsg){return;}
	virtual void LogResourceLoadFinished(const char *cszMsg){return;}
	virtual SteamCallHandle_t RefreshLogin(const char *cszPassphrase, int bIsSecureComputer, TSteamError* pError){return 1;}
	virtual int VerifyPassword(const char* cszArg1, int* piArg2, TSteamError* pError){return 1;}
	virtual int GetUserType(unsigned int* puArg1, TSteamError* pError){return 1;}
	virtual int GetAppStats(TSteamAppStats *pAppStats, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t IsAccountNameInUse(const char* cszArg1, int* piArg2, TSteamError *pError){return 1;}
	virtual int GetAppIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError){return 1;}
	virtual int GetSubscriptionStats(TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RefreshAccountInfo(TSteamError *pError){return 1;}
	virtual SteamCallHandle_t Subscribe(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t Unsubscribe(unsigned int uSubscriptionId, TSteamError *pError){return 1;}
	virtual int GetSubscriptionReceipt(unsigned int uSubscriptionId , TSteamSubscriptionReceipt* pSteamSubscriptionReceipt, TSteamError *pError){return 1;}
	virtual int GetAccountStatus(unsigned int* puArg1, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t SetUser(const char *cszUser, int *pbUserSet, TSteamError *pError){return 1;}
	virtual int GetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t Login(const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t AckSubscriptionReceipt(unsigned int uArg1, TSteamError *pError){return 1;}
	
	virtual int IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pReserved, TSteamError *pError);
	
	virtual int GetSubscriptionIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError){return 1;}
	virtual int EnumerateSubscription(unsigned int uSubscriptionId, TSteamSubscription *pSubscription, TSteamError *pError){return 1;}
	virtual int EnumerateSubscriptionDiscount(unsigned int uSubscriptionId, unsigned int uDiscountIdx, TSteamSubscriptionDiscount* pSteamSubscriptionDiscount, TSteamError *pError){return 1;}
	virtual int EnumerateSubscriptionDiscountQualifier(unsigned int uSubscriptionId, unsigned int uDiscountIdx, unsigned int uQualifierIdx, TSteamDiscountQualifier* pSteamDiscountQualifier, TSteamError *pError){return 1;}
	virtual int EnumerateApp(unsigned int uAppId, TSteamApp *pApp, TSteamError *pError){return 1;}
	virtual int EnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t DeleteAccount(TSteamError *pError){return 1;}
	virtual int EnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize,  unsigned int *puSizeOfIconData, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t LaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError){return 1;}
	virtual int GetCacheFilePath(unsigned int uAppId,  char* szFilePath, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError){return 1;}
	virtual int EnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError){return 1;}
	virtual int EnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t StartLoadingCache(unsigned int uAppId, TSteamError *pError){return 1;}
	virtual int InsertAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError){return 1;}
	virtual int RemoveAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamError *pError){return 1;}
	virtual int FindApp(const char* cszArg1, unsigned int* puArg2, TSteamError *pError){return 1;}
	virtual int GetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError *pError){return 1;}
	virtual int IsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pReserved, TSteamError *pError){return 1;}
	virtual int GetAppUserDefinedInfo(unsigned int uAppId, const char *cszPropertyName, char *szPropertyValue, unsigned int uBufSize, unsigned int *puPropertyValueLength, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t WaitForAppReadyToLaunch(unsigned int uAppId, TSteamError *pError){return 1;}
	virtual int IsCacheLoadingEnabled(unsigned int uAppId, int *pbIsLoading, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t StopLoadingCache(unsigned int uAppId, TSteamError *pError){return 1;}
	
	virtual ESteamError GetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError){return eSteamErrorNone;}
	/*{
		memcpy(pOutputBuffer, m_encrypted, 348);
		*pReceiveSizeOfEncryptedTicket = 348;
		SteamClearError(pError);
		return eSteamErrorNone;
	}*/
	
	virtual SteamCallHandle_t FlushCache(unsigned int uAppId, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RepairOrDecryptCaches(unsigned int uAppId, int iArg2, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t LoadCacheFromDir(unsigned int uAppId, const char *cszPath, TSteamError *pError){return 1;}
	virtual int GetCacheDefaultDirectory(char *szPath, TSteamError *pError){return 1;}
	virtual int SetCacheDefaultDirectory(const char *cszPath, TSteamError *pError){return 1;}
	virtual int GetAppDir(unsigned int uAppId, char* szAppDir, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t MoveApp(unsigned int uAppId, const char *szPath, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t GetAppCacheSize(unsigned int uAppId, unsigned int *puCacheSizeInMb, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t SetAppCacheSize(unsigned int uAppId, unsigned int uCacheSizeInMb, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t SetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t Uninstall(TSteamError *pError){return 1;}
	virtual int SetNotificationCallback(SteamNotificationCallback_t pCallbackFunction, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t ChangeForgottenPassword(const char* cszArg1, const char* cszArg2, const char* cszArg3, const char* cszArg4, int* piArg5, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RequestForgottenPasswordEmail(const char* cszArg1, char* szArg2, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RequestAccountsByEmailAddressEmail(const char* cszArg1, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t RequestAccountsByCdKeyEmail(const char* cszArg1, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t GetNumAccountsWithEmailAddress(const char* cszArg1, unsigned int* puArg2, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t UpdateAccountBillingInfo(const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t UpdateSubscriptionBillingInfo(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError){return 1;}
	virtual int GetSponsorUrl(unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError){return 1;}
	virtual int GetContentServerInfo(unsigned int uArg1, unsigned int* puArg2, unsigned int* puArg3, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t GetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType eSteamAppUpdateStatsQueryType, TSteamUpdateStats *pUpdateStats, TSteamError *pError){return 1;}
	virtual int GetTotalUpdateStats(TSteamUpdateStats *pUpdateStats, TSteamError *pError){return 1;}
	virtual SteamCallHandle_t CreateCachePreloaders(TSteamError *pError){return 1;}
};
