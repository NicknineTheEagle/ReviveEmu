#pragma once

extern CLogFile* Logger;
extern bool bLogging;
extern bool bLogUserId;
extern bool g_bSteamDll;
extern bool g_bAllowNonRev;

extern void RevInitialize(const char* cszInitSource);

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
	unsigned int uIP;
	unsigned int uTicketIP;
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
	V_sprintf_safe(idstr, "STEAM_%u:%u:%u", (SteamInstanceID_t)steamid.m_SteamInstanceID,
	          (unsigned int)((SteamLocalUserID_t)steamid.m_SteamLocalUserID.Split.High32bits),
	          (unsigned int)((SteamLocalUserID_t)steamid.m_SteamLocalUserID.Split.Low32bits));
	idstr[sizeof(idstr) - 1] = '\0';

	return idstr;
}

#ifndef VALIDATOR_DLL
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

	TSteamGlobalUserID Steam2ID;
	g_SteamID.ConvertToSteam2(&Steam2ID);
	if (bLogging && bLogUserId) Logger->Write("\t---\n");
	if (bLogging && bLogUserId) Logger->Write("\tSteamID: %s\n", GetUserIDString(Steam2ID));

	uint32 uSignature = REVTICKET_SIGNATURE;
	uint32 uVersion = REVTICKET_VERSION;
	uint64 ulSteamID = g_SteamID.ConvertToUint64();
	uint32 uIP = 0; // TODO: Maybe set this?

	char* pBuf = (char*)pOutputBuffer;
	memcpy(pBuf + 0x00, &uSignature, sizeof(uSignature));
	memcpy(pBuf + 0x04, &uVersion, sizeof(uVersion));
	memcpy(pBuf + 0x08, &ulSteamID, sizeof(ulSteamID));
	memcpy(pBuf + 0x10, &uIP, sizeof(uIP));
	*pReceiveSizeOfEncryptedTicket = REVTICKET_SIZE;

	SteamClearError(pError);
	return eSteamErrorNone;
}
#endif

STEAM_API ESteamError STEAM_CALL SteamInitializeUserIDTicketValidator(const char * pszOptionalPublicEncryptionKeyFilename, const char *	pszOptionalPrivateDecryptionKeyFilename, unsigned int ClientClockSkewToleranceInSeconds, unsigned int ServerClockSkewToleranceInSeconds, unsigned int MaxNumLoginsWithinClientClockSkewTolerancePerClient, unsigned int	HintPeakSimultaneousValidations, unsigned int AbortValidationAfterStallingForNProcessSteps)
{
// #ifdef DEBUG
	if (bLogging && bLogUserId) Logger->Write("SteamInitializeUserIDTicketValidator\n");
// #endif

	RevInitialize("SteamInitializeUserIDTicketValidator");

	if (g_bSteamDll)
	{
		ESteamError retval = eSteamErrorNone;
		SteamInitializeUserIDTicketValidatorCall CallSteamInitializeUserIDTicketValidator;
		CallSteamInitializeUserIDTicketValidator = (SteamInitializeUserIDTicketValidatorCall) GetProcAddress(g_hOrigSteamDll, "SteamInitializeUserIDTicketValidator");
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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamShutdownUserIDTicketValidator");
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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamGetEncryptionKeyToSendToNewClient");
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
		StartValidating = (SteamStartValidatingUserIDTicketCall) GetProcAddress(g_hOrigSteamDll, "SteamStartValidatingUserIDTicket");
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
	hRevHandle->uIP = ObservedClientIPAddr;

	uint32 uCheckTicket;
	memcpy(&uCheckTicket, pEncryptedUserIDTicketFromClient, sizeof(uCheckTicket));

	if (uCheckTicket == REVTICKET_SIGNATURE)
	{
		// This is our auth ticket format.
		if (bLogging && bLogUserId) Logger->Write("\t Client using REVive emulator.\n");

		hRevHandle->eClientType = eClientRev;

		char* pBuf = (char*)pEncryptedUserIDTicketFromClient;
		uint32 uVersion;
		memcpy(&uVersion, pBuf + 0x04, sizeof(uVersion));

		if (uVersion == REVTICKET_VERSION)
		{
			if (uSizeOfEncryptedUserIDTicketFromClient == REVTICKET_SIZE)
			{
				uint64 uSteamID;
				uint32 uIP;
				memcpy(&uSteamID, pBuf + 0x08, sizeof(uSteamID));
				memcpy(&uIP, pBuf + 0x10, sizeof(uIP));

				hRevHandle->Steam3ID = uSteamID;
				hRevHandle->uTicketIP = uIP;
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
			if (bLogging && bLogUserId) Logger->Write("\t REVive auth ticket version %u not supported.\n", uVersion);
			hRevHandle->eReturnCode = eSteamErrorInvalidUserIDTicket;
		}
	}
	else if (uCheckTicket == STEAMTICKET_SIGNATURE)
	{
		// This is auth ticket used by Valve's official Steam2 wrapper dll.
		if (bLogging && bLogUserId) Logger->Write("\t Client using legitimate Steam account.\n");

		hRevHandle->eClientType = eClientLegitWrapper;

		// Wrapper's ticket is stored as a C struct, and C structs are not at all guaranteed to be packed the same way
		// across different platforms. So here's our portable way of unpacking it.
		if (uSizeOfEncryptedUserIDTicketFromClient == STEAMTICKET_SIZE_WIN)
		{
			// Windows version.
			char* pBuf = (char*)pEncryptedUserIDTicketFromClient;
			uint16 uInstanceID;
			uint64 uAccountID;
			uint32 uIP;
			memcpy(&uInstanceID, pBuf + 0x08, sizeof(uInstanceID));
			memcpy(&uAccountID, pBuf + 0x10, sizeof(uAccountID));
			memcpy(&uIP, pBuf + 0x18, sizeof(uIP));

			hRevHandle->Steam2ID.m_SteamInstanceID = uInstanceID;
			hRevHandle->Steam2ID.m_SteamLocalUserID.As64bits = uAccountID;
			hRevHandle->uTicketIP = uIP;
			hRevHandle->eReturnCode = eSteamErrorNone;
		}
		else if (uSizeOfEncryptedUserIDTicketFromClient == STEAMTICKET_SIZE_LINUX)
		{
			// Linux version. There are no Linux clients to ever send this but just in case.
			char* pBuf = (char*)pEncryptedUserIDTicketFromClient;
			uint16 uInstanceID;
			uint64 uAccountID;
			uint32 uIP;
			memcpy(&uInstanceID, pBuf + 0x04, sizeof(uInstanceID));
			memcpy(&uAccountID, pBuf + 0x08, sizeof(uAccountID));
			memcpy(&uIP, pBuf + 0x10, sizeof(uIP));

			hRevHandle->Steam2ID.m_SteamInstanceID = uInstanceID;
			hRevHandle->Steam2ID.m_SteamLocalUserID.As64bits = uAccountID;
			hRevHandle->uTicketIP = uIP;
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
		hRevHandle->uTicketIP = 0;
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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamStartValidatingNewValveCDKey");
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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamProcessOngoingUserIDTicketValidation");
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
			hRevHandle->uIP,
			GetUserIDString(*pReceiveValidSteamGlobalUserID));
	}

	if (pReceiveClientLocalIPAddr)
		*pReceiveClientLocalIPAddr = hRevHandle->uTicketIP;

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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamAbortOngoingUserIDTicketValidation");
		(*fptr)(hRevHandle->LegitHandle);
		if (bLogging && bLogUserId) Logger->Write("\t returned.\n");
	}

	auto it = std::find(g_RevUserValidations.begin(), g_RevUserValidations.end(), hRevHandle);
	if (it != g_RevUserValidations.end())
	{
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
		*(void **)(&fptr) = GetProcAddress(g_hOrigSteamDll, "SteamOptionalCleanUpAfterClientHasDisconnected");
		retval = (*fptr)(ObservedClientIPAddr, ClientLocalIPAddr);
		if (bLogging && bLogUserId) Logger->Write("\t %u\n", (int)retval);
		return retval;
	}
	return eSteamErrorNone;
}
