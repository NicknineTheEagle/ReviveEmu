#pragma once

extern CLogFile* Logger;
extern bool bLogging;
extern bool bLogUserId;
extern bool g_bSteamDll;
extern bool g_bAllowNonRev;
extern char g_szOrigSteamDll[MAX_PATH];

enum ERevClientType
{
	eClientRev = 0,
	eClientLegit,
	eClientLegitWrapper,
	eClientUnknown
};

struct TRevUserValidationHandle
{
	ERevClientType eClientType;
	TSteamGlobalUserID Steam2ID;
	CSteamID Steam3ID;
	unsigned int uClientIP;
	unsigned int uClientLocalIP;
	SteamUserIDTicketValidationHandle_t LegitHandle;
	ESteamError eReturnCode;
};

std::vector<TRevUserValidationHandle *> g_RevUserValidations;

typedef ESteamError (STEAM_CALL *SteamInitializeUserIDTicketValidatorCall)(const char*, const char*, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
typedef ESteamError (STEAM_CALL *SteamStartValidatingUserIDTicketCall)(void*, unsigned int, unsigned int, SteamUserIDTicketValidationHandle_t*);
typedef ESteamError (STEAM_CALL *SteamProcessOngoingUserIDTicketValidationCall)(SteamUserIDTicketValidationHandle_t Handle,
													  TSteamGlobalUserID*,
													  unsigned int*,
													  unsigned char*,
													  size_t,
													  size_t*);

const char* GetUserIDString(const TSteamGlobalUserID& steamid)
{
	static char idstr[128];
	_snprintf(idstr, sizeof(idstr) - 1, "STEAM_%u:%u:%u", (SteamInstanceID_t)steamid.m_SteamInstanceID,
	          (unsigned int)((SteamLocalUserID_t)steamid.m_SteamLocalUserID.Split.High32bits),
	          (unsigned int)((SteamLocalUserID_t)steamid.m_SteamLocalUserID.Split.Low32bits));
	idstr[sizeof(idstr) - 1] = '\0';

	return idstr;
}

STEAM_API ESteamError STEAM_CALL SteamGetEncryptedUserIDTicket(const void *pEncryptionKeyReceivedFromAppServer, unsigned int uEncryptionKeyLength, void *pOutputBuffer, unsigned int uSizeOfOutputBuffer, unsigned int *pReceiveSizeOfEncryptedTicket, TSteamError *pError)
{
	if (bLogging && bLogUserId)
	{
		Logger->Write("SteamGetEncryptedUserIDTicket:\n");
		Logger->Write("\tKey Address: 0x%p\n", pEncryptionKeyReceivedFromAppServer);
		Logger->Write("\tKey Length: %u\n", uEncryptionKeyLength);
		Logger->Write("\tOutput Address: 0x%p\n", pOutputBuffer);
		Logger->Write("\tOutput Size: %u\n", uSizeOfOutputBuffer);
		Logger->Write("\tTicket Size Address: 0x%p\n", pReceiveSizeOfEncryptedTicket);
		Logger->Write("\tError Address: 0x%p\n", pError);
	}
	if (bLogging && bLogUserId) Logger->Write("\t---\n");
	if (bLogging && bLogUserId) Logger->Write("\tSteamID: %llu\n", g_SteamID.ConvertToUint64());

	struct TRevTicket sGetRevTicket;
	sGetRevTicket.uSignature = REVTICKET_SIGNATURE;
	sGetRevTicket.uVersion = REVTICKET_VERSION;
	sGetRevTicket.ulSteamID = g_SteamID.ConvertToUint64();
	sGetRevTicket.uLocalIP = 0; // TODO: Maybe set this?
	memcpy(pOutputBuffer, &sGetRevTicket, sizeof(TRevTicket));
	*pReceiveSizeOfEncryptedTicket = sizeof(TRevTicket);

	SteamClearError(pError);
	return eSteamErrorNone;
}


STEAM_API ESteamError STEAM_CALL SteamInitializeUserIDTicketValidator(const char * pszOptionalPublicEncryptionKeyFilename, const char *	pszOptionalPrivateDecryptionKeyFilename, unsigned int ClientClockSkewToleranceInSeconds, unsigned int ServerClockSkewToleranceInSeconds, unsigned int MaxNumLoginsWithinClientClockSkewTolerancePerClient, unsigned int	HintPeakSimultaneousValidations, unsigned int AbortValidationAfterStallingForNProcessSteps)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamInitializeUserIDTicketValidator\n");
// #endif

	if (g_bSteamDll)
	{
		ESteamError retval = eSteamErrorNone;
		SteamInitializeUserIDTicketValidatorCall CallSteamInitializeUserIDTicketValidator;
		CallSteamInitializeUserIDTicketValidator = (SteamInitializeUserIDTicketValidatorCall) GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamInitializeUserIDTicketValidator");
		retval = CallSteamInitializeUserIDTicketValidator(pszOptionalPublicEncryptionKeyFilename, pszOptionalPrivateDecryptionKeyFilename, ClientClockSkewToleranceInSeconds, ServerClockSkewToleranceInSeconds, MaxNumLoginsWithinClientClockSkewTolerancePerClient, HintPeakSimultaneousValidations, AbortValidationAfterStallingForNProcessSteps);
		if (bLogging && bLogUserId) Logger->Write("\t %u\n", (int)retval);
		return retval;
	}

	return eSteamErrorNone;
}

STEAM_API ESteamError STEAM_CALL SteamShutdownUserIDTicketValidator()
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamShutdownUserIDTicketValidator\n");
// #endif

	for (TRevUserValidationHandle* hRevHandle : g_RevUserValidations)
	{
		delete hRevHandle;
	}
	g_RevUserValidations.clear();

	if (g_bSteamDll)
	{
		ESteamError retval = eSteamErrorNone;
		ESteamError (*fptr)();
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamShutdownUserIDTicketValidator");
		retval = (*fptr)();
		if (bLogging && bLogUserId) Logger->Write("\t %u\n", (int)retval);
		return retval;
	}

	return eSteamErrorNone;
}

STEAM_API const unsigned char* STEAM_CALL SteamGetEncryptionKeyToSendToNewClient(unsigned int * pReceiveSizeOfEncryptionKey)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamGetEncryptionKeyToSendToNewClient (0x%p)\n", pReceiveSizeOfEncryptionKey);
// #endif

	if (g_bSteamDll)
	{
		const unsigned char* retval;
		const unsigned char* (*fptr)(unsigned int*);
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamGetEncryptionKeyToSendToNewClient");
		retval = (*fptr)(pReceiveSizeOfEncryptionKey);
		if (bLogging && bLogUserId) Logger->Write("\t 0x%p\n", retval);
		return retval;
	}

	*pReceiveSizeOfEncryptionKey = sizeof(g_TicketKey);
	return g_TicketKey;
}

STEAM_API ESteamError STEAM_CALL SteamStartValidatingUserIDTicket(void *pEncryptedUserIDTicketFromClient, unsigned int uSizeOfEncryptedUserIDTicketFromClient, unsigned int ObservedClientIPAddr, SteamUserIDTicketValidationHandle_t *pReceiveHandle)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamStartValidatingUserIDTicket (0x%p, %u, %u, 0x%p)\n", pEncryptedUserIDTicketFromClient, uSizeOfEncryptedUserIDTicketFromClient, ObservedClientIPAddr, pReceiveHandle);
	// #endif

	if (g_bSteamDll)
	{
		SteamStartValidatingUserIDTicketCall StartValidating;
		ESteamError retval = eSteamErrorNone;
		StartValidating = (SteamStartValidatingUserIDTicketCall) GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamStartValidatingUserIDTicket");
		retval = StartValidating(pEncryptedUserIDTicketFromClient, uSizeOfEncryptedUserIDTicketFromClient, ObservedClientIPAddr, pReceiveHandle);
		if (bLogging && bLogUserId) Logger->Write("\t %u\n", (int)retval);

		if (retval == eSteamErrorNone || retval == eSteamErrorNotFinishedProcessing)
		{
			if (bLogging && bLogUserId) Logger->Write("\t Client using legitimate Steam account.\n");

			TRevUserValidationHandle* hRevHandle = new TRevUserValidationHandle();
			memset(hRevHandle, 0, sizeof(TRevUserValidationHandle));
			hRevHandle->eClientType = eClientLegit;
			hRevHandle->LegitHandle = *pReceiveHandle;
			g_RevUserValidations.push_back(hRevHandle);

			*pReceiveHandle = hRevHandle;
			if (bLogging && bLogUserId) Logger->Write("\t Returned handle 0x%p\n", hRevHandle);
			return retval;
		}
	}

	if (uSizeOfEncryptedUserIDTicketFromClient < 8)
	{
		*pReceiveHandle = STEAM_INACTIVE_USERIDTICKET_VALIDATION_HANDLE;
		return eSteamErrorUnknown;
	}

	TRevUserValidationHandle* hRevHandle = new TRevUserValidationHandle();
	memset(hRevHandle, 0, sizeof(TRevUserValidationHandle));
	hRevHandle->uClientIP = ObservedClientIPAddr;

	uint32 uCheckTicket = *(uint32*)pEncryptedUserIDTicketFromClient;
	if (uCheckTicket == REVTICKET_SIGNATURE)
	{
		// This is our auth ticket format.
		if (bLogging && bLogUserId) Logger->Write("\t Client using REVive emulator.\n");

		hRevHandle->eClientType = eClientRev;
		const TRevTicket* pRevTicket = (TRevTicket*)pEncryptedUserIDTicketFromClient;

		if (pRevTicket->uVersion == REVTICKET_VERSION)
		{
			if (uSizeOfEncryptedUserIDTicketFromClient == sizeof(TRevTicket))
			{
				hRevHandle->Steam3ID = pRevTicket->ulSteamID;
				hRevHandle->uClientLocalIP = pRevTicket->uLocalIP;
				hRevHandle->eReturnCode = eSteamErrorNone;
			}
			else
			{
				if (bLogging && bLogUserId) Logger->Write("\t Malformed REVive auth ticket.\n");
				hRevHandle->eReturnCode = eSteamErrorCorruptEncryptedUserIDTicket;
			}
		}
		else
		{
			if (bLogging && bLogUserId) Logger->Write("\t REVive auth ticket version %u not supported.\n", pRevTicket->uVersion);
			hRevHandle->eReturnCode = eSteamErrorInvalidUserIDTicket;
		}
	}
	else if (uCheckTicket == STEAMTICKET_SIGNATURE)
	{
		// This is auth ticket used by Valve's official Steam2 wrapper dll.
		if (bLogging && bLogUserId) Logger->Write("\t Client using legitimate Steam account.\n");

		hRevHandle->eClientType = eClientLegitWrapper;
		static_assert(sizeof(TSteam2WrapperTicket) == 32, "Size of steam2wrapper ticket is not 32 bytes");
		const TSteam2WrapperTicket* pTicket = (TSteam2WrapperTicket*)pEncryptedUserIDTicketFromClient;

		if (uSizeOfEncryptedUserIDTicketFromClient == sizeof(TSteam2WrapperTicket))
		{
			hRevHandle->Steam2ID = pTicket->SteamID;
			hRevHandle->uClientLocalIP = pTicket->uLocalIP;
			hRevHandle->eReturnCode = eSteamErrorNone;
		}
		else
		{
			if (bLogging && bLogUserId) Logger->Write("\t Malformed Steam auth ticket.\n");
			hRevHandle->eReturnCode = eSteamErrorCorruptEncryptedUserIDTicket;
		}
	}
	else
	{
		// Unknown client, generate SteamID from IP address.
		if (bLogging && bLogUserId) Logger->Write("\t Client not using REVive emulator.\n");

		hRevHandle->eClientType = eClientUnknown;
		hRevHandle->Steam3ID.Set(ObservedClientIPAddr, k_EUniversePublic, k_EAccountTypeIndividual);
		hRevHandle->uClientLocalIP = 0;
		hRevHandle->eReturnCode = g_bAllowNonRev ? eSteamErrorNone : eSteamErrorInvalidUserIDTicket;
	}

	g_RevUserValidations.push_back(hRevHandle);

	*pReceiveHandle = hRevHandle;
	if (bLogging && bLogUserId) Logger->Write("\t Returned handle 0x%p\n", hRevHandle);

	return eSteamErrorNotFinishedProcessing;
}

STEAM_API ESteamError STEAM_CALL SteamStartValidatingNewValveCDKey(void *pEncryptedNewValveCDKeyFromClient, unsigned int uSizeOfEncryptedNewValveCDKeyFromClient, unsigned int ObservedClientIPAddr, struct sockaddr *pPrimaryValidateNewCDKeyServerSockAddr, struct sockaddr *pSecondaryValidateNewCDKeyServerSockAddr, SteamUserIDTicketValidationHandle_t *pReceiveHandle)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamStartValidatingNewValveCDKey\n");
// #endif

	if (g_bSteamDll)
	{
		ESteamError retval = eSteamErrorNone;
		ESteamError (*fptr)(void*, unsigned int, unsigned int, struct sockaddr*, struct sockaddr*, SteamUserIDTicketValidationHandle_t*);
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamStartValidatingNewValveCDKey");
		retval = (*fptr)(pEncryptedNewValveCDKeyFromClient, uSizeOfEncryptedNewValveCDKeyFromClient, ObservedClientIPAddr, pPrimaryValidateNewCDKeyServerSockAddr, pSecondaryValidateNewCDKeyServerSockAddr, pReceiveHandle);
		if (bLogging && bLogUserId) Logger->Write("\t 0x%08X\n", (int)retval);

		if (retval == eSteamErrorNone || retval == eSteamErrorNotFinishedProcessing)
		{
			if (bLogging && bLogUserId) Logger->Write("\t Client using legitimate STEAM account.\n");

			TRevUserValidationHandle* hRevHandle = new TRevUserValidationHandle();
			memset(hRevHandle, 0, sizeof(TRevUserValidationHandle));
			hRevHandle->eClientType = eClientLegit;
			hRevHandle->LegitHandle = *pReceiveHandle;
			g_RevUserValidations.push_back(hRevHandle);

			*pReceiveHandle = hRevHandle;
			return retval;
		}
	}

	// TODO
	*pReceiveHandle = STEAM_INACTIVE_USERIDTICKET_VALIDATION_HANDLE;
	return eSteamErrorUnknown;
}

STEAM_API ESteamError STEAM_CALL SteamProcessOngoingUserIDTicketValidation(SteamUserIDTicketValidationHandle_t Handle, TSteamGlobalUserID *pReceiveValidSteamGlobalUserID, unsigned int *pReceiveClientLocalIPAddr, unsigned char *pOptionalReceiveProofOfAuthenticationToken, size_t SizeOfOptionalAreaToReceiveProofOfAuthenticationToken, size_t *pOptionalReceiveSizeOfProofOfAuthenticationToken)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamProcessOngoingUserIDTicketValidation (0x%p)\n", Handle);
// #endif

	if (Handle == STEAM_INACTIVE_USERIDTICKET_VALIDATION_HANDLE)
		return eSteamErrorNothingToDo;

	TRevUserValidationHandle* hRevHandle = (TRevUserValidationHandle*)Handle;

	if (g_bSteamDll && hRevHandle->eClientType == eClientLegit)
	{
		ESteamError retval = eSteamErrorNone;
		ESteamError (*fptr)(SteamUserIDTicketValidationHandle_t, TSteamGlobalUserID*, unsigned int*, unsigned char*, size_t, size_t*);
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamProcessOngoingUserIDTicketValidation");
		retval = (*fptr)(hRevHandle->LegitHandle, pReceiveValidSteamGlobalUserID, pReceiveClientLocalIPAddr, pOptionalReceiveProofOfAuthenticationToken, SizeOfOptionalAreaToReceiveProofOfAuthenticationToken, pOptionalReceiveSizeOfProofOfAuthenticationToken);
		if (bLogging && bLogUserId) Logger->Write("\t 0x%08X\n", (int)retval);
		return retval;
	}

	if (hRevHandle->eReturnCode != eSteamErrorNone)
		return hRevHandle->eReturnCode;

	if (hRevHandle->eClientType == eClientRev)
	{
		hRevHandle->Steam3ID.ConvertToSteam2(pReceiveValidSteamGlobalUserID);

		if (bLogging && bLogUserId) Logger->Write("\t Received Steam ID: %s\n",
			GetUserIDString(*pReceiveValidSteamGlobalUserID));
	}
	else if (hRevHandle->eClientType == eClientLegitWrapper)
	{
		*pReceiveValidSteamGlobalUserID = hRevHandle->Steam2ID;

		if (bLogging && bLogUserId) Logger->Write("\t Received Steam ID: %s\n",
			GetUserIDString(*pReceiveValidSteamGlobalUserID));
	}
	else
	{
		hRevHandle->Steam3ID.ConvertToSteam2(pReceiveValidSteamGlobalUserID);

		if (bLogging && bLogUserId) Logger->Write("\t Received IP: %u -> %s\n",
			hRevHandle->uClientIP,
			GetUserIDString(*pReceiveValidSteamGlobalUserID));
	}

	if (pReceiveClientLocalIPAddr)
		*pReceiveClientLocalIPAddr = hRevHandle->uClientLocalIP;

	if (pOptionalReceiveSizeOfProofOfAuthenticationToken)
		*pOptionalReceiveSizeOfProofOfAuthenticationToken = 0;

	return eSteamErrorNone;
}

STEAM_API void STEAM_CALL SteamAbortOngoingUserIDTicketValidation(SteamUserIDTicketValidationHandle_t Handle)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamAbortOngoingUserIDTicketValidation (0x%p)\n", Handle);
// #endif

	if (Handle == STEAM_INACTIVE_USERIDTICKET_VALIDATION_HANDLE)
		return;

	TRevUserValidationHandle* hRevHandle = (TRevUserValidationHandle*)Handle;

	if (g_bSteamDll && hRevHandle->eClientType == eClientLegit)
	{
		void (*fptr)(SteamUserIDTicketValidationHandle_t);
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamAbortOngoingUserIDTicketValidation");
		(*fptr)(hRevHandle->LegitHandle);
		if (bLogging && bLogUserId) Logger->Write("\t returned.\n");
	}

	auto it = std::find(g_RevUserValidations.begin(), g_RevUserValidations.end(), hRevHandle);
	if (it != g_RevUserValidations.end()) {
		delete hRevHandle;
		g_RevUserValidations.erase(it);
	}
}

STEAM_API ESteamError STEAM_CALL SteamOptionalCleanUpAfterClientHasDisconnected(unsigned int ObservedClientIPAddr, unsigned int ClientLocalIPAddr)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamOptionalCleanUpAfterClientHasDisconnected\n");
// #endif
	if (g_bSteamDll)
	{
		ESteamError retval = eSteamErrorNone;
		ESteamError (*fptr)(unsigned int, unsigned int);
		*(void **)(&fptr) = GetProcAddress(GetModuleHandleA(g_szOrigSteamDll), "SteamOptionalCleanUpAfterClientHasDisconnected");
		retval = (*fptr)(ObservedClientIPAddr, ClientLocalIPAddr);
		if (bLogging && bLogUserId) Logger->Write("\t %u\n", (int)retval);
		return retval;
	}
	return eSteamErrorNone;
}
