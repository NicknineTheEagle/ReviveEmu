#include "stdafx.h"
#include "Steam.h"
#include "SteamInterface004.h"

extern CLogFile* Logger;
extern bool bLogging;

CSteamInterface004::CSteamInterface004(){if (bLogging) Logger->Write("CSteamInterface004 constructed.\n");}
CSteamInterface004::~CSteamInterface004(){if (bLogging) Logger->Write("CSteamInterface004 destructed.\n");}

int CSteamInterface004::GetCurrentEmailAddress(char *szEmailaddress, unsigned int uBufSize, unsigned int *puEmailaddressChars, TSteamError *pError)
{
	return SteamGetCurrentEmailAddress(szEmailaddress, uBufSize, puEmailaddressChars, pError);
}

int CSteamInterface004::MountAppFilesystem(TSteamError *pError)
{
	return SteamMountAppFilesystem(pError);
}

int CSteamInterface004::UnmountAppFilesystem(TSteamError* pError)
{
	return SteamUnmountAppFilesystem(pError);
}

int CSteamInterface004::MountFilesystem(unsigned int uAppId, const char *szMountPath, TSteamError *pError)
{
	return SteamMountFilesystem(uAppId, szMountPath, pError);
}

int CSteamInterface004::Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError)
{
	return SteamStat(cszName, pInfo, pError);
}

SteamCallHandle_t CSteamInterface004::OpenFile(const char *cszFileName, const char *cszMode, TSteamError *pError)
{
	return SteamOpenFile(cszFileName, cszMode, pError);
}

SteamCallHandle_t CSteamInterface004::OpenFileEx(const char *cszFileName, const char *cszMode, unsigned int *size, TSteamError *pError)
{
	return SteamOpenFileEx(cszFileName, cszMode, size, pError);
}

int CSteamInterface004::ProcessCall(SteamCallHandle_t handle, TSteamProgress *pProgress, TSteamError *pError)
{
	return SteamProcessCall(handle, pProgress, pError);
}

int CSteamInterface004::CloseFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamCloseFile(hFile, pError);
}

unsigned int CSteamInterface004::ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamReadFile(pBuf, uSize, uCount, hFile, pError);
}

unsigned int CSteamInterface004::WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamWriteFile(pBuf, uSize, uCount, hFile, pError);
}

int CSteamInterface004::SeekFile(SteamHandle_t hFile, long lOffset, ESteamSeekMethod sm, TSteamError *pError)
{
	return SteamSeekFile(hFile, lOffset, sm, pError);
}

long CSteamInterface004::TellFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamTellFile(hFile, pError);
}

SteamCallHandle_t CSteamInterface004::FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError);
}

int CSteamInterface004::FindNext(SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindNext(hDirectory, pFindInfo, pError);
}

int CSteamInterface004::FindClose(SteamHandle_t hDirectory, TSteamError *pError)
{
	return SteamFindClose(hDirectory, pError);
}

int CSteamInterface004::GetLocalFileCopy(const char* cszFileName, TSteamError* pError)
{
	return SteamGetLocalFileCopy(cszFileName, pError);
}

SteamCallHandle_t CSteamInterface004::WaitForResources(const char *cszMasterList, TSteamError* pError)
{
	return SteamWaitForResources(cszMasterList, pError);
}

int CSteamInterface004::Startup(unsigned int uUsingMask, TSteamError *pError)
{
	return SteamStartup(uUsingMask, pError);
}

int CSteamInterface004::Cleanup(TSteamError *pError)
{
	return SteamCleanup(pError);
}

int CSteamInterface004::IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError)
{
	return SteamIsLoggedIn(pbIsLoggedIn, pError);
}

int CSteamInterface004::GetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError)
{
	return SteamGetUser(szUser, uBufSize, puUserChars, pSteamGlobalUserID, pError);
}

int CSteamInterface004::IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pbUnknown, pError);
}

int CSteamInterface004::EnumerateApp(unsigned int AppId, TSteamApp *pApp, TSteamError *pError)
{
	return SteamEnumerateApp(AppId, pApp, pError);
}

int CSteamInterface004::EnumerateAppDependency(unsigned int AppId, unsigned int uDependency, TSteamAppDependencyInfo *pDependencyInfo, TSteamError *pError)
{
	return SteamEnumerateAppDependency(AppId, uDependency, pDependencyInfo, pError);
}

int CSteamInterface004::IsSubscribed(unsigned int uSubscriptionId, int *pbIsSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsSubscribed(uSubscriptionId, pbIsSubscribed, pbUnknown, pError);
}

ESteamError CSteamInterface004::GetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError)
{
	return SteamGetEncryptedUserIDTicket(pEncryptionKeyReceivedFromAppServer, uEncryptionKeyLength, pOutputBuffer, uSizeOfOutputBuffer, pReceiveSizeOfEncryptedTicket, pError);
}