#pragma once

extern CLogFile* Logger;
extern bool bLogging;
extern bool bLogAcc;


STEAM_API SteamCallHandle_t STEAM_CALL SteamCreateAccount( const char *cszUser, const char *cszEmailAddress, const char *cszPassphrase, const char *cszCreationKey, const char *cszPersonalQuestion, const char *cszAnswerToQuestion, int *pbCreated, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamCreateAccount\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamDeleteAccount( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamDeleteAccount\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangeAccountName( const char *cszCurrentPassphrase, const char *cszNewAccountName, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamChangeAccountName\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangeEmailAddress( const char *cszNewEmailAddress, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamChangeEmailAddress\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangeForgottenPassword( const char *cszUser, const char *cszAnswerToQuestion, const char *cszEmailVerificationKey, const char *cszNewPassphrase, int *pbChanged, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangeForgottenPassword\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangePassword( const char *cszCurrentPassphrase, const char *cszNewPassphrase, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangePassword\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamChangePersonalQA( const char *cszCurrentPassphrase, const char *cszNewPersonalQuestion, const char *cszNewAnswerToQuestion, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamChangePersonalQA\n");
// #endif
	return 1;
}



STEAM_API int STEAM_CALL SteamEnumerateSubscription( unsigned int uId, TSteamSubscription *pSubscription, TSteamError *pError )
{
	if(pSubscription && pError)
	{
// #ifdef DEBUG
		if (bLogging && bLogAcc) Logger->Write("SteamEnumerateSubscription (%u)\n", uId);
// #endif
		return 1;
	}
	return 0;
}

STEAM_API int STEAM_CALL SteamEnumerateSubscriptionDiscount( unsigned int uSubscriptionId, unsigned int uDiscountIndex, TSteamSubscriptionDiscount *pDiscount, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamEnumerateSubscriptionDiscount\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamEnumerateSubscriptionDiscountQualifier( unsigned int uSubscriptionId, unsigned int uDiscountIndex, unsigned int uQualifierIndex, TSteamDiscountQualifier *pDiscountQualifier, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamEnumerateSubscriptionDiscountQualifier\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamGenerateSuggestedAccountNames( const char *cszAccountNameToSelectMasterAS, const char *cszGenerateNamesLikeAccountName, char *pSuggestedNamesBuf, unsigned int uBufSize, unsigned int *puNumSuggestedChars, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGenerateSuggestedAccountNames\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetAccountStatus( unsigned int *puAccountStatusFlags, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetAccountStatus\n");
// #endif
	return 1;
}


STEAM_API int STEAM_CALL SteamGetCurrentEmailAddress( char *szEmailAddress, unsigned int uBufSize, unsigned int *puEmailChars, TSteamError *pError )
{
	if (bLogging && bLogAcc)  Logger->Write("SteamGetCurrentEmailAddress\n");
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamGetNumAccountsWithEmailAddress( const char *cszEmailAddress, unsigned int *puNumAccounts, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetNumAccountsWithEmailAddress\n");
// #endif
	return 1;
}



STEAM_API int STEAM_CALL SteamGetSubscriptionExtendedInfo( unsigned int uSubscriptionId, const char *cszKey, char *szValueBuf, unsigned int uValueBufLen, unsigned *puValueLen, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionExtendedInfo\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionIds( unsigned int *puIds, unsigned int uMaxIds, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionIds (%u)\n", uMaxIds);
// #endif
	return 0;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionPurchaseCountry( unsigned int uSubscriptionId, char *szCountry, unsigned int uBufSize, int *pPurchaseTime, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionPurchaseCountry\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionReceipt( unsigned int uSubscriptionId, TSteamSubscriptionReceipt *pSubscriptionReceipt, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionReceipt\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamGetSubscriptionStats( TSteamSubscriptionStats *pSubscriptionStats, TSteamError *pError )
{
// #ifdef DEBUG
		if (bLogging && bLogAcc)  Logger->Write("SteamGetSubscriptionStats\n");
// #endif
	return 0;
}

//STEAM_API int STEAM_CALL SteamGetUser( char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamError *pError )
STEAM_API int STEAM_CALL SteamGetUser( char *szUser, unsigned int uBufSize, unsigned int *puUserChars, TSteamGlobalUserID *pOptionalReceiveUserID, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc) Logger->Write("SteamGetUser\n");
// #endif

	// 2003 DLL has less arguments.
	TSteamError *pArgError = NULL;
	if (g_eCompatMode <= REV_COMPAT_2003)
	{
		memcpy(&pArgError, &pOptionalReceiveUserID, sizeof(pArgError));
	}
	else
	{
		memcpy(&pArgError, &pError, sizeof(pArgError));
	}

	if (szUser)
	{
		strncpy(szUser, g_szSteamUser, uBufSize);
		if (puUserChars)
			*puUserChars = strlen(g_szSteamUser);
	}

	if (g_eCompatMode > REV_COMPAT_2003)
	{
		if (pOptionalReceiveUserID)
		{
			g_SteamID.ConvertToSteam2(pOptionalReceiveUserID);
		}
	}

	SteamClearError(pArgError);
	return 1;
}

STEAM_API int STEAM_CALL SteamGetUserType( unsigned int *puUserTypeFlags, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamGetUserType\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamIsAccountNameInUse( const char *cszAccountName, int *pbIsUsed, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamIsAccountNameInUse\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamIsLoggedIn( int *pbIsLoggedIn, TSteamError *pError )
{
	if(bLogging) Logger->Write("SteamIsLoggedIn\n");

	*pbIsLoggedIn = 1;
	SteamClearError(pError);
	return 1;
}

STEAM_API int STEAM_CALL SteamIsSecureComputer(  int *pbIsSecure, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamIsSecureComputer\n");
// #endif
	return 0;
}




STEAM_API SteamCallHandle_t STEAM_CALL SteamLogin( const char *cszUser, const char *cszPassphrase, int bIsSecureComputer, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamLogin\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamLogout( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamLogout\n");
// #endif
	return 1;
}


STEAM_API SteamCallHandle_t STEAM_CALL SteamRefreshAccountInfo( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshAccountInfo\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRefreshAccountInfoEx( int bContentDescriptionOnly, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshAccountInfoEx\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRefreshLogin( const char *cszPassphrase, int bIsSecureComputer, TSteamError * pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRefreshLogin\n");
// #endif
	SteamClearError(pError);
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRequestAccountsByCdKeyEmail( const char *cszCdKey, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestAccountsByCdKeyEmail\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRequestAccountsByEmailAddressEmail( const char *cszEmailAddress, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestAccountsByEmailAddressEmail\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRequestEmailAddressVerificationEmail( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestEmailAddressVerificationEmail\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamRequestForgottenPasswordEmail( const char *cszUser, SteamPersonalQuestion_t ReceivePersonalQuestion, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamRequestForgottenPasswordEmail\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamSetUser( const char *cszUser, int *pbUserSet, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamSetUser (%s)\n", cszUser);
// #endif
	strcpy(g_szSteamUser, cszUser);
	*pbUserSet = 1;
	SteamClearError(pError);
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamSubscribe( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamSubscribe\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUnsubscribe( unsigned int uSubscriptionId, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUnsubscribe\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUpdateAccountBillingInfo( const TSteamPaymentCardInfo *pPaymentCardInfo, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUpdateAccountBillingInfo\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamUpdateSubscriptionBillingInfo( unsigned int uSubscriptionId, const TSteamSubscriptionBillingInfo *pSubscriptionBillingInfo, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamUpdateSubscriptionBillingInfo\n");
// #endif
	return 1;
}

STEAM_API SteamCallHandle_t STEAM_CALL SteamVerifyEmailAddress( const char *cszEmailVerificationKey, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamVerifyEmailAddress\n");
// #endif
	return 1;
}

STEAM_API int STEAM_CALL SteamVerifyPassword( const char *cszPassphrase, int *pbCorrect, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamVerifyPassword\n");
// #endif
	return 1;
}



STEAM_API SteamCallHandle_t STEAM_CALL SteamAckSubscriptionReceipt( unsigned int uSubscriptionId, TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamAckSubscriptionReceipt\n");
// #endif
	return 1;
}


STEAM_API unsigned int STEAM_CALL SteamNumAppsRunning( TSteamError *pError )
{
// #ifdef DEBUG
	if (bLogging && bLogAcc)  Logger->Write("SteamNumAppsRunning\n");
// #endif
	return 1;
}
