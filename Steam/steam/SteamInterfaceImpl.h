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

	SteamCallHandle_t ChangePassword( const char *cszCurrentPassphrase, const char *cszNewPassphrase, TSteamError *pError );
	int GetCurrentEmailAddress( char *szEmailAddress, unsigned int uBufSize, unsigned int *puEmailChars, TSteamError *pError );
	SteamCallHandle_t ChangePersonalQA( const char *cszCurrentPassphrase, const char *cszNewPersonalQuestion, const char *cszNewAnswerToQuestion, TSteamError *pError );
	SteamCallHandle_t ChangeEmailAddress( const char *cszNewEmailAddress, TSteamError *pError );
	SteamCallHandle_t VerifyEmailAddress( const char *cszEmailVerificationKey, TSteamError *pError );
	SteamCallHandle_t RequestEmailAddressVerificationEmail( TSteamError *pError );
	SteamCallHandle_t ChangeAccountName( const char *cszCurrentPassphrase, const char *cszNewAccountName, TSteamError *pError );
	int MountAppFilesystem( TSteamError *pError );
	int UnmountAppFilesystem( TSteamError *pError );
	SteamHandle_t MountFilesystem( unsigned int uAppId, const char *szMountPath, TSteamError *pError );
	int UnmountFilesystem( SteamHandle_t hFs, TSteamError *pError );
	int Stat( const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError );
	int SetvBuf( SteamHandle_t hFile, void *pBuf, ESteamBufferMethod eMethod, unsigned int uBytes, TSteamError *pError );
	int FlushFile( SteamHandle_t hFile, TSteamError *pError );
	SteamHandle_t OpenFile( const char *cszName, const char *cszMode, TSteamError *pError );
	SteamHandle_t OpenFileEx( const char *cszName, const char *cszMode, unsigned int *puFileSize, TSteamError *pError );
	SteamHandle_t OpenFileEx( const char *cszName, const char *cszMode, int nFlags, unsigned int *puFileSize, int *pbLocal, TSteamError *pError );
	SteamHandle_t OpenTmpFile( TSteamError *pError );
	void ClearError( TSteamError *pError );
	int GetVersion( char *szVersion, unsigned int uVersionBufSize );
	int GetOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError );
	int ChangeOfflineStatus( TSteamOfflineStatus *pStatus, TSteamError *pError );
	int ProcessCall( SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError );
	int AbortCall( SteamCallHandle_t handle, TSteamError *pError );
	int BlockingCall( SteamCallHandle_t handle, unsigned int uiProcessTickMS, TSteamError *pError );
	int SetMaxStallCount( unsigned int uNumStalls, TSteamError *pError );
	int CloseFile( SteamHandle_t hFile, TSteamError *pError );
	unsigned int ReadFile( void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError );
	unsigned int WriteFile( const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError );
	int Getc( SteamHandle_t hFile, TSteamError *pError );
	int Putc( int cChar, SteamHandle_t hFile, TSteamError *pError );
	int SeekFile( SteamHandle_t hFile, long lOffset,  ESteamSeekMethod, TSteamError *pError );
	long TellFile( SteamHandle_t hFile, TSteamError *pError );
	long SizeFile( SteamHandle_t hFile, TSteamError *pError );
	SteamHandle_t FindFirst( const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError );
	int FindNext( SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError );
	int FindClose( SteamHandle_t hDirectory, TSteamError *pError );
	int GetLocalFileCopy( const char *cszName, TSteamError *pError );
	int IsFileImmediatelyAvailable( const char *cszName, TSteamError *pError );
	int HintResourceNeed( const char *cszMasterList, int bForgetEverything, TSteamError *pError );
	int ForgetAllHints( TSteamError *pError );
	int PauseCachePreloading( TSteamError *pError );
	int ResumeCachePreloading( TSteamError *pError );
	SteamCallHandle_t WaitForResources( const char *cszMasterList, TSteamError *pError );
	int StartEngine( TSteamError *pError );
	int ShutdownEngine( TSteamError *pError );
	int Startup( unsigned int uUsingMask, TSteamError *pError );
	int Cleanup( TSteamError *pError );
	unsigned int NumAppsRunning( TSteamError *pError );
	SteamCallHandle_t CreateAccount( const char *cszUser, const char *cszEmailAddress, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, int *pbCreated, TSteamError *pError );
	SteamCallHandle_t GenerateSuggestedAccountNames( const char *cszAccountNameToSelectMasterAS, const char *cszGenerateNamesLikeAccountName, char *pSuggestedNamesBuf, unsigned int uBufSize, unsigned int *puNumSuggestedChars, TSteamError *pError );
	int IsLoggedIn( int *pbIsLoggedIn, TSteamError *pError );
	SteamCallHandle_t Logout( TSteamError *pError );
	int IsSecureComputer( int *pbIsSecure, TSteamError *pError );
	SteamHandle_t CreateLogContext( const char *cszName );
	int Log( SteamHandle_t hContext, const char *cszMsg );
	void LogResourceLoadStarted( const char *cszMsg );
	void LogResourceLoadFinished( const char *cszMsg );
	SteamCallHandle_t RefreshLogin( const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError );
	int VerifyPassword( const char *cszPassphrase, int *pbCorrect, TSteamError *pError );
	int GetUserType( unsigned int *puUserTypeFlags, TSteamError *pError );
	int GetAppStats( TSteamAppStats *pAppStats, TSteamError *pError );
	SteamCallHandle_t IsAccountNameInUse( const char *cszAccountName, int *pbIsUsed, TSteamError *pError );
	int GetAppIds( unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError );
	int GetSubscriptionStats( TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError );
	SteamCallHandle_t RefreshAccountInfo( TSteamError *pError );
	SteamCallHandle_t RefreshAccountInfo( int bContentDescriptionOnly, TSteamError *pError );
	SteamCallHandle_t Subscribe( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError );
	SteamCallHandle_t Unsubscribe( unsigned int uSubscriptionId, TSteamError *pError );
	int GetSubscriptionReceipt( unsigned int uSubscriptionId, TSteamSubscriptionReceipt *pSubscriptionReceipt, TSteamError *pError );
	int GetAccountStatus( unsigned int *puAccountStatusFlags, TSteamError *pError );
	SteamCallHandle_t SetUser( const char *cszUser, int *pbUserSet, TSteamError *pError );
	int GetUser( char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID *pOptionalReceiveUserID, TSteamError *pError );
	SteamCallHandle_t Login( const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError );
	SteamCallHandle_t AckSubscriptionReceipt( unsigned int uSubscriptionId, TSteamError *pError );
	int IsAppSubscribed( unsigned int uAppId, int *pbIsAppSubscribed, int *pbIsSubscriptionPending, TSteamError *pError );
	int GetSubscriptionIds( unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError );
	int EnumerateSubscription( unsigned int uId, TSteamSubscription *pSubscription, TSteamError *pError );
	int EnumerateSubscriptionDiscount( unsigned int uSubscriptionId, unsigned int uDiscountIndex, TSteamSubscriptionDiscount *pDiscount, TSteamError *pError );
	int EnumerateSubscriptionDiscountQualifier( unsigned int uSubscriptionId, unsigned int uDiscountIndex, unsigned int uQualifierIndex, TSteamDiscountQualifier *pDiscountQualifier, TSteamError *pError );
	int EnumerateApp( unsigned int uId, TSteamApp *pApp, TSteamError *pError );
	int EnumerateAppLaunchOption( unsigned int uAppId, unsigned int uLaunchOptionIndex, TSteamAppLaunchOption *pLaunchOption, TSteamError *pError );
	SteamCallHandle_t DeleteAccount( TSteamError *pError );
	int EnumerateAppIcon( unsigned int uAppId, unsigned int uIconIndex, unsigned char *pIconData, unsigned int uIconDataBufSize, unsigned int *puSizeOfIconData, TSteamError *pError );
	SteamCallHandle_t LaunchApp( unsigned int uAppId, unsigned int uLaunchOption, const char *cszArgs, TSteamError *pError );
	int GetCacheFilePath( unsigned int uCacheId, char *szPathBuf, unsigned int uBufSize, unsigned int *puPathChars, TSteamError *pError );
	int EnumerateAppVersion( unsigned int uAppId, unsigned int uVersionIndex, TSteamAppVersion *pAppVersion, TSteamError *pError );
	int EnumerateAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError );
	SteamCallHandle_t StartLoadingCache( unsigned int uAppId, TSteamError *pError );
	int InsertAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError );
	int RemoveAppDependency( unsigned int uAppId, unsigned int uIndex, TSteamError *pError );
	int FindApp( const char *cszAppName, unsigned int *puAppId, TSteamError *pError );
	int GetAppDependencies( unsigned int uAppId, unsigned int *puCacheIds, unsigned int uMaxIds, TSteamError *pError );
	int IsSubscribed( unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbIsSubscriptionPending, TSteamError *pError );
	int GetAppUserDefinedInfo( unsigned int uAppId, const char *cszKey, char *szValueBuf, unsigned int uValueBufLen, unsigned int *puValueLen, TSteamError *pError );
	SteamCallHandle_t WaitForAppReadyToLaunch( unsigned int uAppId, TSteamError *pError );
	int IsCacheLoadingEnabled( unsigned int uAppId, int *pbIsLoading, TSteamError *pError );
	SteamCallHandle_t StopLoadingCache( unsigned int uAppId, TSteamError *pError );
	ESteamError GetEncryptedUserIDTicket( const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError );
	SteamCallHandle_t FlushCache( unsigned int uCacheId, TSteamError *pError );
	SteamCallHandle_t RepairOrDecryptCaches( unsigned int uAppId, int bForceValidation, TSteamError *pError );
	SteamCallHandle_t LoadCacheFromDir( unsigned int uAppId, const char *szPath, TSteamError *pError );
	int GetCacheDefaultDirectory( char *szPath, TSteamError *pError );
	int SetCacheDefaultDirectory( const char *szPath, TSteamError *pError );
	int GetAppDir( unsigned int uAppId, char *szPath, TSteamError *pError );
	SteamCallHandle_t MoveApp( unsigned int uAppId, const char *szPath, TSteamError *pError );
	SteamCallHandle_t GetAppCacheSize( unsigned int uCacheId, unsigned int *pCacheSizeInMb, TSteamError *pError );
	SteamCallHandle_t SetAppCacheSize( unsigned int uCacheId, unsigned int nCacheSizeInMb, TSteamError *pError );
	SteamCallHandle_t SetAppVersion( unsigned int uAppId, unsigned int uAppVersionId, TSteamError *pError );
	SteamCallHandle_t Uninstall( TSteamError *pError );
	int SetNotificationCallback( SteamNotificationCallback_t pCallbackFunction, TSteamError *pError );
	SteamCallHandle_t ChangeForgottenPassword( const char *cszUser, const char *cszAnswerToQuestion, const char *cszEmailVerificationKey, const char *cszNewPassphrase, int *pbChanged, TSteamError *pError );
	SteamCallHandle_t RequestForgottenPasswordEmail( const char *cszUser, SteamPersonalQuestion_t ReceivePersonalQuestion, TSteamError *pError );
	SteamCallHandle_t RequestAccountsByEmailAddressEmail( const char *cszEmailAddress, TSteamError *pError );
	SteamCallHandle_t RequestAccountsByCdKeyEmail( const char *cszCdKey, TSteamError *pError );
	SteamCallHandle_t GetNumAccountsWithEmailAddress( const char *cszEmailAddress, unsigned int *puNumAccounts, TSteamError *pError );
	SteamCallHandle_t UpdateAccountBillingInfo( const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError );
	SteamCallHandle_t UpdateSubscriptionBillingInfo( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError );
	int GetSponsorUrl( unsigned int uAppId, char *szUrl, unsigned int uBufSize, unsigned int *pUrlChars, TSteamError *pError );
	int GetContentServerInfo( unsigned int uAppId, unsigned int *puServerId, unsigned int *puServerIpAddress, TSteamError *pError );
	SteamCallHandle_t GetAppUpdateStats( unsigned int uAppOrCacheId, ESteamAppUpdateStatsQueryType eQueryType, TSteamUpdateStats *pUpdateStats, TSteamError *pError );
	int GetTotalUpdateStats( TSteamUpdateStats *pUpdateStats, TSteamError *pError );
	SteamCallHandle_t CreateCachePreloaders( TSteamError *pError );
//SteamInterface004 functions
	void Win32SetMiniDumpComment( const char *comment );
	void Win32SetMiniDumpSourceControlId( unsigned int SourcecontrolID );
	void Win32SetMiniDumpEnableFullMemory();
	void Win32WriteMiniDump( const char *szErrorOrAssertType, const char *szDescriptionOrAssertName, const char *szAssertExpr, const char *szAssertFilename, unsigned int uAssertLineNumber );
	int GetCurrentAppId( unsigned int *puAppId, TSteamError *pError );
	int GetAppPurchaseCountry( unsigned int uAppId, char *szCountry, unsigned int uBufSize, int *pPurchaseTime, TSteamError *pError );
//SteamInterface005 functions
	int GetLocalClientVersion( unsigned int *puBootstrapperVersion, unsigned int *puClientVersion, TSteamError *pError );
	int IsFileNeededByCache( unsigned int uCacheId, const char *pchFileName, unsigned int uFileSize, TSteamError *pError );
	int LoadFileToCache( unsigned int uCacheId, const char *pchFileName, const void *pubDataChunk, unsigned int cubDataChunk, unsigned int cubDataOffset, TSteamError *pError );
	int GetCacheDecryptionKey( unsigned int uCacheId, char *pchKeyBuffer, unsigned int cubBuff, unsigned int *pcubKey, TSteamError *pError );
	int GetSubscriptionExtendedInfo( unsigned int uSubscriptionId, const char *cszKey, char *szValueBuf, unsigned int uValueBufLen, unsigned *puValueLen, TSteamError *pError );
	int GetSubscriptionPurchaseCountry( unsigned int uSubscriptionId, char *szCountry, unsigned int uBufSize, int *pPurchaseTime, TSteamError *pError );
	int GetAppUserDefinedRecord( unsigned int uAppId, KeyValueIteratorCallback_t AddEntryToKeyValueFunc, void *pvCKeyValue, TSteamError *pError );
//SteamInterface006 functions
	int FindServersNumServers( ESteamServerType eServerType );
	int FindServersIterateServer( ESteamServerType eServerType, unsigned int nServer, char *szIpAddrPort, int szIpAddrPortLen );
	const char *FindServersGetErrorString();
	int CheckAppOwnership( unsigned int uAppId, int *pbOwned, TSteamGlobalUserID *pSteamID, TSteamError *pError );
};
