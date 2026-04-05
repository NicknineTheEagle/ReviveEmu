#include "stdafx.h"
#include "Steam.h"
#include "SteamInterface003.h"

extern CLogFile* Logger;
extern bool bLogging;

CSteamInterface003::CSteamInterface003(){if (bLogging) Logger->Write("CSteamInterface003 constructed.\n");}
CSteamInterface003::~CSteamInterface003(){if (bLogging) Logger->Write("CSteamInterface003 destructed.\n");}

int CSteamInterface003::Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError)
{
	return SteamStat(cszName, pInfo, pError);
}

SteamCallHandle_t CSteamInterface003::OpenFile(const char *cszFileName, const char *cszMode, TSteamError *pError)
{
	return SteamOpenFile(cszFileName, cszMode, pError);
}

int CSteamInterface003::CloseFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamCloseFile(hFile, pError);
}

unsigned int CSteamInterface003::ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamReadFile(pBuf, uSize, uCount, hFile, pError);
}

unsigned int CSteamInterface003::WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamWriteFile(pBuf, uSize, uCount, hFile, pError);
}

SteamCallHandle_t CSteamInterface003::FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError);
}

int CSteamInterface003::FindNext(SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindNext(hDirectory, pFindInfo, pError);
}

int CSteamInterface003::FindClose(SteamHandle_t hDirectory, TSteamError *pError)
{
	return SteamFindClose(hDirectory, pError);
}

int CSteamInterface003::GetLocalFileCopy(const char* cszFileName, TSteamError* pError)
{
	return SteamGetLocalFileCopy(cszFileName, pError);
}

int CSteamInterface003::Startup(unsigned int uUsingMask, TSteamError *pError)
{
	return SteamStartup(uUsingMask, pError);
}

int CSteamInterface003::Cleanup(TSteamError *pError)
{
	return SteamCleanup(pError);
}

int CSteamInterface003::IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError)
{
	return SteamIsLoggedIn(pbIsLoggedIn, pError);
}

int CSteamInterface003::IsSecureComputer(int *pbIsSecure, TSteamError *pError)
{
	return SteamIsSecureComputer(pbIsSecure, pError);
}

int CSteamInterface003::IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pbUnknown, pError);
}
