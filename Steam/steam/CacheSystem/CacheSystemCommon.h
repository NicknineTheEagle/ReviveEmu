typedef intptr_t CacheHandle;

typedef enum
{
	eSteamCacheFileTypeGCF = 1,
	eSteamCacheFileTypeNCF = 2
} ESteamCacheFileType;

typedef struct
{
	unsigned int Dummy0;		
	ESteamCacheFileType Type;	
	unsigned int CacheFileVersion;	
	unsigned int CacheID;
	unsigned int CacheVersion;
	bool IsMounted;
	unsigned int Dummy2;
	unsigned int FileSize;		
	unsigned int PhysicalSectorSize;	
	unsigned int SectorCount;	
	unsigned int Checksum;
}TCacheHeader;

typedef struct
{
	unsigned int BlockCount;
	unsigned int BlocksUsed;
	unsigned int LastUsedBlock;
	unsigned int Dummy1;
	unsigned int Dummy2;
	unsigned int Dummy3;
	unsigned int Dummy4;
	unsigned int Checksum;		
}TCacheBlockHeader;

typedef struct
{
	unsigned int EntryType;				
	unsigned int FileDataOffset;	
	unsigned int FileDataSize;		
	unsigned int FirstDataBlockIndex;	
	unsigned int NextBlockEntryIndex;	
	unsigned int PreviousBlockEntryIndex;	
	unsigned int DirectoryIndex;		
}TCacheBlockEntry;

typedef struct
{
	unsigned int BlockCount;
	unsigned int Dummy0;
	unsigned int Dummy1;
	unsigned int Checksum;	
}TCacheAllocationTableHeader;

typedef struct
{
	unsigned int NextDataBlockIndex;	
}TCacheAllocationTableEntry;

typedef struct
{
	unsigned int Dummy0;		
	unsigned int CacheID;			
	unsigned int CacheVersion;		
	unsigned int ItemCount;				
	unsigned int FileCount;			
	unsigned int LogicalSectorSize;	
	unsigned int ManifestSize;		
	unsigned int NameSize;		
	unsigned int Info1Count;	
	unsigned int CopyCount;	
	unsigned int LocalCount;	
	unsigned int Dummy1;
	unsigned int Dummy2;
	unsigned int Checksum;	
}TCacheManifestHeader;

typedef struct
{
	unsigned int NameOffset;
	unsigned int ItemSize;		
	unsigned int ChecksumIndex;
	unsigned int Type;
	unsigned int ParentIndex;	
	unsigned int NextIndex;	
	unsigned int FirstIndex;	
}TCacheManifestEntry;

typedef struct
{
	unsigned int Dummy0;
}TCacheManifestInfo1Entry;

typedef struct
{
	unsigned int Dummy0;
}TCacheManifestInfo2Entry;

typedef struct
{
	unsigned int DirectoryIndex;
}TCacheManifestCopyEntry;

typedef struct
{
	unsigned int DirectoryIndex;
}TCacheManifestLocalEntry;

typedef struct
{
	unsigned int Dummy0;	
	unsigned int Dummy1;	
}TCacheManifestMapHeader;

typedef struct
{
	unsigned int FirstBlockIndex;
}TCacheManifestMapEntry;

typedef struct
{
	unsigned int Dummy0;		
	unsigned int ChecksumSize;	
}TCacheChecksumTableHeader;

typedef struct
{
	unsigned int Dummy0;	
	unsigned int Dummy1;	
	unsigned int ItemCount;		
	unsigned int ChecksumCount;	
}TCacheChecksumTableMapHeader;

typedef struct
{
	unsigned int ChecksumCount;		
	unsigned int FirstChecksumIndex;
}TCacheChecksumTableMapEntry;

typedef struct
{
	unsigned int Checksum;
}TCacheChecksumTableEntry;

typedef struct
{
	unsigned int CacheVersion;
	unsigned int SectorCount;		
	unsigned int PhysicalSectorSize;
	unsigned int FirstSectorOffset;	
	unsigned int SectorsUsed;	
	unsigned int Checksum;			
}TCacheSectorsHeader;

typedef struct
{
	void* pCache;
	char* Name;
	char* FullName;
	unsigned int Size;
	unsigned int Type;
	unsigned int Flags;
	unsigned int* Sectors;
	unsigned int numofsectors;
	unsigned int* Cheksums;
	unsigned int numofcheksums;
	unsigned int ParentIndex;
	unsigned int NextIndex;
	unsigned int FirstIndex;
	bool IsFragmented;
	bool IsLocalCopyMade;
	bool IsLocalCopyHasPriorityOverChache;

}TManifestEntriesInCache;

typedef struct
{
	SteamHandle_t hSteamHandle;
	bool IsFileLocal;
	union
	{
		FILE* LocalFile;
		TManifestEntriesInCache* FileInCache;
	};
	unsigned int Position;
	char mode[8];
}TFileInCacheHandle;

typedef struct
{
	SteamHandle_t hSteamHandle;
	bool IsFindLocal;
	union
	{
		intptr_t LocalFind;
		TManifestEntriesInCache* hFind;
	};
	ESteamFindFilter eFilter;
	char szCachePattern[MAX_PATH];
	unsigned int CurrentIndex;
	char szPattern[MAX_PATH];
}TFindHandle;
