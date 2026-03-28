#pragma once

extern CLogFile* Logger;
extern BOOL bLogging;
extern BOOL bLogAcc;
extern BOOL bSteamClient;


STEAM_API SteamCallHandle_t STEAM_CALL SteamCreateAccount(const char *cszUser, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, const char* cszArg6, int *pbCreated, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamCreateAccount\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamDeleteAccount(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamDeleteAccount\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamChangeAccountName(const char* cszCurrentAccountName, const char* cszNewAccountName, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamChangeAccountName\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangeEmailAddress(const char* cszNewEmailAddress, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamChangeEmailAddress\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamChangeForgottenPassword()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangeForgottenPassword\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangePassword(const char* cszCurrentPassphrase, const char* cszNewPassphrase, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangePassword\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangePersonalQA(const char* cszCurrentPassphrase, const char* cszNewPersonalQuestion, const char* cszNewAnswerToQuestion, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangePersonalQA\n");
// #endif
	return 1;
}



STEAM_API int STEAM_CALL SteamEnumerateSubscription(unsigned int uSubId, TSteamSubscription *pSubscription, TSteamError *pError)
{
	if(pSubscription && pError)
	{
// #ifdef DEBUG
		if (bLogging && bLogAcc)  Logger->Write("SteamEnumerateSubscription (%u)\n", uSubId);
// #endif
		return 1;
	}
	return 0;
}

STEAM_API int STEAM_CALL SteamEnumerateSubscriptionDiscount()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamEnumerateSubscriptionDiscount\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamEnumerateSubscriptionDiscountQualifier()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamEnumerateSubscriptionDiscountQualifier\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGenerateSuggestedAccountNames(const char* cszArg1, const char* cszArg2, char* szArg3, unsigned int uArg4, unsigned int* puArg5, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGenerateSuggestedAccountNames\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetAccountStatus()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetAccountStatus\n");
// #endif
	return 1;
}


STEAM_API int STEAM_CALL SteamGetCurrentEmailAddress(char *szEmailaddress, unsigned int uBufSize, unsigned int *puEmailaddressChars, TSteamError *pError)
{
	if (bLogging && bLogAcc)  Logger->Write("SteamGetCurrentEmailAddress\n");
	return 1;
}

STEAM_API int STEAM_CALL SteamGetNumAccountsWithEmailAddress()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetNumAccountsWithEmailAddress\n");
// #endif
	return 1;
}



STEAM_API int STEAM_CALL SteamGetSubscriptionExtendedInfo()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionExtendedInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionIds(unsigned int *puSubIds, unsigned int uMaxIds, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionIds (%u)\n", uMaxIds);
// #endif
	return 0;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionPurchaseCountry()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionPurchaseCountry\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionReceipt(unsigned int uSubscriptionId , TSteamSubscriptionReceipt* pSteamSubscriptionReceipt, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionReceipt\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionStats(TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError)
{
// #ifdef DEBUG
		if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionStats\n");
// #endif
	return 0;
}

//STEAM_API int STEAM_CALL SteamGetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamError *pError)
STEAM_API int STEAM_CALL SteamGetUser(char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID* pSteamGlobalUserID, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamGetUser\n");
// #endif

	// 2003 DLL has less arguments.
	TSteamError *pArgError = NULL;
	if (g_CompatMode <= REV_COMPAT_2003)
	{
		memcpy(&pArgError, &pSteamGlobalUserID, sizeof(pArgError));
	}
	else
	{
		memcpy(&pArgError, &pError, sizeof(pArgError));
	}

	if (szUser)
	{
		strncpy(szUser, szSteamUser, uBufSize);
		if (puUserChars)
			*puUserChars = strlen(szSteamUser);
	}
	
	if (g_CompatMode > REV_COMPAT_2003)
	{
		if (pSteamGlobalUserID)
		{
			g_SteamID.ConvertToSteam2(pSteamGlobalUserID);
		}
	}

	SteamClearError(pArgError);
	return 1;
}

STEAM_API int STEAM_CALL SteamGetUserType(unsigned int* puArg1, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetUserType\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamIsAccountNameInUse()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamIsAccountNameInUse\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamIsLoggedIn(int *pbIsLoggedIn, TSteamError *pError)
{
	if(bLogging) Logger->Write("SteamIsLoggedIn\n");
	
	*pbIsLoggedIn = 1;
	SteamClearError(pError);
	return 1;
}

STEAM_API int STEAM_CALL SteamIsSecureComputer(int *pbIsSecure, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamIsSecureComputer\n");
// #endif
	return 0;
}




STEAM_API SteamCallHandle_t STEAM_CALL SteamLogin(const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamLogin\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamLogout(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamLogout\n");
// #endif
	return 1;
}


STEAM_API int STEAM_CALL SteamRefreshAccountInfo()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshAccountInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamRefreshAccountInfoEx()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshAccountInfoEx\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRefreshLogin(const char *cszPassphrase, int bIsSecureComputer, TSteamError * pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshLogin\n");
// #endif
	SteamClearError(pError);
	return 1;
}

STEAM_API int STEAM_CALL SteamRequestAccountsByCdKeyEmail()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestAccountsByCdKeyEmail\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamRequestAccountsByEmailAddressEmail()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestAccountsByEmailAddressEmail\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamRequestEmailAddressVerificationEmail(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestEmailAddressVerificationEmail\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamRequestForgottenPasswordEmail()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestForgottenPasswordEmail\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamSetUser(const char *cszUser, int *pbUserSet, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamSetUser (%s)\n", cszUser);
// #endif
	strcpy(szSteamUser, cszUser);
	*pbUserSet = 1;
	SteamClearError(pError);
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamSubscribe(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamSubscribe\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUnsubscribe(unsigned int uSubscriptionId, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUnsubscribe\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUpdateAccountBillingInfo(const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUpdateAccountBillingInfo\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUpdateSubscriptionBillingInfo(unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUpdateSubscriptionBillingInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamVerifyEmailAddress(const char* cszEmailAddress, TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamVerifyEmailAddress\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamVerifyPassword(const char* cszArg1, int* piArg2, TSteamError* pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamVerifyPassword\n");
// #endif
	return 1;
}



STEAM_API int STEAM_CALL SteamAckSubscriptionReceipt()
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamAckSubscriptionReceipt\n");
// #endif
	return 1;
}


STEAM_API int STEAM_CALL SteamNumAppsRunning(TSteamError *pError)
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamNumAppsRunning\n");
// #endif
	return 1;
}

