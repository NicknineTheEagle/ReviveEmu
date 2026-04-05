#include "stdafx.h"
#include "Steam.h"
#include "SteamInterface001.h"

extern CLogFile* Logger;
extern bool bLogging;

CSteamInterface001::CSteamInterface001(){if (bLogging) Logger->Write("CSteamInterface001 constructed.\n");}
CSteamInterface001::~CSteamInterface001(){if (bLogging) Logger->Write("CSteamInterface001 destructed.\n");}

int CSteamInterface001::GetCurrentEmailAddress(char *szEmailaddress, unsigned int uBufSize, unsigned int *puEmailaddressChars, TSteamError *pError)
{
	return SteamGetCurrentEmailAddress(szEmailaddress, uBufSize, puEmailaddressChars, pError);
}

int CSteamInterface001::MountAppFilesystem(TSteamError *pError)
{
	return SteamMountAppFilesystem(pError);
}

int CSteamInterface001::UnmountAppFilesystem(TSteamError* pError)
{
	return SteamUnmountAppFilesystem(pError);
}

int CSteamInterface001::MountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError)
{
	return SteamMountFilesystem(uAppId, szMountPath, pError);
}

int CSteamInterface001::Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError)
{
	return SteamStat(cszName, pInfo, pError);
}

SteamCallHandle_t CSteamInterface001::OpenFile(const char *cszFileName, const char *cszMode, TSteamError *pError)
{
	return SteamOpenFile(cszFileName, cszMode, pError);
}

int CSteamInterface001::ProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError)
{
	return SteamProcessCall(handle, pProgress, pError);
}

int CSteamInterface001::CloseFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamCloseFile(hFile, pError);
}

unsigned int CSteamInterface001::ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamReadFile(pBuf, uSize, uCount, hFile, pError);
}

unsigned int CSteamInterface001::WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamWriteFile(pBuf, uSize, uCount, hFile, pError);
}

int CSteamInterface001::SeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod sm, TSteamError *pError)
{
	return SteamSeekFile(hFile, lOffset, sm, pError);
}

long CSteamInterface001::TellFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamTellFile(hFile, pError);
}

SteamCallHandle_t CSteamInterface001::FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError);
}

int CSteamInterface001::FindNext(SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindNext(hDirectory, pFindInfo, pError);
}

int CSteamInterface001::FindClose(SteamHandle_t hDirectory, TSteamError *pError)
{
	return SteamFindClose(hDirectory, pError);
}

int CSteamInterface001::GetLocalFileCopy(const char* cszFileName, TSteamError* pError)
{
	return SteamGetLocalFileCopy(cszFileName, pError);
}

SteamCallHandle_t CSteamInterface001::WaitForResources(const char *cszMasterList, TSteamError* pError)
{
	return SteamWaitForResources(cszMasterList, pError);
}

int CSteamInterface001::Startup(unsigned int uUsingMask, TSteamError *pError)
{
	return SteamStartup(uUsingMask, pError);
}

int CSteamInterface001::Cleanup(TSteamError *pError)
{
	return SteamCleanup(pError);
}

int CSteamInterface001::IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError)
{
	return SteamIsLoggedIn(pbIsLoggedIn, pError);
}

int CSteamInterface001::GetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError)
{
	return SteamGetUser(szUser, uBufSize, puUserChars, pSteamGlobalUserID, pError);
}

int CSteamInterface001::IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pbUnknown, pError);
}

int CSteamInterface001::EnumerateApp(unsigned int AppId, TSteamApp *pApp, TSteamError *pError)
{
	return SteamEnumerateApp(AppId, pApp, pError);
}

int CSteamInterface001::EnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError)
{
	return SteamEnumerateAppDependency(AppId, uDependency, pDependencyInfo, pError);
}

int CSteamInterface001::IsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsSubscribed(uSubscriptionId, pbIsSubscribed, pbUnknown, pError);
}

ESteamError CSteamInterface001::GetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError)
{
	return SteamGetEncryptedUserIDTicket(pEncryptionKeyReceivedFromAppServer, uEncryptionKeyLength, pOutputBuffer, uSizeOfOutputBuffer, pReceiveSizeOfEncryptedTicket, pError);
}