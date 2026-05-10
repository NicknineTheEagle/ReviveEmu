#pragma once

#include "IAppSystem.h"

class CSteamDLLAppsystem : public IAppSystem
{
public:
	CSteamDLLAppsystem();
	~CSteamDLLAppsystem();

	bool Connect(CreateInterfaceFn factory);
	void Disconnect();
	void* QueryInterface(const char* pInterfaceName);
	InitReturnVal_t Init();
	void Shutdown();
};
