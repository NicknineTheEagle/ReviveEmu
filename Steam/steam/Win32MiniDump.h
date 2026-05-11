#pragma once

class CWin32MiniDump
{
private:
	char m_szName[MAX_PATH];

public:
	CWin32MiniDump(const char* cszName, _se_translator_function fnSETranslatorFunction);
	~CWin32MiniDump();

	void SetComment(const char* cszComment);
	void AddComment(const char* cszComment);
	void ClearComments();
	void WriteUsingExceptionInfo(unsigned int uStructuredExceptionCode, _EXCEPTION_POINTERS* pExceptionInfo);
};
