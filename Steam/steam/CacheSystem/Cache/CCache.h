#pragma once

class CCache
{

#include "CCacheBlock.h"
#include "CCacheAllocationTable.h"
#include "CCacheManifest.h"
#include "CCacheChecksumTable.h"
#include "CCacheSectors.h"

public:
	FILE* fCacheFile;
	TCacheHeader* Header;
	CCacheBlock* Block;
	CCacheAllocationTable* AllocationTable;
	CCacheManifest* Manifest;
	CCacheChecksumTable* ChecksumTable;
	CCacheSectors* Sectors;
	TManifestEntriesInCache* DirectoryTable;
	char* Name;

	unsigned int Index;
	bool bIsMounted;

	CCache(FILE* fCacheFile)
	{
		this->fCacheFile = fCacheFile;
		this->Name = new char[255];
	}
	
	~CCache()
	{
		if (DirectoryTable)
		{
			for (unsigned int x = 1; x < Manifest->Header->ItemCount ; x++)
			{
				delete [] DirectoryTable[x].FullName; 
				delete [] DirectoryTable[x].Name;

				if (DirectoryTable[x].numofsectors != 0)
					delete [] DirectoryTable[x].Sectors;
			}
			delete [] DirectoryTable;
		}
		if (Name)
			delete [] Name;
		if (Block)
			delete Block;
		if (AllocationTable)
			delete AllocationTable;
		if (Manifest)
			delete Manifest;
		if (ChecksumTable)
			delete ChecksumTable;
		if (Sectors)
			delete Sectors;
		if (Header)
			delete Header;
		HashTable.clear();
	}
	
	bool Read(const char* ExtraMountPath)
	{
		fseek(fCacheFile, 0, SEEK_SET);
		Header = new TCacheHeader;
		fread((void*)Header, sizeof(TCacheHeader), 1, fCacheFile);
		switch (Header->Type)
		{
			case eSteamCacheFileTypeGCF:
				Block = new CCacheBlock(this);
				Block->Read(fCacheFile);
				AllocationTable = new CCacheAllocationTable(this);
				AllocationTable->Read(fCacheFile);
				Manifest = new CCacheManifest(this);
				Manifest->Read(fCacheFile);
				ChecksumTable = new CCacheChecksumTable(this);
				ChecksumTable->Read(fCacheFile);
				Sectors = new CCacheSectors(this);
				Sectors->ReadHeader(fCacheFile);
				break;
			case eSteamCacheFileTypeNCF:
				Manifest = new CCacheManifest(this);
				Manifest->Read(fCacheFile);
				ChecksumTable = new CCacheChecksumTable(this);
				ChecksumTable->Read(fCacheFile);
				/*Sectors = new CCacheSectors(this);
				Sectors->ReadHeader(fCacheFile);*/
				break;
		}
		fseek(fCacheFile, Sectors->Header->FirstSectorOffset, SEEK_SET);

		DirectoryTable = new TManifestEntriesInCache[Manifest->Header->ItemCount];
		memset(DirectoryTable, 0, sizeof(TManifestEntriesInCache) * Manifest->Header->ItemCount);

		DirectoryTable[0].pCache = this;
		//if (bLogging) Logger->Write("\tTHIS = 0x%08X\n", (unsigned int)this);
		DirectoryTable[0].FullName = "\\";
		DirectoryTable[0].Size = Manifest->Entries[0].ItemSize;
		DirectoryTable[0].FirstIndex = Manifest->Entries[0].FirstIndex;
		DirectoryTable[0].NextIndex = Manifest->Entries[0].NextIndex;
		DirectoryTable[0].ParentIndex = Manifest->Entries[0].ParentIndex;
		BuildDirectoryTable(0, "", ExtraMountPath);
		return true;
	}

private:
	
	void BuildDirectoryTable(unsigned int ManifestEntryIndex, const char* PathName, const char* ExtraMountPath)
	{

		char ExtraMount[MAX_PATH];
		int ExtraMountLength = 0;

		if (strlen(ExtraMountPath) >0)
		{
			strcpy(ExtraMount,"\\");
			strcat(ExtraMount, ExtraMountPath);
			ExtraMountLength = strlen(ExtraMountPath) + 1;

			if (strstr(PathName, ExtraMountPath) == PathName+1)
				strcpy(ExtraMount, "");

		}

		for (int i = Manifest->Entries[ManifestEntryIndex].FirstIndex; i != 0; i = Manifest->Entries[i].NextIndex)
		{

			if(Manifest->Entries[i].Type == 0)
			{
				DirectoryTable[i].pCache = this;
				char* entryname = Manifest->FileNames + Manifest->Entries[i].NameOffset;
				int length = strlen(entryname) + 1;
				int pathlength = strlen(PathName) + length + ExtraMountLength + 1;
				char* entrypath = new char[pathlength];
				if (ExtraMountLength > 0)
				{
					_snprintf(entrypath, pathlength,"%s%s\\%s", ExtraMount, PathName, entryname);
				}
				else
				{
					_snprintf(entrypath, pathlength,"%s\\%s", PathName, entryname);
				}
				DirectoryTable[i].Name = new char[length];
				strcpy(DirectoryTable[i].Name, _strlwr(entryname));
				DirectoryTable[i].FullName = _strlwr(entrypath);
				DirectoryTable[i].Size = Manifest->Entries[i].ItemSize;
				DirectoryTable[i].FirstIndex = Manifest->Entries[i].FirstIndex;
				DirectoryTable[i].NextIndex = Manifest->Entries[i].NextIndex;
				DirectoryTable[i].ParentIndex = Manifest->Entries[i].ParentIndex;
				BuildDirectoryTable(i, entrypath, ExtraMountPath);
			}
			else
			{
				DirectoryTable[i].pCache = this;
				char* entryname = Manifest->FileNames + Manifest->Entries[i].NameOffset;
				int length = strlen(entryname) + 1;
				int pathlength = strlen(PathName) + length + ExtraMountLength + 1;
				char* entrypath = new char[pathlength];
				if (ExtraMountLength > 0)
				{
					_snprintf(entrypath, pathlength,"%s%s\\%s", ExtraMount, PathName, entryname);
				}
				else
				{
					_snprintf(entrypath, pathlength,"%s\\%s", PathName, entryname);
				}
				DirectoryTable[i].Name = new char[length];
				strcpy(DirectoryTable[i].Name, _strlwr(entryname));
				DirectoryTable[i].FullName = _strlwr(entrypath);
				DirectoryTable[i].Size = Manifest->Entries[i].ItemSize;
				DirectoryTable[i].Type = Manifest->Entries[i].Type;
				DirectoryTable[i].FirstIndex = Manifest->Entries[i].FirstIndex;
				DirectoryTable[i].NextIndex = Manifest->Entries[i].NextIndex;
				DirectoryTable[i].ParentIndex = Manifest->Entries[i].ParentIndex;
				BuildFileInfo(i);
			}

			GlobalDirectoryTableSize++;
			//if (bLogging && bLogFS) Logger->Write("Add to directory table: %s\n", DirectoryTable[i].FullName);

		}

	}

	void BuildFileInfo(unsigned int ManifestEntryIndex)
	{
		unsigned int minusone = 0xFFFF;
		unsigned int numofsectors = 0;
		unsigned int sectorcount = Sectors->Header->SectorCount;
		if(sectorcount >= 0xFFFF)
		{
			minusone = 0xFFFFFFFF;
		}
		unsigned int blockindex = Manifest->MapEntries[ManifestEntryIndex].FirstBlockIndex;
		DirectoryTable[ManifestEntryIndex].Flags = Block->Entries[blockindex].EntryType;
		numofsectors = Manifest->Entries[ManifestEntryIndex].ItemSize / Sectors->Header->PhysicalSectorSize;
		if ((Manifest->Entries[ManifestEntryIndex].ItemSize % Sectors->Header->PhysicalSectorSize) != 0)
		{
			numofsectors++;
		}
		DirectoryTable[ManifestEntryIndex].numofsectors = numofsectors;

		if (numofsectors != 0)
		{
			DirectoryTable[ManifestEntryIndex].Sectors = new unsigned int[numofsectors];
			unsigned int sectorindexptr = 0;
			while (blockindex != sectorcount)
			{
				if (DirectoryTable[ManifestEntryIndex].IsFragmented == false && Block->Entries[blockindex].NextBlockEntryIndex != sectorcount)
				{
					DirectoryTable[ManifestEntryIndex].IsFragmented = true;
				}
				unsigned int sectorindex = Block->Entries[blockindex].FirstDataBlockIndex;
				while (sectorindex != minusone)
				{
					if (DirectoryTable[ManifestEntryIndex].IsFragmented == false && AllocationTable->Entries[sectorindex].NextDataBlockIndex != minusone &&
					    (AllocationTable->Entries[sectorindex].NextDataBlockIndex - sectorindex) != 1)
					{
						DirectoryTable[ManifestEntryIndex].IsFragmented = true;
					}
					DirectoryTable[ManifestEntryIndex].Sectors[sectorindexptr] = sectorindex;
					sectorindexptr++;
					sectorindex = AllocationTable->Entries[sectorindex].NextDataBlockIndex;
				}
				blockindex = Block->Entries[blockindex].NextBlockEntryIndex;
			}
		}

		DirectoryTable[ManifestEntryIndex].IsLocalCopyMade = IsLocalCopyMade(ManifestEntryIndex);
		DirectoryTable[ManifestEntryIndex].IsLocalCopyHasPriorityOverChache = IsLocalCopyHasPriorityOverChache(ManifestEntryIndex);
	}

	bool IsLocalCopyMade(unsigned int ManifestEntryIndex) const
	{
		for(unsigned int i = 0; i < Manifest->Header->CopyCount;i++)
		{
			if(Manifest->CopyEntries[i].DirectoryIndex == ManifestEntryIndex)
			{
				return true;
			}
		}
		return false;
	}

	bool IsLocalCopyHasPriorityOverChache(unsigned int ManifestEntryIndex) const
	{
		for(unsigned int i = 0; i < Manifest->Header->LocalCount;i++)
		{
			if(Manifest->LocalEntries[i].DirectoryIndex == ManifestEntryIndex)
			{
				return true;
			}
		}
		return false;
	}
};
