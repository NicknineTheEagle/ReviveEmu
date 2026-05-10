#include "stdafx.h"
#include "SteamDLLAppsystemImpl.h"
#include "Steam.h"

CSteamDLLAppsystem::CSteamDLLAppsystem() {}
CSteamDLLAppsystem::~CSteamDLLAppsystem() {}

bool CSteamDLLAppsystem::Connect(CreateInterfaceFn factory)
{
	return true;
}
void CSteamDLLAppsystem::Disconnect()
{
	return;
}
void* CSteamDLLAppsystem::QueryInterface(const char* pInterfaceName)
{
	return _f(pInterfaceName);
}
InitReturnVal_t CSteamDLLAppsystem::Init()
{
	return INIT_OK;
}
void CSteamDLLAppsystem::Shutdown()
{
	return;
}
