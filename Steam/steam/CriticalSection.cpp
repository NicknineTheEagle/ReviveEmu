#include "stdafx.h"
#include "CriticalSection.h"

CInitCriticalSection::CInitCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	this->m_pCriticalSection = lpCriticalSection;
	InitializeCriticalSection(lpCriticalSection);
}

CInitCriticalSection::~CInitCriticalSection()
{
	DeleteCriticalSection(this->m_pCriticalSection);
}

CEnterCriticalSection::CEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection)
{
	this->m_pCriticalSection = lpCriticalSection;
	EnterCriticalSection(this->m_pCriticalSection);
}

CEnterCriticalSection::~CEnterCriticalSection()
{
	LeaveCriticalSection(this->m_pCriticalSection);
}