#include "stdafx.h"
#include "Steam.h"
#include "SteamInterfaceImpl.h"

extern CLogFile* Logger;
extern bool bLogging;
extern bool bLogFS;

CSteamInterface::CSteamInterface(){}
CSteamInterface::~CSteamInterface(){}

SteamCallHandle_t CSteamInterface::ChangePassword(const char* cszCurrentPassphrase, const char* cszNewPassphrase, TSteamError* pError){return SteamChangePassword(cszCurrentPassphrase, cszNewPassphrase, pError);}
int CSteamInterface::GetCurrentEmailAddress(char* szEmailaddress, unsigned int uBufSize, unsigned int* puEmailaddressChars, TSteamError* pError){return SteamGetCurrentEmailAddress(szEmailaddress, uBufSize, puEmailaddressChars, pError);}
SteamCallHandle_t CSteamInterface::ChangePersonalQA(const char* cszCurrentPassphrase, const char* cszNewPersonalQuestion, const char* cszNewAnswerToQuestion, TSteamError* pError){return SteamChangePersonalQA(cszCurrentPassphrase, cszNewPersonalQuestion, cszNewAnswerToQuestion, pError);}
SteamCallHandle_t CSteamInterface::ChangeEmailAddress(const char* cszNewEmailAddress, TSteamError* pError){return SteamChangeEmailAddress(cszNewEmailAddress, pError);}
SteamCallHandle_t CSteamInterface::VerifyEmailAddress(const char* cszEmailAddress, TSteamError *pError){return SteamVerifyEmailAddress(cszEmailAddress, pError);}
SteamCallHandle_t CSteamInterface::RequestEmailAddressVerificationEmail(TSteamError *pError){return SteamRequestEmailAddressVerificationEmail(pError);}
SteamCallHandle_t CSteamInterface::ChangeAccountName(const char* cszCurrentAccountName, const char* cszNewAccountName, TSteamError* pError){return SteamChangeAccountName(cszCurrentAccountName, cszNewAccountName, pError);}
int CSteamInterface::MountAppFilesystem(TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamMountAppFilesystem(pError);}
int CSteamInterface::UnmountAppFilesystem(TSteamError* pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamUnmountAppFilesystem(pError);}
int CSteamInterface::MountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamMountFilesystem(uAppId, szMountPath, pError);}
int CSteamInterface::UnmountFilesystem(unsigned int uAppId, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamUnmountFilesystem(uAppId, pError);}
int CSteamInterface::Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError)
{
	/*if(bLogging && bLogFS)Logger->Write("[006] ");*/
	return SteamStat(cszName, pInfo, pError);
}
int CSteamInterface::SetvBuf(SteamHandle_t hFile, void* pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamSetvBuf(hFile, pBuf, eMethod, uBytes, pError);}
int CSteamInterface::FlushFile(SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamFlushFile(hFile, pError);}
SteamHandle_t CSteamInterface::OpenFile(const char *cszName, const char *cszMode, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamOpenFile(cszName, cszMode, pError);}
SteamHandle_t CSteamInterface::OpenFileEx(const char* cszFileName, const char* cszMode, unsigned int* puFileSize, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamOpenFileEx(cszFileName, cszMode, puFileSize, pError);}
SteamHandle_t CSteamInterface::OpenFileEx(const char* cszFileName, const char* cszMode, int nFlags, unsigned int* puFileSize, int* pbLocal, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamOpenFile2(cszFileName, cszMode, nFlags, puFileSize, pbLocal, pError);}
SteamHandle_t CSteamInterface::OpenTmpFile(TSteamError* pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamOpenTmpFile(pError);}
void CSteamInterface::ClearError(TSteamError* pError){return SteamClearError(pError);}
int CSteamInterface::GetVersion(char* szVersion, unsigned int uVersionBufSize){return SteamGetVersion(szVersion, uVersionBufSize);}
int CSteamInterface::GetOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError){return SteamGetOfflineStatus(pSteamOfflineStatus, pError);}
int CSteamInterface::ChangeOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError){return SteamChangeOfflineStatus(pSteamOfflineStatus, pError);}
int CSteamInterface::ProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError){return SteamProcessCall(handle, pProgress, pError);}
int CSteamInterface::AbortCall(SteamCallHandle_t handle, TSteamError *pError){return SteamAbortCall(handle, pError);}
int CSteamInterface::BlockingCall(SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError){return SteamBlockingCall(handle, uiProcessTickMS, pError);}
int CSteamInterface::SetMaxStallCount(unsigned int uNumStalls, TSteamError *pError){return SteamSetMaxStallCount(uNumStalls, pError);}
int CSteamInterface::CloseFile(SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamCloseFile(hFile, pError);}
unsigned int CSteamInterface::ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamReadFile(pBuf, uSize, uCount, hFile, pError);}
unsigned int CSteamInterface::WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamWriteFile(pBuf, uSize, uCount, hFile, pError);}
int CSteamInterface::Getc(SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamGetc(hFile, pError);}
int CSteamInterface::Putc(int cChar, SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamPutc(cChar, hFile, pError);}
int CSteamInterface::SeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod sm, TSteamError *pError)
{
	/*if(bLogging && bLogFS)Logger->Write("[006] ");*/
	return SteamSeekFile(hFile, lOffset, sm, pError);
}
long CSteamInterface::TellFile(SteamHandle_t hFile, TSteamError *pError)
{
	/*if(bLogging && bLogFS)Logger->Write("[006] ");*/
	return SteamTellFile(hFile, pError);
}
int CSteamInterface::SizeFile(SteamHandle_t hFile, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamSizeFile(hFile, pError);}
SteamHandle_t CSteamInterface::FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError);}
int CSteamInterface::FindNext(SteamHandle_t hFind, TSteamElemInfo *pFindInfo, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamFindNext(hFind, pFindInfo, pError);}
int CSteamInterface::FindClose(SteamHandle_t hFind, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamFindClose(hFind, pError);}
int CSteamInterface::GetLocalFileCopy(const char *cszName, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamGetLocalFileCopy(cszName, pError);}
int CSteamInterface::IsFileImmediatelyAvailable(const char *cszName, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamIsFileImmediatelyAvailable(cszName, pError);}
int CSteamInterface::HintResourceNeed(const char *cszHintList, int bForgetEverything, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamHintResourceNeed(cszHintList, bForgetEverything, pError);}
int CSteamInterface::ForgetAllHints(TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamForgetAllHints(pError);}
int CSteamInterface::PauseCachePreloading(TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamPauseCachePreloading(pError);}
int CSteamInterface::ResumeCachePreloading(TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamResumeCachePreloading(pError);}
SteamCallHandle_t CSteamInterface::WaitForResources(const char *cszMasterList, TSteamError *pError){return SteamWaitForResources(cszMasterList, pError);}
int CSteamInterface::StartEngine(TSteamError *pError){return SteamStartEngine(pError);}
int CSteamInterface::ShutdownEngine(TSteamError *pError){return SteamShutdownEngine(pError);}
int CSteamInterface::Startup(unsigned int uUsingMask, TSteamError *pError){return SteamStartup(uUsingMask, pError);}
int CSteamInterface::Cleanup(TSteamError *pError){return SteamCleanup(pError);}
int CSteamInterface::NumAppsRunning(TSteamError *pError){return SteamNumAppsRunning(pError);}
SteamCallHandle_t CSteamInterface::CreateAccount(const char *cszUser, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, const char* cszArg6, int *pbCreated, TSteamError *pError){return SteamCreateAccount(cszUser, cszPassphrase, cszCreationKey, cszPersonalQuestion, cszAnswerToQuestion, cszArg6, pbCreated, pError);}
SteamCallHandle_t CSteamInterface::GenerateSuggestedAccountNames(const char* cszArg1, const char* cszArg2, char* szArg3, unsigned int uArg4, unsigned int* puArg5, TSteamError *pError){return SteamGenerateSuggestedAccountNames(cszArg1, cszArg2, szArg3, uArg4, puArg5, pError);}
int CSteamInterface::IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError){return SteamIsLoggedIn(pbIsLoggedIn, pError);}
SteamCallHandle_t CSteamInterface::Logout(TSteamError *pError){return SteamLogout(pError);}
int CSteamInterface::IsSecureComputer(int *pbIsSecure, TSteamError *pError){return SteamIsSecureComputer(pbIsSecure, pError);}
SteamHandle_t CSteamInterface::CreateLogContext(const char *cszName){return SteamCreateLogContext(cszName);}
int CSteamInterface::Log(SteamHandle_t hContext, const char *cszMsg){return SteamLog(hContext, cszMsg);}
void CSteamInterface::LogResourceLoadStarted(const char *cszMsg){return SteamLogResourceLoadStarted(cszMsg);}
void CSteamInterface::LogResourceLoadFinished(const char *cszMsg){return SteamLogResourceLoadFinished(cszMsg);}
SteamCallHandle_t CSteamInterface::RefreshLogin(const char *cszPassphrase, int bIsSecureComputer, TSteamError* pError){return SteamRefreshLogin(cszPassphrase, bIsSecureComputer, pError);}
int CSteamInterface::VerifyPassword(const char* cszArg1, int* piArg2, TSteamError* pError){return SteamVerifyPassword(cszArg1, piArg2, pError);}
int CSteamInterface::GetUserType(unsigned int* puArg1, TSteamError* pError){return SteamGetUserType(puArg1, pError);}
int CSteamInterface::GetAppStats(TSteamAppStats *pAppStats, TSteamError *pError){return SteamGetAppStats(pAppStats, pError);}
SteamCallHandle_t CSteamInterface::IsAccountNameInUse(const char* cszArg1, int* piArg2, TSteamError *pError){return SteamIsAccountNameInUse(/*cszArg1, piArg2, pError*/);}
int CSteamInterface::GetAppIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError){return SteamGetAppIds(puIds, uMaxIds, pError);}
int CSteamInterface::GetSubscriptionStats(TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError){return SteamGetSubscriptionStats(pSubscriptionStats, pError);}
SteamCallHandle_t CSteamInterface::RefreshAccountInfo(TSteamError *pError){return SteamRefreshAccountInfo(/*pError*/);}
SteamCallHandle_t CSteamInterface::RefreshAccountInfo(int arg1, TSteamError *pError){return SteamRefreshAccountInfo(/*pError*/);}	
SteamCallHandle_t CSteamInterface::Subscribe(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError){return SteamSubscribe(uSubscriptionId, pSubscriptionBillingInfo, pError);}
SteamCallHandle_t CSteamInterface::Unsubscribe(unsigned int uSubscriptionId, TSteamError *pError){return SteamUnsubscribe(uSubscriptionId, pError);}
int CSteamInterface::GetSubscriptionReceipt(unsigned int uSubscriptionId , TSteamSubscriptionReceipt* pSteamSubscriptionReceipt, TSteamError *pError){return SteamGetSubscriptionReceipt(uSubscriptionId , pSteamSubscriptionReceipt, pError);}
int CSteamInterface::GetAccountStatus(unsigned int* puArg1, TSteamError *pError){return SteamGetAccountStatus(/*puArg1, pError*/);}
SteamCallHandle_t CSteamInterface::SetUser(const char *cszUser, int *pbUserSet, TSteamError *pError){return SteamSetUser(cszUser, pbUserSet, pError);}
int CSteamInterface::GetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError){return SteamGetUser(szUser, uBufSize, puUserChars, pSteamGlobalUserID, pError);}
SteamCallHandle_t CSteamInterface::Login(const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError){return SteamLogin(cszUser, cszPassphrase, bIsSecureComputer, pError);}
SteamCallHandle_t CSteamInterface::AckSubscriptionReceipt(unsigned int uArg1, TSteamError *pError){return SteamAckSubscriptionReceipt(/*uArg1, pError*/);}
int CSteamInterface::IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pReserved, TSteamError *pError){return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pReserved, pError);}
int CSteamInterface::GetSubscriptionIds(unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError){return SteamGetSubscriptionIds(puIds, uMaxIds, pError);}
int CSteamInterface::EnumerateSubscription(unsigned int uSubscriptionId, TSteamSubscription *pSubscription, TSteamError *pError){return SteamEnumerateSubscription(uSubscriptionId, pSubscription, pError);}
int CSteamInterface::EnumerateSubscriptionDiscount(unsigned int uSubscriptionId, unsigned int uDiscountIdx, TSteamSubscriptionDiscount* pSteamSubscriptionDiscount, TSteamError *pError){return SteamEnumerateSubscriptionDiscount(/*uSubscriptionId, uDiscountIdx, pSteamSubscriptionDiscount, pError*/);}
int CSteamInterface::EnumerateSubscriptionDiscountQualifier(unsigned int uSubscriptionId, unsigned int uDiscountIdx, unsigned int uQualifierIdx, TSteamDiscountQualifier* pSteamDiscountQualifier, TSteamError *pError){return SteamEnumerateSubscriptionDiscountQualifier(/*uSubscriptionId, uDiscountIdx, uQualifierIdx, pSteamDiscountQualifier, pError*/);}
int CSteamInterface::EnumerateApp(unsigned int uAppId, TSteamApp *pApp, TSteamError *pError){return SteamEnumerateApp(uAppId, pApp, pError);}
int CSteamInterface::EnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError){return SteamEnumerateAppLaunchOption(uAppId, uLaunchOptionIndex, pLaunchOption, pError);}
SteamCallHandle_t CSteamInterface::DeleteAccount(TSteamError *pError){return SteamDeleteAccount(pError);}
int CSteamInterface::EnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize,  unsigned int *puSizeOfIconData, TSteamError *pError){return SteamEnumerateAppIcon(uAppId, uIconIndex, pIconData, uIconDataBufSize, puSizeOfIconData, pError);}
SteamCallHandle_t CSteamInterface::LaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError){return SteamLaunchApp(uAppId, uLaunchOption, cszArgs, pError);}
int CSteamInterface::GetCacheFilePath(unsigned int uAppId,  char* szFilePath, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError){return SteamGetCacheFilePath(uAppId, szFilePath, uBufferLength, puRecievedLength, pError);}
int CSteamInterface::EnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError){return SteamEnumerateAppVersion(uAppId, uVersionIndex, pAppVersion, pError);}
int CSteamInterface::EnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError){return SteamEnumerateAppDependency(AppId, uDependency, pDependencyInfo, pError);}
SteamCallHandle_t CSteamInterface::StartLoadingCache(unsigned int uAppId, TSteamError *pError){return SteamStartLoadingCache(uAppId, pError);}
int CSteamInterface::InsertAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError){return SteamInsertAppDependency(/*uAppId, uFileSystemIndex, pDependencyInfo, pError*/);}
int CSteamInterface::RemoveAppDependency(unsigned int uAppId, unsigned int uFileSystemIndex, TSteamError *pError){return SteamRemoveAppDependency(/*uAppId, uFileSystemIndex, pError*/);}
int CSteamInterface::FindApp(const char* cszArg1, unsigned int* puArg2, TSteamError *pError){return SteamFindApp(/*cszArg1, puArg2, pError*/);}
int CSteamInterface::GetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError *pError){return SteamGetAppDependencies(uAppId, puDependecies, uBufferLength, pError);}
int CSteamInterface::IsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pReserved, TSteamError *pError){return SteamIsSubscribed(uSubscriptionId, pbIsSubscribed, pReserved, pError);}
int CSteamInterface::GetAppUserDefinedInfo(unsigned int uAppId, const char *cszPropertyName, char *szPropertyValue, unsigned int uBufSize, unsigned int *puPropertyValueLength, TSteamError *pError){return SteamGetAppUserDefinedInfo(uAppId, cszPropertyName, szPropertyValue, uBufSize, puPropertyValueLength, pError);}
SteamCallHandle_t CSteamInterface::WaitForAppReadyToLaunch(unsigned int uAppId, TSteamError *pError){return SteamWaitForAppReadyToLaunch(uAppId, pError);}
int CSteamInterface::IsCacheLoadingEnabled(unsigned int uAppId, int *pbIsLoading, TSteamError *pError){return SteamIsCacheLoadingEnabled(uAppId, pbIsLoading, pError);}
SteamCallHandle_t CSteamInterface::StopLoadingCache(unsigned int uAppId, TSteamError *pError){return SteamStopLoadingCache(uAppId, pError);}
ESteamError CSteamInterface::GetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError){return SteamGetEncryptedUserIDTicket(pEncryptionKeyReceivedFromAppServer, uEncryptionKeyLength, pOutputBuffer, uSizeOfOutputBuffer, pReceiveSizeOfEncryptedTicket, pError);}
SteamCallHandle_t CSteamInterface::FlushCache(unsigned int uAppId, TSteamError *pError){return SteamFlushCache(uAppId, pError);}
SteamCallHandle_t CSteamInterface::RepairOrDecryptCaches(unsigned int uAppId, int iArg2, TSteamError *pError){return SteamRepairOrDecryptCaches(/*uAppId, iArg2, pError*/);}
SteamCallHandle_t CSteamInterface::LoadCacheFromDir(unsigned int uAppId, const char *cszPath, TSteamError *pError){return SteamLoadCacheFromDir(uAppId, cszPath, pError);}
int CSteamInterface::GetCacheDefaultDirectory(char *szPath, TSteamError *pError){return SteamGetCacheDefaultDirectory(szPath, pError);}
int CSteamInterface::SetCacheDefaultDirectory(const char *cszPath, TSteamError *pError){return SteamSetCacheDefaultDirectory(cszPath, pError);}
int CSteamInterface::GetAppDir(unsigned int uAppId, char* szAppDir, TSteamError *pError){/*if(bLogging && bLogFS)Logger->Write("[006] ");*/return SteamGetAppDir(uAppId, szAppDir, pError);}
SteamCallHandle_t CSteamInterface::MoveApp(unsigned int uAppId, const char *szPath, TSteamError *pError){return SteamMoveApp(uAppId, szPath, pError);}
SteamCallHandle_t CSteamInterface::GetAppCacheSize(unsigned int uAppId, unsigned int *puCacheSizeInMb, TSteamError *pError){return SteamGetAppCacheSize(uAppId, puCacheSizeInMb, pError);}
SteamCallHandle_t CSteamInterface::SetAppCacheSize(unsigned int uAppId, unsigned int uCacheSizeInMb, TSteamError *pError){return SteamSetAppCacheSize(uAppId, uCacheSizeInMb, pError);}
SteamCallHandle_t CSteamInterface::SetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError){return SteamSetAppVersion(uAppId, uAppVersionId, pError);}
SteamCallHandle_t CSteamInterface::Uninstall(TSteamError *pError){return SteamUninstall(pError);}
int CSteamInterface::SetNotificationCallback(SteamNotificationCallback_t pCallbackFunction, TSteamError *pError){return SteamSetNotificationCallback(pCallbackFunction, pError);}
SteamCallHandle_t CSteamInterface::ChangeForgottenPassword(const char* cszArg1, const char* cszArg2, const char* cszArg3, const char* cszArg4, int* piArg5, TSteamError *pError){return SteamChangeForgottenPassword(/*cszArg1, cszArg2, cszArg3, cszArg4, piArg5, pError*/);}
SteamCallHandle_t CSteamInterface::RequestForgottenPasswordEmail(const char* cszArg1, char* szArg2, TSteamError *pError){return SteamRequestForgottenPasswordEmail(/*cszArg1, szArg2, pError*/);}
SteamCallHandle_t CSteamInterface::RequestAccountsByEmailAddressEmail(const char* cszArg1, TSteamError *pError){return SteamRequestAccountsByEmailAddressEmail(/*cszArg1, pError*/);}
SteamCallHandle_t CSteamInterface::RequestAccountsByCdKeyEmail(const char* cszArg1, TSteamError *pError){return SteamRequestAccountsByCdKeyEmail(/*cszArg1, pError*/);}
SteamCallHandle_t CSteamInterface::GetNumAccountsWithEmailAddress(const char* cszArg1, unsigned int* puArg2, TSteamError *pError){return SteamGetNumAccountsWithEmailAddress(/*cszArg1, puArg2, pError*/);}
SteamCallHandle_t CSteamInterface::UpdateAccountBillingInfo(const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError){return SteamUpdateAccountBillingInfo(pPaymentCardInfo, pError);}
SteamCallHandle_t CSteamInterface::UpdateSubscriptionBillingInfo(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError){return SteamUpdateSubscriptionBillingInfo(uSubscriptionId, pSubscriptionBillingInfo, pError);}
int CSteamInterface::GetSponsorUrl(unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError){return SteamGetSponsorUrl(uAppId, szUrl, uBufSize, pUrlChars, pError);}
int CSteamInterface::GetContentServerInfo(unsigned int uAppId, unsigned int* pServerId, unsigned int* pServerIpAddress, TSteamError* pError){return SteamGetContentServerInfo(uAppId, pServerId, pServerIpAddress, pError);}
SteamCallHandle_t CSteamInterface::GetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType eSteamAppUpdateStatsQueryType, TSteamUpdateStats *pUpdateStats, TSteamError *pError){return SteamGetAppUpdateStats(uAppId, eSteamAppUpdateStatsQueryType, pUpdateStats, pError);}
int CSteamInterface::GetTotalUpdateStats(TSteamUpdateStats *pUpdateStats, TSteamError *pError){return SteamGetTotalUpdateStats(pUpdateStats, pError);}
SteamCallHandle_t CSteamInterface::CreateCachePreloaders(TSteamError *pError){return SteamCreateCachePreloaders(pError);}
//SteamInterface004 functions	
void CSteamInterface::Win32SetMiniDumpComment(const char *cszComment){return /*SteamWriteMiniDumpSetComment(cszComment)*/;}
void CSteamInterface::Win32SetMiniDumpSourceControlId(unsigned int uSourceControlId){return /*SteamWriteMiniDumpUsingExceptionInfoWithBuildId(uSourceControlId)*/;}
void CSteamInterface::Win32SetMiniDumpEnableFullMemory(){return /*SteamWin32SetMiniDumpEnableFullMemory()*/;}
void CSteamInterface::Win32WriteMiniDump(const char* cszArg1, const char* cszArg2, const char* cszArg3, const char* cszArg4, unsigned int uArg5){return /*SteamWin32WriteMiniDump(cszArg1, cszArg2, cszArg3, cszArg4, uArg5)*/;}
int CSteamInterface::GetCurrentAppId(unsigned int* puAppId, TSteamError *pError){return SteamGetCurrentAppId(puAppId, pError);}
int CSteamInterface::GetAppPurchaseCountry(unsigned int uAppId, char* szCountryCode, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError* pError){return SteamGetAppPurchaseCountry(uAppId, szCountryCode, uBufferLength, puRecievedLength, pError);}
//SteamInterface005 functions	
int CSteamInterface::GetLocalClientVersion(unsigned int* puArg1, unsigned int* puArg2, TSteamError *pError){return SteamGetLocalClientVersion(puArg1, puArg2, pError);}
int CSteamInterface::IsFileNeededByCache(unsigned int uArg1, const char* cszFileName, unsigned int uArg3, TSteamError *pError){return SteamIsFileNeededByCache(/*uArg1, cszFileName, uArg3, pError*/);}
int CSteamInterface::LoadFileToCache(unsigned int uArg1, const char* cszArg2, const void* pcvArg3, unsigned int uArg4, unsigned int uArg5, TSteamError *pError){return SteamLoadFileToCache(/*uArg1, cszArg2, pcvArg3, uArg4, uArg5, pError*/);}
int CSteamInterface::GetCacheDecryptionKey(unsigned int uAppId, char* szCacheDecryptionKey, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError){return SteamGetCacheDecryptionKey(/*uAppId, szCacheDecryptionKey, uBufferLength, puRecievedLength, pError*/);}
int CSteamInterface::GetSubscriptionExtendedInfo(unsigned int uSubscritptionId, const char* cszKeyName, char* szKeyValue, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError){return SteamGetSubscriptionExtendedInfo(/*uSubscritptionId, cszKeyName, szKeyValue, uBufferLength, puRecievedLength, pError*/);}
int CSteamInterface::GetSubscriptionPurchaseCountry(unsigned int uSubscritptionId, char* szCountry, unsigned int uBufferLength , int* piRecievedLength, TSteamError *pError){return SteamGetSubscriptionPurchaseCountry(/*uSubscritptionId, szCountry, uBufferLength , piRecievedLength, pError*/);}
int CSteamInterface::GetAppUserDefinedRecord(unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void* pvCKeyValue, TSteamError *pError){return SteamGetAppUserDefinedRecord(uAppId, AddEntryToKeyValueFunc, pvCKeyValue, pError);}
//SteamInterface006 functions	
int CSteamInterface::FindServersNumServers(ESteamServerType eSteamServerType){return SteamFindServersNumServers(eSteamServerType);}
int CSteamInterface::FindServersIterateServer(ESteamServerType eSteamServerType, int iIndex, char *szServerAddress, unsigned int uServerAddressChars){return SteamFindServersIterateServer(eSteamServerType, iIndex, szServerAddress, uServerAddressChars);}
const char* CSteamInterface::FindServersGetErrorString(){return SteamFindServersGetErrorString();}
int CSteamInterface::CheckAppOwnership(unsigned int uAppId, int* pbOwned, TSteamGlobalUserID* pSteamID, TSteamError* pError){return SteamCheckAppOwnership(uAppId, pbOwned, pSteamID, pError);}

