#ifndef INCLUDED_STEAM_H
#define INCLUDED_STEAM_H

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef INCLUDED_STEAM_COMMON_STEAMCOMMON_H
	#include "SteamCommon.h"
#endif

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the STEAM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// STEAM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
**
** Exported function prototypes
**
******************************************************************************/

/*
** Interface
*/

STEAM_API void*					STEAM_CALL	CreateInterface(const char* cszSteamDLLAppsystemInterfaceVersion, int* pReturnCode);
STEAM_API void*					STEAM_CALL	_f(const char* cszSteamInterface);

/*
** Initialization
*/

STEAM_API int					STEAM_CALL	SteamStartEngine( TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamStartup( unsigned int uUsingMask, TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamCleanup( TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamGetVersion( char *szVersion, unsigned int uVersionBufSize );
STEAM_API int					STEAM_CALL	SteamShutdownEngine( TSteamError *pError );

/*
** Asynchrounous call handling
*/

STEAM_API int					STEAM_CALL	SteamProcessCall( SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamAbortCall( SteamCallHandle_t handle, TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamBlockingCall( SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError );
STEAM_API int					STEAM_CALL	SteamSetMaxStallCount( unsigned int uNumStalls, TSteamError *pError );
							
/*
** Filesystem
*/

STEAM_API int					STEAM_CALL	SteamMountAppFilesystem(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamUnmountAppFilesystem(TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamMountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamUnmountFilesystem(unsigned int uAppId, TSteamError *pError);
STEAM_API SteamHandle_t			STEAM_CALL	SteamOpenFileEx(const char *cszFileName, const char *cszMode, unsigned int *puFileSize, TSteamError *pError);
STEAM_API SteamHandle_t			STEAM_CALL	SteamOpenFile(const char *cszFileName, const char *cszMode, TSteamError *pError);
STEAM_API SteamHandle_t			STEAM_CALL	SteamOpenTmpFile(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamFlushFile(SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamCloseFile(SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamSetvBuf(SteamHandle_t hFile, void* pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetc(SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamPutc(int cChar, SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamPrintFile(SteamHandle_t hFile, TSteamError *pError, const char *cszFormat, ...);
STEAM_API unsigned int			STEAM_CALL	SteamReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
STEAM_API unsigned int			STEAM_CALL	SteamWriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamSeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod esMethod, TSteamError *pError);
STEAM_API long					STEAM_CALL	SteamSizeFile(SteamHandle_t hFile, TSteamError *pError);
STEAM_API long					STEAM_CALL	SteamTellFile(SteamHandle_t hFile, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamStat(const char *cszFileName, TSteamElemInfo *pInfo, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamFindClose(SteamHandle_t hDirectory, TSteamError *pError);
STEAM_API SteamHandle_t			STEAM_CALL	SteamFindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamFindNext(SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetLocalFileCopy(const char *cszFileName, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsFileImmediatelyAvailable(const char *cszName, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamHintResourceNeed(const char *cszHintList, int bForgetEverything, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamForgetAllHints(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamPauseCachePreloading(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamResumeCachePreloading(TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamWaitForResources(const char *cszMasterList, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamFlushCache(unsigned int uAppId, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetCacheDecryptionKey();
STEAM_API int					STEAM_CALL	SteamGetCacheDefaultDirectory(char *szPath, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamSetCacheDefaultDirectory(const char *szPath, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetCacheFilePath(unsigned int uAppId,  char* szFilePath, unsigned int uBufferLength, unsigned int* puRecievedLength, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsFileNeededByCache();
STEAM_API int					STEAM_CALL	SteamRepairOrDecryptCaches();
STEAM_API int					STEAM_CALL	SteamCreateCachePreloaders(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsCacheLoadingEnabled(unsigned int uAppId, int *pbIsLoading, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamLoadCacheFromDir(unsigned int uAppId, const char *szPath, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamLoadFileToCache();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamStartLoadingCache(unsigned int uAppId, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamStopLoadingCache(unsigned int uAppId, TSteamError *pError);

/*
** Logging
*/

STEAM_API SteamHandle_t			STEAM_CALL	SteamCreateLogContext( const char *cszName );
STEAM_API int					STEAM_CALL	SteamLog( SteamHandle_t hContext, const char *cszMsg );
STEAM_API void					STEAM_CALL	SteamLogResourceLoadStarted( const char *cszMsg );
STEAM_API void					STEAM_CALL	SteamLogResourceLoadFinished( const char *cszMsg );

/*
** Account
*/

STEAM_API SteamCallHandle_t		STEAM_CALL	SteamCreateAccount(const char *cszUser, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, const char* cszArg6, int *pbCreated, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamDeleteAccount(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamChangeAccountName(const char* cszCurrentAccountName, const char* cszNewAccountName, TSteamError* pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamChangeEmailAddress(const char* cszNewEmailAddress, TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamChangeForgottenPassword();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamChangePassword(const char* cszCurrentPassphrase, const char* cszNewPassphrase, TSteamError* pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamChangePersonalQA(const char* cszCurrentPassphrase, const char* cszNewPersonalQuestion, const char* cszNewAnswerToQuestion, TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamEnumerateApp(unsigned int AppId, TSteamApp *pApp, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateAppIcon(unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize, unsigned int *puSizeOfIconData, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateAppLaunchOption(unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateAppVersion(unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateSubscription(unsigned int uSubId, TSteamSubscription *pSubscription, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamEnumerateSubscriptionDiscount();
STEAM_API int					STEAM_CALL	SteamEnumerateSubscriptionDiscountQualifier();
STEAM_API int					STEAM_CALL	SteamGenerateSuggestedAccountNames(const char* cszArg1, const char* cszArg2, char* szArg3, unsigned int uArg4, unsigned int* puArg5, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAccountStatus();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamGetAppCacheSize(unsigned int uAppId, unsigned int *pCacheSizeInMb, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppDependencies(unsigned int uAppId, unsigned int* puDependecies, unsigned int uBufferLength, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppDir(unsigned int uAppId, char* szAppDir, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppIds(unsigned int *puAppIds, unsigned int uMaxIds, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppPurchaseCountry(unsigned int uAppId, char* szCountryBuf, unsigned int uBufSize, unsigned int* pPurchaseTime, TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamGetAppStats(TSteamAppStats *pAppStats, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppUpdateStats(unsigned int uAppId, ESteamAppUpdateStatsQueryType uStatType, TSteamUpdateStats *pUpdateStats, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppUserDefinedInfo(unsigned int uAppId, const char *cszPropertyName, char *szPropertyValue, unsigned int uBufSize, unsigned int *puPropertyValueLength, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetAppUserDefinedRecord(unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void* pvCKeyValue, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetCurrentEmailAddress(char *szEmailaddress, unsigned int uBufSize, unsigned int *puEmailaddressChars, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetNumAccountsWithEmailAddress();
STEAM_API int					STEAM_CALL	SteamGetSponsorUrl(unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetSubscriptionExtendedInfo();
STEAM_API int					STEAM_CALL	SteamGetSubscriptionIds(unsigned int *puSubIds, unsigned int uMaxIds, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetSubscriptionPurchaseCountry();
STEAM_API int					STEAM_CALL	SteamGetSubscriptionReceipt(unsigned int uSubscriptionId , TSteamSubscriptionReceipt* pSteamSubscriptionReceipt, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetSubscriptionStats(TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetTotalUpdateStats(TSteamUpdateStats *pUpdateStats, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamGetUserType(unsigned int* puArg1, TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamIsAccountNameInUse();
STEAM_API int					STEAM_CALL	SteamIsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbIsSubscriptionPending, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsLoggedIn(int *pbIsLoggedIn, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsSecureComputer(int *pbIsSecure, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamIsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbIsSubscriptionPending, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamLaunchApp(unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamLogin(const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamLogout(TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamMoveApp(unsigned int uAppId, const char *szPath, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamRefreshAccountInfo();
STEAM_API int					STEAM_CALL	SteamRefreshAccountInfoEx();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamRefreshLogin(const char *cszPassphrase, int bIsSecureComputer, TSteamError * pError);
STEAM_API int					STEAM_CALL	SteamRequestAccountsByCdKeyEmail();
STEAM_API int					STEAM_CALL	SteamRequestAccountsByEmailAddressEmail();
STEAM_API int					STEAM_CALL	SteamRequestEmailAddressVerificationEmail(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamRequestForgottenPasswordEmail();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamSetUser(const char *cszUser, int *pbUserSet, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamSubscribe(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamUnsubscribe(unsigned int uSubscriptionId, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamUpdateAccountBillingInfo(const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamUpdateSubscriptionBillingInfo(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamVerifyEmailAddress(const char* cszEmailAddress, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamVerifyPassword(const char* cszArg1, int* piArg2, TSteamError* pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamWaitForAppReadyToLaunch(unsigned int uAppId, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamAckSubscriptionReceipt();
STEAM_API int					STEAM_CALL	SteamRemoveAppDependency();
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamSetAppCacheSize(unsigned int uAppId, unsigned int nCacheSizeInMb, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamSetAppVersion(unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamInsertAppDependency();
STEAM_API int					STEAM_CALL	SteamNumAppsRunning(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamFindApp();

#ifdef _WIN32
/*
** Minidump
*/

STEAM_API int					STEAM_CALL	SteamWriteMiniDumpFromAssert();
STEAM_API int					STEAM_CALL	SteamWriteMiniDumpSetComment(const char *cszComment);
STEAM_API void					STEAM_CALL	SteamWriteMiniDumpUsingExceptionInfo( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers );
STEAM_API void					STEAM_CALL	SteamWriteMiniDumpUsingExceptionInfoWithBuildId( DWORD dwExceptionCode, _EXCEPTION_POINTERS* pStructuredExceptionPointers, unsigned int uSourceControlID );
#endif

/*
** User ID functions
*/

STEAM_API ESteamError			STEAM_CALL	SteamGetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError);
STEAM_API ESteamError			STEAM_CALL	SteamInitializeUserIDTicketValidator(const char * pszOptionalPublicEncryptionKeyFilename, const char *	pszOptionalPrivateDecryptionKeyFilename, unsigned int ClientClockSkewToleranceInSeconds, unsigned int ServerClockSkewToleranceInSeconds, unsigned int MaxNumLoginsWithinClientClockSkewTolerancePerClient, unsigned int	HintPeakSimultaneousValidations, unsigned int AbortValidationAfterStallingForNProcessSteps);
STEAM_API ESteamError			STEAM_CALL	SteamShutdownUserIDTicketValidator();
STEAM_API const unsigned char*	STEAM_CALL	SteamGetEncryptionKeyToSendToNewClient(unsigned int * pReceiveSizeOfEncryptionKey);
STEAM_API ESteamError			STEAM_CALL	SteamStartValidatingUserIDTicket(void *pEncryptedUserIDTicketFromClient, unsigned int uSizeOfEncryptedUserIDTicketFromClient, unsigned int ObservedClientIPAddr, SteamUserIDTicketValidationHandle_t *pReceiveHandle);
STEAM_API ESteamError			STEAM_CALL	SteamStartValidatingNewValveCDKey(void *pEncryptedNewValveCDKeyFromClient, unsigned int uSizeOfEncryptedNewValveCDKeyFromClient, unsigned int ObservedClientIPAddr, struct sockaddr *pPrimaryValidateNewCDKeyServerSockAddr, struct sockaddr *pSecondaryValidateNewCDKeyServerSockAddr, SteamUserIDTicketValidationHandle_t *pReceiveHandle);
STEAM_API ESteamError			STEAM_CALL	SteamProcessOngoingUserIDTicketValidation(SteamUserIDTicketValidationHandle_t Handle, TSteamGlobalUserID *pReceiveValidSteamGlobalUserID, unsigned int *pReceiveClientLocalIPAddr, unsigned char *pOptionalReceiveProofOfAuthenticationToken, size_t SizeOfOptionalAreaToReceiveProofOfAuthenticationToken, size_t *pOptionalReceiveSizeOfProofOfAuthenticationToken);
STEAM_API void					STEAM_CALL	SteamAbortOngoingUserIDTicketValidation(SteamUserIDTicketValidationHandle_t Handle);
STEAM_API ESteamError			STEAM_CALL	SteamOptionalCleanUpAfterClientHasDisconnected(unsigned int ObservedClientIPAddr, unsigned int ClientLocalIPAddr);

/*
** Misc
*/

STEAM_API void					STEAM_CALL	SteamClearError(TSteamError *pError);
STEAM_API int					STEAM_CALL	InternalSteamNumClientsConnectedToEngine();
STEAM_API int					STEAM_CALL	InternalSteamShouldShutdownEngine2();
STEAM_API int					STEAM_CALL	SteamGetLocalClientVersion(unsigned int* puArg1, unsigned int* puArg2, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamChangeOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError);
STEAM_API int					STEAM_CALL	SteamGetOfflineStatus(TSteamOfflineStatus* pSteamOfflineStatus, TSteamError* pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamUninstall(TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamWeakVerifyNewValveCDKey();
STEAM_API int					STEAM_CALL	SteamGetEncryptedNewValveCDKey();
STEAM_API int					STEAM_CALL	SteamDecryptDataForThisMachine();
STEAM_API int					STEAM_CALL	SteamEncryptDataForThisMachine();
STEAM_API const char*			STEAM_CALL	SteamFindServersGetErrorString();
STEAM_API int					STEAM_CALL	SteamFindServersIterateServer(ESteamServerType eSteamServerType, unsigned int uIndex, char *szServerAddress, int iServerAddressChars);
STEAM_API int					STEAM_CALL	SteamFindServersNumServers(ESteamServerType eSteamServerType);
STEAM_API int					STEAM_CALL	SteamGetContentServerInfo(unsigned int uAppId, unsigned int* pServerId, unsigned int* pServerIpAddress, TSteamError *pError);
STEAM_API SteamCallHandle_t		STEAM_CALL	SteamRefreshMinimumFootprintFiles(unsigned int uAppId, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamSetNotificationCallback(SteamNotificationCallback_t pCallbackFunction, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamWasBlobRegistryDeleted(int *puWasDeleted, TSteamError *pError);
STEAM_API int					STEAM_CALL	SteamCheckAppOwnership(unsigned int uAppId, int* pbOwned, TSteamGlobalUserID* pSteamID, TSteamError* pError);

#ifdef __cplusplus
}
#endif

SteamHandle_t SteamOpenFile2(const char* cszFileName, const char* cszMode, int nFlags, unsigned int* puFileSize, int* pbLocal, TSteamError* pError);
int SteamGetCurrentAppId(unsigned int* puAppId, TSteamError* pError);

#endif
