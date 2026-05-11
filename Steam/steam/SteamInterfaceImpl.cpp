#include "stdafx.h"
#include "Steam.h"
#include "SteamInterfaceImpl.h"

extern CLogFile* Logger;
extern bool bLogging;
extern bool bLogFS;

CSteamInterface::CSteamInterface() {}
CSteamInterface::~CSteamInterface() {}

SteamCallHandle_t CSteamInterface::ChangePassword( const char *cszCurrentPassphrase, const char *cszNewPassphrase, TSteamError *pError ) { return SteamChangePassword(cszCurrentPassphrase, cszNewPassphrase, pError); }
int CSteamInterface::GetCurrentEmailAddress( char *szEmailAddress, unsigned int uBufSize, unsigned int *puEmailChars, TSteamError *pError ) { return SteamGetCurrentEmailAddress(szEmailAddress, uBufSize, puEmailChars, pError); }
SteamCallHandle_t CSteamInterface::ChangePersonalQA( const char *cszCurrentPassphrase, const char *cszNewPersonalQuestion, const char *cszNewAnswerToQuestion, TSteamError *pError ) { return SteamChangePersonalQA(cszCurrentPassphrase, cszNewPersonalQuestion, cszNewAnswerToQuestion, pError); }
SteamCallHandle_t CSteamInterface::ChangeEmailAddress( const char *cszNewEmailAddress, TSteamError *pError ) { return SteamChangeEmailAddress(cszNewEmailAddress, pError); }
SteamCallHandle_t CSteamInterface::VerifyEmailAddress( const char *cszEmailVerificationKey, TSteamError *pError ) { return SteamVerifyEmailAddress(cszEmailVerificationKey, pError); }
SteamCallHandle_t CSteamInterface::RequestEmailAddressVerificationEmail( TSteamError *pError ) { return SteamRequestEmailAddressVerificationEmail(pError); }
SteamCallHandle_t CSteamInterface::ChangeAccountName( const char *cszCurrentPassphrase, const char *cszNewAccountName, TSteamError *pError ) { return SteamChangeAccountName(cszCurrentPassphrase, cszNewAccountName, pError); }
int CSteamInterface::MountAppFilesystem( TSteamError *pError ) { return SteamMountAppFilesystem(pError); }
int CSteamInterface::UnmountAppFilesystem( TSteamError *pError ) { return SteamUnmountAppFilesystem(pError); }
SteamHandle_t CSteamInterface::MountFilesystem( unsigned int uAppId, const char *szMountPath, TSteamError *pError ) { return SteamMountFilesystem(uAppId, szMountPath, pError); }
int CSteamInterface::UnmountFilesystem( SteamHandle_t hFs, TSteamError *pError ) { return SteamUnmountFilesystem(hFs, pError); }
int CSteamInterface::Stat( const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError ) { return SteamStat(cszName, pInfo, pError); }
int CSteamInterface::SetvBuf( SteamHandle_t hFile, void *pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError ) { return SteamSetvBuf(hFile, pBuf, eMethod, uBytes, pError); }
int CSteamInterface::FlushFile( SteamHandle_t hFile, TSteamError *pError ) { return SteamFlushFile(hFile, pError); }
SteamHandle_t CSteamInterface::OpenFile( const char *cszName, const char *cszMode, TSteamError *pError ) { return SteamOpenFile(cszName, cszMode, pError); }
SteamHandle_t CSteamInterface::OpenFileEx( const char *cszName, const char *cszMode, unsigned int *puFileSize, TSteamError *pError ) { return SteamOpenFileEx(cszName, cszMode, puFileSize, pError); }
SteamHandle_t CSteamInterface::OpenFileEx( const char *cszName, const char *cszMode, int nFlags, unsigned int *puFileSize, int *pbLocal, TSteamError *pError ) { return SteamOpenFile2(cszName, cszMode, nFlags, puFileSize, pbLocal, pError); }
SteamHandle_t CSteamInterface::OpenTmpFile( TSteamError *pError ) { return SteamOpenTmpFile(pError); }
void CSteamInterface::ClearError( TSteamError *pError ) { return SteamClearError(pError); }
int CSteamInterface::GetVersion( char *szVersion, unsigned int uVersionBufSize ) { return SteamGetVersion(szVersion, uVersionBufSize); }
int CSteamInterface::GetOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError ) { return SteamGetOfflineStatus(pStatus, pError); }
int CSteamInterface::ChangeOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError ) { return SteamChangeOfflineStatus(pStatus, pError); }
int CSteamInterface::ProcessCall( SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError ) { return SteamProcessCall(handle, pProgress, pError); }
int CSteamInterface::AbortCall( SteamCallHandle_t handle, TSteamError *pError ) { return SteamAbortCall(handle, pError); }
int CSteamInterface::BlockingCall( SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError ) { return SteamBlockingCall(handle, uiProcessTickMS, pError); }
int CSteamInterface::SetMaxStallCount( unsigned int uNumStalls, TSteamError *pError ) { return SteamSetMaxStallCount(uNumStalls, pError); }
int CSteamInterface::CloseFile( SteamHandle_t hFile, TSteamError *pError ) { return SteamCloseFile(hFile, pError); }
unsigned int CSteamInterface::ReadFile( void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError ) { return SteamReadFile(pBuf, uSize, uCount, hFile, pError); }
unsigned int CSteamInterface::WriteFile( const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError ) { return SteamWriteFile(pBuf, uSize, uCount, hFile, pError); }
int CSteamInterface::Getc( SteamHandle_t hFile, TSteamError *pError ) { return SteamGetc(hFile, pError); }
int CSteamInterface::Putc( int cChar, SteamHandle_t hFile, TSteamError *pError ) { return SteamPutc(cChar, hFile, pError); }
int CSteamInterface::SeekFile( SteamHandle_t hFile, long lOffset,  ESteamSeekMethod eMethod, TSteamError *pError ) { return SteamSeekFile(hFile, lOffset, eMethod, pError); }
long CSteamInterface::TellFile( SteamHandle_t hFile, TSteamError *pError ) { return SteamTellFile(hFile, pError); }
long CSteamInterface::SizeFile( SteamHandle_t hFile, TSteamError *pError ) { return SteamSizeFile(hFile, pError); }
SteamHandle_t CSteamInterface::FindFirst( const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError ) { return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError); }
int CSteamInterface::FindNext( SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError ) { return SteamFindNext(hDirectory, pFindInfo, pError); }
int CSteamInterface::FindClose( SteamHandle_t hDirectory, TSteamError *pError ) { return SteamFindClose(hDirectory, pError); }
int CSteamInterface::GetLocalFileCopy( const char *cszName, TSteamError *pError ) { return SteamGetLocalFileCopy(cszName, pError); }
int CSteamInterface::IsFileImmediatelyAvailable( const char *cszName, TSteamError *pError ) { return SteamIsFileImmediatelyAvailable(cszName, pError); }
int CSteamInterface::HintResourceNeed( const char *cszMasterList, int bForgetEverything, TSteamError *pError ) { return SteamHintResourceNeed(cszMasterList, bForgetEverything, pError); }
int CSteamInterface::ForgetAllHints( TSteamError *pError ) { return SteamForgetAllHints(pError); }
int CSteamInterface::PauseCachePreloading( TSteamError *pError ) { return SteamPauseCachePreloading(pError); }
int CSteamInterface::ResumeCachePreloading( TSteamError *pError ) { return SteamResumeCachePreloading(pError); }
SteamCallHandle_t CSteamInterface::WaitForResources( const char *cszMasterList, TSteamError *pError ) { return SteamWaitForResources(cszMasterList, pError); }
int CSteamInterface::StartEngine( TSteamError *pError ) { return SteamStartEngine(pError); }
int CSteamInterface::ShutdownEngine( TSteamError *pError ) { return SteamShutdownEngine(pError); }
int CSteamInterface::Startup( unsigned int uUsingMask, TSteamError *pError ) { return SteamStartup(uUsingMask, pError); }
int CSteamInterface::Cleanup( TSteamError *pError ) { return SteamCleanup(pError); }
unsigned int CSteamInterface::NumAppsRunning( TSteamError *pError ) { return SteamNumAppsRunning(pError); }
SteamCallHandle_t CSteamInterface::CreateAccount( const char *cszUser, const char *cszEmailAddress, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, int *pbCreated, TSteamError *pError ) { return SteamCreateAccount(cszUser, cszEmailAddress, cszPassphrase, cszCreationKey, cszPersonalQuestion, cszAnswerToQuestion, pbCreated, pError); }
SteamCallHandle_t CSteamInterface::GenerateSuggestedAccountNames( const char *cszAccountNameToSelectMasterAS, const char *cszGenerateNamesLikeAccountName, char *pSuggestedNamesBuf, unsigned int uBufSize, unsigned int *puNumSuggestedChars, TSteamError *pError ) { return SteamGenerateSuggestedAccountNames(cszAccountNameToSelectMasterAS, cszGenerateNamesLikeAccountName, pSuggestedNamesBuf, uBufSize, puNumSuggestedChars, pError); }
int CSteamInterface::IsLoggedIn( int *pbIsLoggedIn, TSteamError *pError ) { return SteamIsLoggedIn(pbIsLoggedIn, pError); }
SteamCallHandle_t CSteamInterface::Logout( TSteamError *pError ) { return SteamLogout(pError); }
int CSteamInterface::IsSecureComputer( int *pbIsSecure, TSteamError *pError ) { return SteamIsSecureComputer(pbIsSecure, pError); }
SteamHandle_t CSteamInterface::CreateLogContext( const char *cszName ) { return SteamCreateLogContext(cszName); }
int CSteamInterface::Log( SteamHandle_t hContext, const char *cszMsg ) { return SteamLog(hContext, cszMsg); }
void CSteamInterface::LogResourceLoadStarted( const char *cszMsg ) { return SteamLogResourceLoadStarted(cszMsg); }
void CSteamInterface::LogResourceLoadFinished( const char *cszMsg ) { return SteamLogResourceLoadFinished(cszMsg); }
SteamCallHandle_t CSteamInterface::RefreshLogin( const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError ) { return SteamRefreshLogin(cszPassphrase, bIsSecureComputer, pError); }
int CSteamInterface::VerifyPassword( const char *cszPassphrase, int *pbCorrect, TSteamError *pError ) { return SteamVerifyPassword(cszPassphrase, pbCorrect, pError); }
int CSteamInterface::GetUserType( unsigned int *puUserTypeFlags, TSteamError *pError ) { return SteamGetUserType(puUserTypeFlags, pError); }
int CSteamInterface::GetAppStats( TSteamAppStats *pAppStats, TSteamError *pError ) { return SteamGetAppStats(pAppStats, pError); }
SteamCallHandle_t CSteamInterface::IsAccountNameInUse( const char *cszAccountName, int *pbIsUsed, TSteamError *pError ) { return SteamIsAccountNameInUse(cszAccountName, pbIsUsed, pError); }
int CSteamInterface::GetAppIds( unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError ) { return SteamGetAppIds(puIds, uMaxIds, pError); }
int CSteamInterface::GetSubscriptionStats( TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError ) { return SteamGetSubscriptionStats(pSubscriptionStats, pError); }
SteamCallHandle_t CSteamInterface::RefreshAccountInfo( TSteamError *pError ) { return SteamRefreshAccountInfo(pError); }
SteamCallHandle_t CSteamInterface::RefreshAccountInfo( int bContentDescriptionOnly, TSteamError *pError ) { return SteamRefreshAccountInfoEx(bContentDescriptionOnly, pError); }
SteamCallHandle_t CSteamInterface::Subscribe( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError ) { return SteamSubscribe(uSubscriptionId, pSubscriptionBillingInfo, pError); }
SteamCallHandle_t CSteamInterface::Unsubscribe( unsigned int uSubscriptionId, TSteamError *pError ) { return SteamUnsubscribe(uSubscriptionId, pError); }
int CSteamInterface::GetSubscriptionReceipt( unsigned int uSubscriptionId, TSteamSubscriptionReceipt *pSubscriptionReceipt, TSteamError *pError ) { return SteamGetSubscriptionReceipt(uSubscriptionId, pSubscriptionReceipt, pError); }
int CSteamInterface::GetAccountStatus( unsigned int *puAccountStatusFlags, TSteamError *pError ) { return SteamGetAccountStatus(puAccountStatusFlags, pError); }
SteamCallHandle_t CSteamInterface::SetUser( const char *cszUser, int *pbUserSet, TSteamError *pError ) { return SteamSetUser(cszUser, pbUserSet, pError); }
int CSteamInterface::GetUser( char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID *pOptionalReceiveUserID, TSteamError *pError ) { return SteamGetUser(szUser, uBufSize, puUserChars, pOptionalReceiveUserID, pError); }
SteamCallHandle_t CSteamInterface::Login( const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError ) { return SteamLogin(cszUser, cszPassphrase, bIsSecureComputer, pError); }
SteamCallHandle_t CSteamInterface::AckSubscriptionReceipt( unsigned int uSubscriptionId, TSteamError *pError ) { return SteamAckSubscriptionReceipt(uSubscriptionId, pError); }
int CSteamInterface::IsAppSubscribed( unsigned int uAppId, int *pbIsAppSubscribed, int *pbIsSubscriptionPending, TSteamError *pError ) { return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pbIsSubscriptionPending, pError); }
int CSteamInterface::GetSubscriptionIds( unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError ) { return SteamGetSubscriptionIds(puIds, uMaxIds, pError); }
int CSteamInterface::EnumerateSubscription( unsigned int uId, TSteamSubscription *pSubscription, TSteamError *pError ) { return SteamEnumerateSubscription(uId, pSubscription, pError); }
int CSteamInterface::EnumerateSubscriptionDiscount( unsigned int uSubscriptionId, unsigned int uDiscountIndex, TSteamSubscriptionDiscount *pDiscount, TSteamError *pError ) { return SteamEnumerateSubscriptionDiscount(uSubscriptionId, uDiscountIndex, pDiscount, pError); }
int CSteamInterface::EnumerateSubscriptionDiscountQualifier( unsigned int uSubscriptionId, unsigned int uDiscountIndex, unsigned int uQualifierIndex, TSteamDiscountQualifier *pDiscountQualifier, TSteamError *pError ) { return SteamEnumerateSubscriptionDiscountQualifier(uSubscriptionId, uDiscountIndex, uQualifierIndex, pDiscountQualifier, pError); }
int CSteamInterface::EnumerateApp( unsigned int uId, TSteamApp *pApp, TSteamError *pError ) { return SteamEnumerateApp(uId, pApp, pError); }
int CSteamInterface::EnumerateAppLaunchOption( unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError ) { return SteamEnumerateAppLaunchOption(uAppId, uLaunchOptionIndex, pLaunchOption, pError); }
SteamCallHandle_t CSteamInterface::DeleteAccount( TSteamError *pError ) { return SteamDeleteAccount(pError); }
int CSteamInterface::EnumerateAppIcon( unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize, unsigned int *puSizeOfIconData, TSteamError *pError ) { return SteamEnumerateAppIcon(uAppId, uIconIndex, pIconData, uIconDataBufSize, puSizeOfIconData, pError); }
SteamCallHandle_t CSteamInterface::LaunchApp( unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError ) { return SteamLaunchApp(uAppId, uLaunchOption, cszArgs, pError); }
int CSteamInterface::GetCacheFilePath( unsigned int uCacheId, char *szPathBuf, unsigned int uBufSize, unsigned int *puPathChars, TSteamError *pError ) { return SteamGetCacheFilePath(uCacheId, szPathBuf, uBufSize, puPathChars, pError); }
int CSteamInterface::EnumerateAppVersion( unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError ) { return SteamEnumerateAppVersion(uAppId, uVersionIndex, pAppVersion, pError); }
int CSteamInterface::EnumerateAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError ) { return SteamEnumerateAppDependency(uAppId, uIndex, pDependencyInfo, pError); }
SteamCallHandle_t CSteamInterface::StartLoadingCache( unsigned int uAppId, TSteamError *pError ) { return SteamStartLoadingCache(uAppId, pError); }
int CSteamInterface::InsertAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError ) { return SteamInsertAppDependency(uAppId, uIndex, pDependencyInfo, pError); }
int CSteamInterface::RemoveAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamError *pError ) { return SteamRemoveAppDependency(uAppId, uIndex, pError); }
int CSteamInterface::FindApp( const char *cszAppName, unsigned int *puAppId, TSteamError *pError ) { return SteamFindApp(cszAppName, puAppId, pError); }
int CSteamInterface::GetAppDependencies( unsigned int uAppId, unsigned int *puCacheIds, unsigned int uMaxIds, TSteamError *pError ) { return SteamGetAppDependencies(uAppId, puCacheIds, uMaxIds, pError); }
int CSteamInterface::IsSubscribed( unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbIsSubscriptionPending, TSteamError *pError ) { return SteamIsSubscribed(uSubscriptionId, pbIsSubscribed, pbIsSubscriptionPending, pError); }
int CSteamInterface::GetAppUserDefinedInfo( unsigned int uAppId, const char *cszKey, char *szValueBuf, unsigned int uValueBufLen, unsigned int *puValueLen, TSteamError *pError ) { return SteamGetAppUserDefinedInfo(uAppId, cszKey, szValueBuf, uValueBufLen, puValueLen, pError); }
SteamCallHandle_t CSteamInterface::WaitForAppReadyToLaunch( unsigned int uAppId, TSteamError *pError ) { return SteamWaitForAppReadyToLaunch(uAppId, pError); }
int CSteamInterface::IsCacheLoadingEnabled( unsigned int uAppId, int *pbIsLoading, TSteamError *pError ) { return SteamIsCacheLoadingEnabled(uAppId, pbIsLoading, pError); }
SteamCallHandle_t CSteamInterface::StopLoadingCache( unsigned int uAppId, TSteamError *pError ) { return SteamStopLoadingCache(uAppId, pError); }
ESteamError CSteamInterface::GetEncryptedUserIDTicket( const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError ) { return SteamGetEncryptedUserIDTicket(pEncryptionKeyReceivedFromAppServer, uEncryptionKeyLength, pOutputBuffer, uSizeOfOutputBuffer, pReceiveSizeOfEncryptedTicket, pError); }
SteamCallHandle_t CSteamInterface::FlushCache( unsigned int uCacheId, TSteamError *pError ) { return SteamFlushCache(uCacheId, pError); }
SteamCallHandle_t CSteamInterface::RepairOrDecryptCaches( unsigned int uAppId, int bForceValidation, TSteamError *pError ) { return SteamRepairOrDecryptCaches(uAppId, bForceValidation, pError); }
SteamCallHandle_t CSteamInterface::LoadCacheFromDir( unsigned int uAppId, const char *szPath, TSteamError *pError ) { return SteamLoadCacheFromDir(uAppId, szPath, pError); }
int CSteamInterface::GetCacheDefaultDirectory( char *szPath, TSteamError *pError ) { return SteamGetCacheDefaultDirectory(szPath, pError); }
int CSteamInterface::SetCacheDefaultDirectory( const char *szPath, TSteamError *pError ) { return SteamSetCacheDefaultDirectory(szPath, pError); }
int CSteamInterface::GetAppDir( unsigned int uAppId, char *szPath, TSteamError *pError ) { return SteamGetAppDir(uAppId, szPath, pError); }
SteamCallHandle_t CSteamInterface::MoveApp( unsigned int uAppId, const char *szPath, TSteamError *pError ) { return SteamMoveApp(uAppId, szPath, pError); }
SteamCallHandle_t CSteamInterface::GetAppCacheSize( unsigned int uCacheId, unsigned int *pCacheSizeInMb, TSteamError *pError ) { return SteamGetAppCacheSize(uCacheId, pCacheSizeInMb, pError); }
SteamCallHandle_t CSteamInterface::SetAppCacheSize( unsigned int uCacheId, unsigned int nCacheSizeInMb, TSteamError *pError ) { return SteamSetAppCacheSize(uCacheId, nCacheSizeInMb, pError); }
SteamCallHandle_t CSteamInterface::SetAppVersion( unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError ) { return SteamSetAppVersion(uAppId, uAppVersionId, pError); }
SteamCallHandle_t CSteamInterface::Uninstall( TSteamError *pError ) { return SteamUninstall(pError); }
int CSteamInterface::SetNotificationCallback( SteamNotificationCallback_t pCallbackFunction, TSteamError *pError ) { return SteamSetNotificationCallback(pCallbackFunction, pError); }
SteamCallHandle_t CSteamInterface::ChangeForgottenPassword( const char *cszUser, const char *cszAnswerToQuestion, const char *cszEmailVerificationKey, const char *cszNewPassphrase, int *pbChanged, TSteamError *pError ) { return SteamChangeForgottenPassword(cszUser, cszAnswerToQuestion, cszEmailVerificationKey, cszNewPassphrase, pbChanged, pError); }
SteamCallHandle_t CSteamInterface::RequestForgottenPasswordEmail( const char *cszUser, SteamPersonalQuestion_t ReceivePersonalQuestion, TSteamError *pError ) { return SteamRequestForgottenPasswordEmail(cszUser, ReceivePersonalQuestion, pError); }
SteamCallHandle_t CSteamInterface::RequestAccountsByEmailAddressEmail( const char *cszEmailAddress, TSteamError *pError ) { return SteamRequestAccountsByEmailAddressEmail(cszEmailAddress, pError); }
SteamCallHandle_t CSteamInterface::RequestAccountsByCdKeyEmail( const char *cszCdKey, TSteamError *pError ) { return SteamRequestAccountsByCdKeyEmail(cszCdKey, pError); }
SteamCallHandle_t CSteamInterface::GetNumAccountsWithEmailAddress( const char *cszEmailAddress, unsigned int *puNumAccounts, TSteamError *pError ) { return SteamGetNumAccountsWithEmailAddress(cszEmailAddress, puNumAccounts, pError); }
SteamCallHandle_t CSteamInterface::UpdateAccountBillingInfo( const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError ) { return SteamUpdateAccountBillingInfo(pPaymentCardInfo, pError); }
SteamCallHandle_t CSteamInterface::UpdateSubscriptionBillingInfo( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError ) { return SteamUpdateSubscriptionBillingInfo(uSubscriptionId, pSubscriptionBillingInfo, pError); }
int CSteamInterface::GetSponsorUrl( unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError ) { return SteamGetSponsorUrl(uAppId, szUrl, uBufSize, pUrlChars, pError); }
int CSteamInterface::GetContentServerInfo( unsigned int uAppId, unsigned int *puServerId, unsigned int *puServerIpAddress, TSteamError *pError ) { return SteamGetContentServerInfo(uAppId, puServerId, puServerIpAddress, pError); }
SteamCallHandle_t CSteamInterface::GetAppUpdateStats( unsigned int uAppOrCacheId, ESteamAppUpdateStatsQueryType eQueryType, TSteamUpdateStats *pUpdateStats, TSteamError *pError ) { return SteamGetAppUpdateStats(uAppOrCacheId, eQueryType, pUpdateStats, pError); }
int CSteamInterface::GetTotalUpdateStats( TSteamUpdateStats *pUpdateStats, TSteamError *pError ) { return SteamGetTotalUpdateStats(pUpdateStats, pError); }
SteamCallHandle_t CSteamInterface::CreateCachePreloaders( TSteamError *pError ) { return SteamCreateCachePreloaders(pError); }
//SteamInterface004 functions
void CSteamInterface::Win32SetMiniDumpComment( const char *comment ) { return; }
void CSteamInterface::Win32SetMiniDumpSourceControlId( unsigned int SourcecontrolID ) { return; }
void CSteamInterface::Win32SetMiniDumpEnableFullMemory() { return; }
void CSteamInterface::Win32WriteMiniDump( const char *szErrorOrAssertType, const char *szDescriptionOrAssertName, const char *szAssertExpr, const char *szAssertFilename, unsigned int uAssertLineNumber ) { return; }
int CSteamInterface::GetCurrentAppId( unsigned int *puAppId, TSteamError *pError ) { return SteamGetCurrentAppId(puAppId, pError); }
int CSteamInterface::GetAppPurchaseCountry( unsigned int uAppId, char *szCountry, unsigned int uBufSize, int *pPurchaseTime, TSteamError *pError ) { return SteamGetAppPurchaseCountry(uAppId, szCountry, uBufSize, pPurchaseTime, pError); }
//SteamInterface005 functions
int CSteamInterface::GetLocalClientVersion( unsigned int *puBootstrapperVersion, unsigned int *puClientVersion, TSteamError *pError ) { return SteamGetLocalClientVersion(puBootstrapperVersion, puClientVersion, pError); }
int CSteamInterface::IsFileNeededByCache( unsigned int uCacheId, const char *pchFileName, unsigned int uFileSize, TSteamError *pError ) { return SteamIsFileNeededByCache(uCacheId, pchFileName, uFileSize, pError); }
int CSteamInterface::LoadFileToCache( unsigned int uCacheId, const char *pchFileName, const void *pubDataChunk, unsigned int cubDataChunk, unsigned int cubDataOffset, TSteamError *pError ) { return SteamLoadFileToCache(uCacheId, pchFileName, pubDataChunk, cubDataChunk, cubDataOffset, pError); }
int CSteamInterface::GetCacheDecryptionKey( unsigned int uCacheId, char *pchKeyBuffer, unsigned int cubBuff, unsigned int *pcubKey, TSteamError *pError ) { return SteamGetCacheDecryptionKey(uCacheId, pchKeyBuffer, cubBuff, pcubKey, pError); }
int CSteamInterface::GetSubscriptionExtendedInfo( unsigned int uSubscriptionId, const char *cszKey, char *szValueBuf, unsigned int uValueBufLen, unsigned *puValueLen, TSteamError *pError ) { return SteamGetSubscriptionExtendedInfo(uSubscriptionId, cszKey, szValueBuf, uValueBufLen, puValueLen, pError); }
int CSteamInterface::GetSubscriptionPurchaseCountry( unsigned int uSubscriptionId, char *szCountry, unsigned int uBufSize, int *pPurchaseTime, TSteamError *pError ) { return SteamGetSubscriptionPurchaseCountry(uSubscriptionId, szCountry, uBufSize, pPurchaseTime, pError); }
int CSteamInterface::GetAppUserDefinedRecord( unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void *pvCKeyValue, TSteamError *pError ) { return SteamGetAppUserDefinedRecord(uAppId, AddEntryToKeyValueFunc, pvCKeyValue, pError); }
//SteamInterface006 functions
int CSteamInterface::FindServersNumServers( ESteamServerType eServerType ) { return SteamFindServersNumServers(eServerType); }
int CSteamInterface::FindServersIterateServer( ESteamServerType eServerType, unsigned int nServer, char *szIpAddrPort, int szIpAddrPortLen ) { return SteamFindServersIterateServer(eServerType, nServer, szIpAddrPort, szIpAddrPortLen); }
const char *CSteamInterface::FindServersGetErrorString() { return SteamFindServersGetErrorString(); }
int CSteamInterface::CheckAppOwnership( unsigned int uAppId, int *pbOwned, TSteamGlobalUserID *pSteamID, TSteamError *pError ) { return SteamCheckAppOwnership(uAppId, pbOwned, pSteamID, pError); }
