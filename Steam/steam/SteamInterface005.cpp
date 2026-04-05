#include "stdafx.h"
#include "Steam.h"
#include "SteamInterface005.h"

extern CLogFile* Logger;
extern bool bLogging;

CSteamInterface005::CSteamInterface005(){if (bLogging) Logger->Write("CSteamInterface005 constructed.\n");}
CSteamInterface005::~CSteamInterface005(){if (bLogging) Logger->Write("CSteamInterface005 destructed.\n");}

int CSteamInterface005::Stat(const char *cszName, TSteamElemInfo *pInfo, TSteamError *pError)
{
	return SteamStat(cszName, pInfo, pError);
}

SteamCallHandle_t CSteamInterface005::OpenFile(const char *cszFileName, const char *cszMode, TSteamError *pError)
{
	return SteamOpenFile(cszFileName, cszMode, pError);
}

int CSteamInterface005::CloseFile(SteamHandle_t hFile, TSteamError *pError)
{
	return SteamCloseFile(hFile, pError);
}

unsigned int CSteamInterface005::ReadFile(void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamReadFile(pBuf, uSize, uCount, hFile, pError);
}

unsigned int CSteamInterface005::WriteFile(const void *pBuf, unsigned int uSize, unsigned int uCount, SteamHandle_t hFile, TSteamError *pError)
{
	return SteamWriteFile(pBuf, uSize, uCount, hFile, pError);
}

SteamCallHandle_t CSteamInterface005::FindFirst(const char *cszPattern, ESteamFindFilter eFilter, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindFirst(cszPattern, eFilter, pFindInfo, pError);
}

int CSteamInterface005::FindNext(SteamHandle_t hDirectory, TSteamElemInfo *pFindInfo, TSteamError *pError)
{
	return SteamFindNext(hDirectory, pFindInfo, pError);
}

int CSteamInterface005::FindClose(SteamHandle_t hDirectory, TSteamError *pError)
{
	return SteamFindClose(hDirectory, pError);
}

int CSteamInterface005::GetLocalFileCopy(const char* cszFileName, TSteamError* pError)
{
	return SteamGetLocalFileCopy(cszFileName, pError);
}

int CSteamInterface005::Startup(unsigned int uUsingMask, TSteamError *pError)
{
	return SteamStartup(uUsingMask, pError);
}

int CSteamInterface005::Cleanup(TSteamError *pError)
{
	return SteamCleanup(pError);
}

int CSteamInterface005::IsLoggedIn(int *pbIsLoggedIn, TSteamError *pError)
{
	return SteamIsLoggedIn(pbIsLoggedIn, pError);
}

int CSteamInterface005::IsSecureComputer(int *pbIsSecure, TSteamError *pError)
{
	return SteamIsSecureComputer(pbIsSecure, pError);
}

int CSteamInterface005::IsAppSubscribed(unsigned int uAppId, int *pbIsAppSubscribed, int *pbUnknown, TSteamError *pError)
{
	return SteamIsAppSubscribed(uAppId, pbIsAppSubscribed, pbUnknown, pError);
}

int CSteamInterface005::GetLocalClientVersion(unsigned int* puArg1, unsigned int* puArg2, TSteamError *pError)
{
	return SteamGetLocalClientVersion(puArg1, puArg2, pError);
}
