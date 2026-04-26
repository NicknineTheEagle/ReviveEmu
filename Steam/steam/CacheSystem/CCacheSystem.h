#include "CacheSystemCommon.h"
#include "Cache/CCache.h"

#include "../MurmurHash.h"
#include "../strtools.h"

#define MURMUR_SEED 0x44444444

typedef struct
{
	FILE* fCacheFile;
	CCache* hCacheFile;
} TCacheHandle;

typedef struct
{
	void* pCache;
	char* FullName;
	//char FullName[MAX_PATH];
	unsigned int Index;
} TGlobalDirectory;

class TGlobalDirectoryKey
{
public:
	const char* FullName;

	TGlobalDirectoryKey(const char* _FullName)
	{
		FullName = _FullName;
	}

	bool operator==(const TGlobalDirectoryKey& other) const
	{
		return (strcmp(FullName, other.FullName) == 0);
	}
};

namespace std
{
template<>
class hash<TGlobalDirectoryKey>
{
public:
	size_t operator()(const TGlobalDirectoryKey& k) const
	{
		uint32_t EntryHash = murmur3_32((const uint8_t*)k.FullName, strlen(k.FullName), MURMUR_SEED);
		return (size_t)EntryHash;
	}
};
}

extern TFileInCacheHandle* NewSteamFileHandle();
extern void CloseSteamFileHandle(SteamHandle_t hSteamHandle);
extern TFindHandle* NewSteamFindFileHandle();
extern void CloseSteamFindFileHandle(SteamHandle_t hSteamHandle);

class CCacheFileSystem
{
public:
	std::vector<TCacheHandle*> Caches;

	std::vector<TGlobalDirectory> GlobalDirectoryTable;
	std::unordered_map<TGlobalDirectoryKey, TGlobalDirectory> HashTable;

	CCacheFileSystem()
	{
	}

	~CCacheFileSystem()
	{
		// Unmount everything.
		UnmountAll();
	}

	CacheHandle MountCache(const char* cszFileName, const char* ExtraMountPath)
	{
		const char* cszCacheName = V_GetFileName(cszFileName);

		for (TCacheHandle* hCache : Caches)
		{
			CCache* CheckCacheFile = hCache->hCacheFile;
			if (strcmp(CheckCacheFile->Name, cszCacheName) == 0)
			{
				if (bLogging && bLogFS) Logger->Write("	Cache is already mounted: %s\n", cszFileName);
				return 0;
			}
		}

		if (FILE* fCacheFile = fopen(cszFileName, "rb"))
		{
			CCache* CacheFile = new CCache(fCacheFile);
			CacheFile->Read(ExtraMountPath);
			CacheFile->bIsMounted = false;
			strncpy(CacheFile->Name, cszCacheName, sizeof(CacheFile->Name));
			TCacheHandle* hCache = new TCacheHandle;
			hCache->fCacheFile = fCacheFile;
			hCache->hCacheFile = CacheFile;
			Caches.push_back(hCache);

			BuildGlobalDirectoryTable(CacheFile);

			if (bLogging && bLogFS) Logger->Write("	Mounted %s\n", cszFileName);

			return (CacheHandle)hCache;
		}

		if (bLogging && bLogFS) Logger->Write("	Failed to Mount %s\n", cszFileName);

		return 0;
	}

	bool UnmountCache(CacheHandle hCacheToMount)
	{
		auto it = std::find(Caches.begin(), Caches.end(), (TCacheHandle*)hCacheToMount);
		if (it != Caches.end())
		{
			TCacheHandle* hCache = *it;
			//if (bLogging && bLogFS) Logger->Write("	Unmounted %s\n", hCache->hCacheFile->Name);
			delete hCache->hCacheFile;
			fclose(hCache->fCacheFile);
			delete hCache;
			Caches.erase(it);
			return true;
		}

		return false;
	}

	void UnmountAll()
	{
		// Clear file tables.
		HashTable.clear();
		GlobalDirectoryTable.clear();

		// Unmount everything.
		for (TCacheHandle* hCache : Caches)
		{
			UnmountCache((CacheHandle)hCache);
		}
	}

	TFileInCacheHandle* CacheOpenFileEx(const char* cszFileName, const char* cszMode, unsigned int* puSize)
	{
		if (strpbrk(cszMode, "wa+"))
			return NULL;

		if (TManifestEntriesInCache* FileToOpen = CacheFindFile(cszFileName))
		{
			TFileInCacheHandle* hFile = NewSteamFileHandle();
			hFile->FileInCache = FileToOpen;

			if (puSize)
			{
				*puSize = FileToOpen->Size;
			}

			strncpy(hFile->mode, cszMode, sizeof(hFile->mode));

			//LoggerFileOpened = true;

			//CCache* CacheFile = (CCache*)FileToOpen->pCache;
			//if (bLogging && bLogFS) Logger->Write("\tOpened 0x%08X from cache(%s) {%s} %s\n", (long)hFile, cszMode, CacheFile->Name, cszFileName );
			//if (bLogging && bLogFS) Logger->Write("\tOpened 0x%08X from cache(%s) %s\n", (long)hFile, cszMode, cszFileName );

			return hFile;
		}

		return NULL;
	}

	int CacheStat(const char* cszFileName, TSteamElemInfo* pInfo)
	{
		if (TManifestEntriesInCache* ItemFound = CacheFindFile(cszFileName))
		{
			pInfo->bIsDir = (ItemFound->Type == 0 ? 1 : 0);
			pInfo->uSizeOrCount = ItemFound->Size;
			pInfo->bIsLocal = 0;
			strcpy(pInfo->cszName, ItemFound->Name);
			pInfo->lCreationTime = 0x44444444;
			pInfo->lLastAccessTime = 0x44444444;
			pInfo->lLastModificationTime = 0x44444444;
			//if (bLogging && bLogFS) Logger->Write("\tStat succesfull from cache > %s\n", cszFileName);

			return 0;
		}

		return -1;
	}

	int CacheCloseFile(TFileInCacheHandle* hFile)
	{
		if (hFile)
		{
			CloseSteamFileHandle(hFile->hSteamHandle);
			return 0;
		}
		return EOF;
	}

	int CacheExtractFile(TFileInCacheHandle* hFile, const char* czDest)
	{
		TManifestEntriesInCache* FileToExtract = hFile->FileInCache;
		char szFullPath[MAX_PATH];

		if (czDest)
		{
			strcpy(szFullPath, czDest);
		}
		else
		{
			V_GetCurrentDirectory(szFullPath, MAX_PATH);
		}

		V_ComposeFileName(szFullPath, FileToExtract->FullName, szFullPath, MAX_PATH);

		char szFilePath[MAX_PATH];
		V_ExtractFilePath(szFullPath, szFilePath, MAX_PATH);
		CreateDirHierarchy(szFilePath);

		if (FILE* FileToWrite = fopen(szFullPath, "wb"))
		{
			unsigned int uBytesLeft = CacheSizeFile(hFile);
			unsigned int uBuffSize = 512 * 1024;
			char* Buff = new char[uBuffSize];
			while (uBytesLeft > 0)
			{
				unsigned int uWriteSize = min(uBuffSize, uBytesLeft);
				CacheReadFile(Buff, 1, uWriteSize, hFile);
				fwrite(Buff, 1, uWriteSize, FileToWrite);
				uBytesLeft -= uWriteSize;
			}
			delete[] Buff;
			fclose(FileToWrite);
			//if (bLogging && bLogFS) Logger->Write("\tFound cache (%s) Extracted to > %s\n", FileToExtract->FullName,  szFilePath);
			return 1;
		}
		//if (bLogging && bLogFS) Logger->Write("\tFound cache (%s) FAILED to Extract to > %s\n", FileToExtract->FullName,  szFilePath);
		return 0;
	}

	unsigned int CacheReadFile(void* pBuf, unsigned int uSize, unsigned int uCount, TFileInCacheHandle* hFile)
	{
		unsigned int readedlength = 0;

		if (hFile)
		{
			if (strchr(hFile->mode, 'b'))
			{
				readedlength = ReadBinary(pBuf, uSize, uCount, hFile);
			}
			else
			{
				readedlength = ReadText(pBuf, uSize, uCount, hFile);
			}
		}

		return readedlength; // needs to return count
	}

	unsigned int ReadBinary(void* pBuf, unsigned int uSize, unsigned int uCount, TFileInCacheHandle* hFile)
	{
		CCache* hCacheFile = (CCache*)hFile->FileInCache->pCache;
		TManifestEntriesInCache* hFileInCache = hFile->FileInCache;

		char* szBuff = (char*)pBuf;
		unsigned int uReadedDataAmount = 0;
		unsigned int uTotalDataToRead = uSize * uCount;

		unsigned int uActualSectorIndex = hFile->Position / hCacheFile->Sectors->Header->PhysicalSectorSize;

		while (uReadedDataAmount < uTotalDataToRead)
		{
			unsigned int uOffset = hFile->Position % hCacheFile->Sectors->Header->PhysicalSectorSize;
			unsigned int uActualDataToRead = min(uTotalDataToRead - uReadedDataAmount,
				hCacheFile->Sectors->Header->PhysicalSectorSize - uOffset);

			unsigned int uReadedData = BinaryReadSector(uActualSectorIndex, szBuff, uOffset, uActualDataToRead, hFile);

			hFile->Position += uReadedData;
			uReadedDataAmount += uReadedData;

			if (uActualDataToRead != uReadedData)
			{
				// EOF reached
				break;
			}

			szBuff += uReadedData;

			uActualSectorIndex++;
		}

		return uReadedDataAmount;
	}

	unsigned int BinaryReadSector(unsigned int iSectorIndex, void* pvBuff, unsigned int uOffset, unsigned int uSize, TFileInCacheHandle* hFile)
	{
		CCache* hCacheFile = (CCache*)hFile->FileInCache->pCache;
		FILE* fCacheFile = hCacheFile->fCacheFile;
		TManifestEntriesInCache* hFileInCache = hFile->FileInCache;

		if (hFile->Position >= hFileInCache->Size) // EOF
			return 0;

		if (hFile->Position + uSize > hFileInCache->Size)
			uSize = hFileInCache->Size - hFile->Position;

		int64 lReadPosition = (int64)hCacheFile->Sectors->Header->FirstSectorOffset +
		                      (int64)(hFileInCache->Sectors[iSectorIndex]) * (int64)(hCacheFile->Sectors->Header->PhysicalSectorSize) +
		                      (int64)uOffset;
#ifdef _WIN32		
		if (_fseeki64(fCacheFile, lReadPosition, SEEK_SET))
			return 0;
#else
		if (fseeko(fCacheFile, lReadPosition, SEEK_SET))
			return 0;
#endif

		return fread(pvBuff, 1, uSize, fCacheFile);
	}

	unsigned int ReadText(void* pBuf, unsigned int uSize, unsigned int uCount, TFileInCacheHandle* hFile)
	{
		CCache* hCacheFile = (CCache*)hFile->FileInCache->pCache;
		TManifestEntriesInCache* hFileInCache = hFile->FileInCache;

		char* BufferedSector = new char[hCacheFile->Sectors->Header->PhysicalSectorSize];
		unsigned int BufferedSectorIndex = UINT_MAX;

		char* szBuffOut = (char*)pBuf;
		unsigned int uReadedDataAmount = 0;
		unsigned int uReadedCharactersAmount = 0;
		unsigned int uTotalDataToRead = uSize * uCount;

		unsigned int uActualSectorIndex = hFile->Position / hCacheFile->Sectors->Header->PhysicalSectorSize;

		while (uReadedDataAmount < uTotalDataToRead)
		{
			if (hFile->Position >= hFileInCache->Size) // EOF
				break;

			if (BufferedSectorIndex != uActualSectorIndex)
			{
				if (TextReadSector(uActualSectorIndex, BufferedSector, hCacheFile->Sectors->Header->PhysicalSectorSize, hFile) !=
				    hCacheFile->Sectors->Header->PhysicalSectorSize)
					break;

				BufferedSectorIndex = uActualSectorIndex;
			}

			unsigned int uOffset = hFile->Position % hCacheFile->Sectors->Header->PhysicalSectorSize;
			unsigned int uActualDataToRead = min(uTotalDataToRead - uReadedDataAmount,
				hCacheFile->Sectors->Header->PhysicalSectorSize - uOffset);
			unsigned int uDataAvailableToRead = min(hFileInCache->Size - hFile->Position,
				hCacheFile->Sectors->Header->PhysicalSectorSize - uOffset);

			unsigned int uReadedCharacters = 0;

			if (uActualDataToRead > 0)
			{
				unsigned int uReadedData = 0;

				do
				{
					char ucChar = BufferedSector[uOffset + uReadedData];
					if (ucChar != '\r')
					{
						szBuffOut[uReadedCharactersAmount + uReadedCharacters] = ucChar;
						uReadedCharacters++;
					}
					uReadedData++;
				} while (uReadedCharacters < uActualDataToRead && uReadedData < uDataAvailableToRead);

				hFile->Position += uReadedData;
				uReadedDataAmount += uReadedData;
				uReadedCharactersAmount += uReadedCharacters;
			}

			uActualSectorIndex++;
		}

		delete[] BufferedSector;
		return uReadedCharactersAmount;
	}

	unsigned int TextReadSector(unsigned int iSectorIndex, void* pvBuff, unsigned int uSize, TFileInCacheHandle* hFile)
	{
		CCache* hCacheFile = (CCache*)hFile->FileInCache->pCache;
		FILE* fCacheFile = hCacheFile->fCacheFile;
		TManifestEntriesInCache* hFileInCache = hFile->FileInCache;

		int64 lReadPosition = (int64)hCacheFile->Sectors->Header->FirstSectorOffset +
		                      (int64)(hFileInCache->Sectors[iSectorIndex]) * (int64)(hCacheFile->Sectors->Header->PhysicalSectorSize);
#ifdef _WIN32		
		if (_fseeki64(fCacheFile, lReadPosition, SEEK_SET))
			return 0;
#else
		if (fseeko(fCacheFile, lReadPosition, SEEK_SET))
			return 0;
#endif

		return fread(pvBuff, 1, uSize, fCacheFile);
	}

	int CacheSeekFile(TFileInCacheHandle* hFile, long lOffset, ESteamSeekMethod esMethod)
	{
		int retval = -1;

		switch (esMethod)
		{
			case eSteamSeekMethodSet:
				if (lOffset >= 0)
				{
					hFile->Position = lOffset;
					retval = 0;
				}
				break;
			case eSteamSeekMethodCur:
				if (((long)hFile->Position + lOffset) >= 0)
				{
					hFile->Position += lOffset;
					retval = 0;
				}
				break;
			case eSteamSeekMethodEnd:
				if (lOffset <= (long)hFile->FileInCache->Size)
				{
					hFile->Position = hFile->FileInCache->Size - lOffset;
					retval = 0;
				}
				break;
			default:
				break;
		}

		return retval;
	}

	int CacheTellFile(TFileInCacheHandle* hFile)
	{
		if (hFile)
		{
			return hFile->Position;
		}
		return -1;
	}

	int CacheSizeFile(TFileInCacheHandle* hFile)
	{
		if (hFile)
		{
			return hFile->FileInCache->Size;
		}
		return -1;
	}

	void CreateDirHierarchy(const char* cszRelativePath)
	{
		char szScratchFileName[MAX_PATH];
		V_MakeAbsolutePath(szScratchFileName, MAX_PATH, cszRelativePath);

		int len = strlen(szScratchFileName) + 1;
		char* end = szScratchFileName + len;
		for (char* s = szScratchFileName; s < end; s++)
		{
			if (*s != CORRECT_PATH_SEPARATOR || s == szScratchFileName)
				continue;

#if defined(_WIN32)
			if (*(s - 1) == ':')
				continue;
#endif

			*s = '\0';
#if defined(_WIN32)
			_mkdir(szScratchFileName);
#elif defined(_LINUX)
			mkdir(szScratchFileName, S_IRWXU | S_IRGRP | S_IROTH); // owner has rwx, rest have r
#endif
			*s = CORRECT_PATH_SEPARATOR;
		}

#if defined(_WIN32)
		_mkdir(szScratchFileName);
#elif defined(_LINUX)
		mkdir(szScratchFileName, S_IRWXU | S_IRGRP | S_IROTH);
#endif
	}

	TFindHandle* CacheFindFirst(const char* cszPattern, ESteamFindFilter eFilter, TSteamElemInfo* pFindInfo)
	{
		char szCachePattern[MAX_PATH];
		strcpy(szCachePattern, cszPattern);

		if (!GetFilenameInCache(szCachePattern))
			return NULL;

		if (szCachePattern[0])
		{
			unsigned int CurrentIndex = 0;
			if (TManifestEntriesInCache* ItemFound = FindItem(0, szCachePattern, &CurrentIndex))
			{
				pFindInfo->bIsDir = (ItemFound->Type == 0 ? 1 : 0);
				pFindInfo->bIsLocal = 0;
				strcpy(pFindInfo->cszName, ItemFound->Name);
				pFindInfo->uSizeOrCount = ItemFound->Size;
				pFindInfo->lCreationTime = 0x44444444;
				pFindInfo->lLastAccessTime = 0x44444444;
				pFindInfo->lLastModificationTime = 0x44444444;

				TFindHandle* hFind = NewSteamFindFileHandle();
				hFind->IsFindLocal = false;
				hFind->hFind = ItemFound;
				hFind->eFilter = eFilter;
				strcpy(hFind->szCachePattern, szCachePattern);
				strcpy(hFind->szPattern, cszPattern);

				hFind->CurrentIndex = CurrentIndex;

				//if (bLogging && bLogFS) Logger->Write("\tFound first file {%s} (%s) using pattern (%s)\n", ((CCache*)hFind->hFind->pCache)->Name, ItemFound->Name, szCachePattern);

				return hFind;
			}
		}

		return NULL;
	}

	int CacheFindNext(TFindHandle* hFind, TSteamElemInfo* pFindInfo)
	{
		unsigned int CurrentIndex = 0;
		if (TManifestEntriesInCache* ItemFound = FindItem(hFind->CurrentIndex + 1, hFind->szCachePattern, &CurrentIndex))
		{
			pFindInfo->bIsDir = (ItemFound->Type == 0 ? 1 : 0);
			pFindInfo->bIsLocal = 0;
			strcpy(pFindInfo->cszName, ItemFound->Name);
			pFindInfo->uSizeOrCount = ItemFound->Size;
			pFindInfo->lCreationTime = 0x44444444;
			pFindInfo->lLastAccessTime = 0x44444444;
			pFindInfo->lLastModificationTime = 0x44444444;
			hFind->hFind = ItemFound;
			hFind->CurrentIndex = CurrentIndex;

			//if (bLogging && bLogFS) Logger->Write("\tFound next file {%s} (%s) using pattern (%s)\n", ((CCache*)hFind->hFind->pCache)->Name, ItemFound->Name, hFind->szCachePattern);

			return 0;
		}

		return -1;
	}

	int CacheFindClose(TFindHandle* hFind)
	{
		if (hFind)
		{
			CloseSteamFindFileHandle(hFind->hSteamHandle);
			return 0;
		}
		return -1;
	}

private:
	TManifestEntriesInCache* FindItem(unsigned int StartIndex, const char* cszPattern, unsigned int *pItemIndex = NULL)
	{
		if (strpbrk(cszPattern, "?*"))
		{
			for (unsigned int i = StartIndex; i < GlobalDirectoryTable.size(); i++)
			{
				TGlobalDirectory& FindItem = GlobalDirectoryTable[i];

				if (IsMatchingWithMask(FindItem.FullName, cszPattern))
				{
					CCache* CacheFile = (CCache*)FindItem.pCache;
					TManifestEntriesInCache* ItemFound = &CacheFile->DirectoryTable[FindItem.Index];
					if (pItemIndex)
						*pItemIndex = i;
					return ItemFound;
				}
			}
		}
		else
		{
			auto it = HashTable.find(cszPattern);

			if (it != HashTable.end())
			{
				TGlobalDirectory& FindItem = it->second;

				CCache* CacheFile = (CCache*)FindItem.pCache;
				TManifestEntriesInCache* ItemFound = &CacheFile->DirectoryTable[FindItem.Index];
				if (ItemFound)
				{
					if (pItemIndex)
						*pItemIndex = 0;
					return ItemFound;
				}
			}
		}

		return NULL;
	}

	bool IsMatchingWithMask(const char* szString, const char* szMask)
	{
		while (*szString != 0 || *szMask != 0)
		{
			if (*szMask == '*')
			{
				szMask++;
				while (*szMask != *szString)
				{
					if (*szString == 0)
					{
						break;
					}
					else if (*szString == '\\')
					{
						break;
					}
					else
					{
						szString++;
					}
				}
			}
			else if (*szString == 0 && *szMask == 0)
			{
				break;
			}
			else if (*szString == 0 && *szMask != 0)
			{
				if (*szMask == '.')
				{
					szMask++;
				}
				if (*szMask == '*')
				{
					szMask++;
				}
				if (*szString == 0 && *szMask != 0)
				{
					return false;
				}
				else
				{
					break;
				}
			}
			else if (*szString != 0 && *szMask == 0)
			{
				return false;
			}
			else if (*szMask == '?')
			{
				szString++;
				szMask++;
			}
			else if (*szString == *szMask)
			{
				szString++;
				szMask++;
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	bool GetFilenameInCache(char* cszFileName)
	{
		char szBuf[MAX_PATH];
		char szCWD[MAX_PATH];

		if (!V_GetCurrentDirectory(szCWD, MAX_PATH))
			return false;

		// Covert to lowercase for lookup.
		V_strlower(cszFileName);

		if (V_IsAbsolutePath(cszFileName))
		{
			if (!V_RemoveDotSlashes(cszFileName))
				return false;

			if (!V_MakeRelativePath(cszFileName, szCWD, szBuf, MAX_PATH))
				return false;

			// Normalize slashes.
			V_FixSlashes(szBuf, '\\');

			// Parsed cache paths start with a slash.
			cszFileName[0] = '\\';
			strcpy(cszFileName + 1, szBuf);
		}
		else
		{
			if (!V_RemoveDotSlashes(cszFileName, '\\', true))
				return false;

			if (cszFileName[0] != '\\')
			{
				size_t len = strlen(cszFileName) + 1;
				memmove(cszFileName + 1, cszFileName, len);
				cszFileName[0] = '\\';
			}
		}

		//if (bLogging && bLogFS)Logger->Write("\tSearching cache with (%s - %s)\n", cszFileName, szCWD);

		return true;
	}

	TManifestEntriesInCache* CacheFindFile(const char* cszFileName)
	{
		char szFileNamebuffer[MAX_PATH];
		strcpy(szFileNamebuffer, cszFileName);

		if (!GetFilenameInCache(szFileNamebuffer))
			return NULL;

		if (szFileNamebuffer[0])
		{
			if (TManifestEntriesInCache* ItemFound = FindItem(0, szFileNamebuffer))
			{
				return ItemFound;
			}
		}

		return NULL;
	}

  private:
	void BuildGlobalDirectoryTable(CCache* CacheFile)
	{
		if (!CacheFile->bIsMounted)
		{
			TManifestEntriesInCache* DirectoryTable = CacheFile->DirectoryTable;
			TManifestEntriesInCache* ActualDirEntry;

			for (unsigned int CacheIndex = 1; CacheIndex < CacheFile->Manifest->Header->ItemCount; CacheIndex++)
			{
				TGlobalDirectory FindThisItem;
				ActualDirEntry = &DirectoryTable[CacheIndex];

				FindThisItem.pCache = ActualDirEntry->pCache;
				FindThisItem.FullName = ActualDirEntry->FullName;
				FindThisItem.Index = CacheIndex;

				auto result = HashTable.insert({FindThisItem.FullName, FindThisItem});

				if (result.second)
				{
					GlobalDirectoryTable.push_back(FindThisItem);

					//if (bLogging && bLogFS) Logger->Write("\tAdd to Global Directory: %s\n", ActualDirEntry->FullName);
				}
				else
				{
					//if (bLogging && bLogFS) Logger->Write("\tExcluded from Global Directory: %s\n", ActualDirEntry->FullName);
				}
			}

			CacheFile->bIsMounted = true;
		}
	}
};
