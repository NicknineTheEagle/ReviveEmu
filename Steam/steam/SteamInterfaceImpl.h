#pragma once

#include "ISteamInterface001.h"
#include "ISteamInterface003.h"
#include "ISteamInterface004.h"
#include "ISteamInterface005.h"
#include "ISteamInterface006.h"

class CSteamInterface :
	public ISteamInterface001,
	public ISteamInterface003,
	public ISteamInterface004,
	public ISteamInterface005,
	public ISteamInterface006
{
public:
	CSteamInterface();
	~CSteamInterface();

	SteamCallHandle_t ChangePassword(const char* cszCurrentPassphrase, const char* cszNewPassphrase, TSteamError* pError);
	int GetCurrentEmailAddress(char* szEmailaddress, unsigned int uBufSize, unsigned int* puEmailaddressChars, TSteamError* pError);
	SteamCallHandle_t ChangePersonalQA(const char* cszCurrentPassphrase, const char* cszNewPersonalQuestion, const char* cszNewAnswerToQuestion, TSteamError* pError);
	SteamCallHandle_t ChangeEmailAddress(const char* cszNewEmailAddress, TSteamError* pError);
	SteamCallHandle_t VerifyEmailAddress(const char* cszEmailAddress, TSteamError *pError);
	SteamCallHandle_t RequestEmailAddressVerificationEmail(TSteamError *pError);
	SteamCallHandle_t ChangeAccountName(const char* cszCurrentAccountName, const char* cszNewAccountName, TSteamError* pError);
	int MountAppFilesystem(TSteamError *pError);
	int UnmountAppFilesystem(TSteamError* pError);
	int MountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError);
	int UnmountFilesystem(unsigned int uAppId, TSteamError *pError);
	int Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError);
	int SetvBuf(SteamHandle_t hFile, void* pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError);
	int FlushFile(SteamHandle_t hFile, TSteamError *pError);
	SteamHandle_t OpenFile(const char *cszName, const char *cszMode, TSteamError *pError);
	SteamHandle_t OpenFileEx(const char* cszFileName, const char* cszMode, unsigned int* puSize, TSteamError* pError);
	SteamHandle_t OpenFileEx(const char* cszFileName, const char* cszMode, int iArg3, unsigned int* puSize, int* piArg5, TSteamError *pError);
	SteamHandle_t OpenTmpFile(TSteamError* pError);
	void ClearError(TSteamError* pError);
	int GetVersion(char* szVersion, unsigned int uVersionBufSize);
	int GetOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError);
	int ChangeOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError);
	int ProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError);
	int AbortCall(SteamCallHandle_t handle, TSteamError *pError);
	int BlockingCall(SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError);
	int SetMaxStallCount(unsigned int uNumStalls, TSteamError *pError);
	int CloseFile(SteamHandle_t hFile, TSteamError *pError);
	unsigned int ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
	unsigned int WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
	int Getc(SteamHandle_t hFile, TSteamError *pError);
	int Putc(int cChar, SteamHandle_t hFile, TSteamError *pError);
	int SeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod sm, TSteamError *pError);
	long TellFile(SteamHandle_t hFile, TSteamError *pError);
	int SizeFile(SteamHandle_t hFile, TSteamError *pError);
	SteamHandle_t FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError);
	int FindNext(SteamHandle_t hFind, TSteamElemInfo *pFindInfo, TSteamError *pError);
	int FindClose(SteamHandle_t hFind, TSteamError *pError);
	int GetLocalFileCopy(const char *cszName, TSteamError *pError);
	int IsFileImmediatelyAvailable(const char *cszName, TSteamError *pError);
	int HintResourceNeed(const char *cszHintList, int bForgetEverything, TSteamError *pError);
	int ForgetAllHints(TSteamError *pError);
	int PauseCachePreloading(TSteamError *pError);
	int ResumeCachePreloading(TSteamError *pError);
	SteamCallHandle_t WaitForResources(const char *cszMasterList, TSteamError *pError);
	int StartEngine(TSteamError *pError);
	int ShutdownEngine(TSteamError *pError);
	int Startup(unsigned int uUsingMask, TSteamError *pError);
	int Cleanup(TSteamError *pError);
	int NumAppsRunning(TSteamError *pError);
	SteamCallHandle_t CreateAccount(const char *cszUser, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, const char* cszArg6, int *pbCreated, TSteamError *pError);
	SteamCallHandle_t GenerateSuggestedAccountNames(const char* cszArg1, const char* cszArg2, char* szArg3, unsigned int uArg4, unsigned int* puArg5, TSteamError *pError);
	int IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError);
	SteamCallHandle_t Logout(TSteamError *pError);
	int IsSecureComputer(int *pbIsSecure, TSteamError *pError);
	SteamHandle_t CreateLogContext(const char *cszName);
	int Log(SteamHandle_t hContext, const char *cszMsg);
	void LogResourceLoadStarted(const char *cszMsg);
	void LogResourceLoadFinished(const char *cszMsg);
	SteamCallHandle_t RefreshLogin(const char *cszPassphrase, int bIsSecureComputer, TSteamError* pError);
	int VerifyPassword(const char* cszArg1, int* piArg2, TSteamError* pError);
	int GetUserType(unsigned int* puArg1, TSteamError* pError);
	int GetAppStats(TSteamAppStats *pAppStats, TSteamError *pError);
	SteamCallHandle_t IsAccountNameInUse(const char* cszArg1, int* piArg2, TSteamError *pError);
	int GetAppIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError);
	int GetSubscriptionStats(TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError);
	SteamCallHandle_t RefreshAccountInfo(TSteamError *pError);
	SteamCallHandle_t RefreshAccountInfo(int arg1, TSteamError *pError);
	SteamCallHandle_t Subscribe(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError);
	SteamCallHandle_t Unsubscribe(unsigned int uSubscriptionId, TSteamError *pError);
	int GetSubscriptionReceipt(unsigned int uSubscriptionId , TSteamSubscriptionReceipt* pSteamSubscriptionReceipt, TSteamError *pError);
	int GetAccountStatus(unsigned int* puArg1, TSteamError *pError);
	SteamCallHandle_t SetUser(const char *cszUser, int *pbUserSet, TSteamError *pError);
	int GetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError);
	SteamCallHandle_t Login(const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError);
	SteamCallHandle_t AckSubscriptionReceipt(unsigned int uArg1, TSteamError *pError);
	int IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pReserved, TSteamError *pError);
	int GetSubscriptionIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError);
	int EnumerateSubscription(unsigned int uSubscriptionId, TSteamSubscription *pSubscription, TSteamError *pError);
	int EnumerateSubscriptionDiscount(unsigned int uSubscriptionId, unsigned int uDiscountIdx, TSteamSubscriptionDiscount* pSteamSubscriptionDiscount, TSteamError *pError);
	int EnumerateSubscriptionDiscountQualifier(unsigned int uSubscriptionId, unsigned int uDiscountIdx, unsigned int uQualifierIdx, TSteamDiscountQualifier* pSteamDiscountQualifier, TSteamError *pError);
	int EnumerateApp(unsigned int uAppId, TSteamApp *pApp, TSteamError *pError);
	int EnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError);
	SteamCallHandle_t DeleteAccount(TSteamError *pError);
	int EnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize,  unsigned int *puSizeOfIconData, TSteamError *pError);
	SteamCallHandle_t LaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError);
	int GetCacheFilePath(unsigned int uAppId,  char* szFilePath, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError);
	int EnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError);
	int EnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError);
	SteamCallHandle_t StartLoadingCache(unsigned int uAppId, TSteamError *pError);
	int InsertAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError);
	int RemoveAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamError *pError);
	int FindApp(const char* cszArg1, unsigned int* puArg2, TSteamError *pError);
	int GetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError *pError);
	int IsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pReserved, TSteamError *pError);
	int GetAppUserDefinedInfo(unsigned int uAppId, const char *cszPropertyName, char *szPropertyValue, unsigned int uBufSize, unsigned int *puPropertyValueLength, TSteamError *pError);
	SteamCallHandle_t WaitForAppReadyToLaunch(unsigned int uAppId, TSteamError *pError);
	int IsCacheLoadingEnabled(unsigned int uAppId, int *pbIsLoading, TSteamError *pError);
	SteamCallHandle_t StopLoadingCache(unsigned int uAppId, TSteamError *pError);
	ESteamError GetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError);
	SteamCallHandle_t FlushCache(unsigned int uAppId, TSteamError *pError);
	SteamCallHandle_t RepairOrDecryptCaches(unsigned int uAppId, int iArg2, TSteamError *pError);
	SteamCallHandle_t LoadCacheFromDir(unsigned int uAppId, const char *cszPath, TSteamError *pError);
	int GetCacheDefaultDirectory(char *szPath, TSteamError *pError);
	int SetCacheDefaultDirectory(const char *cszPath, TSteamError *pError);
	int GetAppDir(unsigned int uAppId, char* szAppDir, TSteamError *pError);
	SteamCallHandle_t MoveApp(unsigned int uAppId, const char *szPath, TSteamError *pError);
	SteamCallHandle_t GetAppCacheSize(unsigned int uAppId, unsigned int *puCacheSizeInMb, TSteamError *pError);
	SteamCallHandle_t SetAppCacheSize(unsigned int uAppId, unsigned int uCacheSizeInMb, TSteamError *pError);
	SteamCallHandle_t SetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError);
	SteamCallHandle_t Uninstall(TSteamError *pError);
	int SetNotificationCallback(SteamNotificationCallback_t pCallbackFunction, TSteamError *pError);
	SteamCallHandle_t ChangeForgottenPassword(const char* cszArg1, const char* cszArg2, const char* cszArg3, const char* cszArg4, int* piArg5, TSteamError *pError);
	SteamCallHandle_t RequestForgottenPasswordEmail(const char* cszArg1, char* szArg2, TSteamError *pError);
	SteamCallHandle_t RequestAccountsByEmailAddressEmail(const char* cszArg1, TSteamError *pError);
	SteamCallHandle_t RequestAccountsByCdKeyEmail(const char* cszArg1, TSteamError *pError);
	SteamCallHandle_t GetNumAccountsWithEmailAddress(const char* cszArg1, unsigned int* puArg2, TSteamError *pError);
	SteamCallHandle_t UpdateAccountBillingInfo(const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError);
	SteamCallHandle_t UpdateSubscriptionBillingInfo(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError);
	int GetSponsorUrl(unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError);
	int GetContentServerInfo(unsigned int uAppId, unsigned int* pServerId, unsigned int* pServerIpAddress, TSteamError* pError);
	SteamCallHandle_t GetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType eSteamAppUpdateStatsQueryType, TSteamUpdateStats *pUpdateStats, TSteamError *pError);
	int GetTotalUpdateStats(TSteamUpdateStats *pUpdateStats, TSteamError *pError);
	SteamCallHandle_t CreateCachePreloaders(TSteamError *pError);
//SteamInterface004 functions
	void Win32SetMiniDumpComment(const char *cszComment);
	void Win32SetMiniDumpSourceControlId(unsigned int uSourceControlId);
	void Win32SetMiniDumpEnableFullMemory();
	void Win32WriteMiniDump(const char* cszArg1, const char* cszArg2, const char* cszArg3, const char* cszArg4, unsigned int uArg5);
	int GetCurrentAppId(unsigned int* puAppId, TSteamError *pError);
	int GetAppPurchaseCountry(unsigned int uAppId, char* szCountryCode, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError* pError);
//SteamInterface005 functions
	int GetLocalClientVersion(unsigned int* puArg1, unsigned int* puArg2, TSteamError *pError);
	int IsFileNeededByCache(unsigned int uArg1, const char* cszFileName, unsigned int uArg3, TSteamError *pError);
	int LoadFileToCache(unsigned int uArg1, const char* cszArg2, const void* pcvArg3, unsigned int uArg4, unsigned int uArg5, TSteamError *pError);
	int GetCacheDecryptionKey(unsigned int uAppId, char* szCacheDecryptionKey, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError);
	int GetSubscriptionExtendedInfo(unsigned int uSubscritptionId, const char* cszKeyName, char* szKeyValue, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError);
	int GetSubscriptionPurchaseCountry(unsigned int uSubscritptionId, char* szCountry, unsigned int uBufferLength , int* piRecievedLength, TSteamError *pError);
	int GetAppUserDefinedRecord(unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void* pvCKeyValue, TSteamError *pError);
//SteamInterface006 functions
	int FindServersNumServers(ESteamServerType eSteamServerType);
	int FindServersIterateServer(ESteamServerType eSteamServerType, int iIndex, char *szServerAddress, unsigned int uServerAddressChars);
	const char* FindServersGetErrorString();
	int CheckAppOwnership(unsigned int uAppId, int* pbOwned, TSteamGlobalUserID* pSteamID, TSteamError* pError);
};
