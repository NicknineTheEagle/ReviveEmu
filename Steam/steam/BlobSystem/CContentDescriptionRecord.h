#include "ContentDescriptionRecord\CAppRecord.h"
#include "ContentDescriptionRecord\CSubscriptionRecord.h"
#include "zlib/zlib.h"

#pragma once

typedef enum
{
	eCRDVersionNumber,
	eCRDApplicationsRecord,
	eCRDSubscriptionsRecord,
	eCRDLastChangedExistingAppOrSubscriptionTime,
	eCRDIndexAppIdToSubscriptionIdsRecord,
	eCRDAllAppsPublicKeysRecord,
	eCRDAllAppsEncryptedPrivateKeysRecord
}ECDRType;

class CContentDescriptionRecord
{

public:

	unsigned short VersionNumber;
	std::vector<CAppRecord*> ApplicationRecords;
	std::vector<CSubscriptionRecord*> SubscriptionsRecord;
	std::map<unsigned int, char*> AllAppsPublicKeysRecord;

	CContentDescriptionRecord(const char* CDRBinary)
	{
		const char* CDRData;
		const char* NodeEnd; 
		Bytef* zBuffer = NULL;

		TNodeHeader *NodeHeader = (TNodeHeader*)CDRBinary;

		if(NodeHeader->magic != NodeMagicNum)
		{
			TNodeHeaderCompressed *NodeHeaderCompressed = (TNodeHeaderCompressed*)CDRBinary;

			if(NodeHeaderCompressed->magic == NodeMagicNumCompressed)
			{
				WORD header=*(WORD*)CDRBinary;
				DWORD compressedSize=*(DWORD*)(CDRBinary+0x02);
				DWORD uncompressedSize=*(DWORD*)(CDRBinary+0x0A);
				WORD unknown2=*(WORD*)(CDRBinary+0x12);
				Bytef* zData=(Bytef*)(CDRBinary+0x14);
				zBuffer=new Bytef[uncompressedSize];
   
				int result=uncompress(zBuffer,&uncompressedSize,zData,compressedSize);
			
				CDRData = (char*)zBuffer;
				NodeEnd = CDRData + NodeHeaderCompressed->datalength;
			}
			else
			{
				MessageBoxA(NULL, "Encountered invalid CDR blob!", "REVive - Bad CDR", 0);
				ExitProcess(0xffffffff);
			}
		}
		else
		{
			CDRData = CDRBinary;
			NodeEnd = CDRData + NodeHeader->datalength;
		}
			
		CDRData += sizeof(TNodeHeader);
		while(CDRData < NodeEnd)
		{
			TDescriptorNode *DNode = (TDescriptorNode*)CDRData;
			CDRData += sizeof(TDescriptorNode);

			switch (DNode->type)
			{
				case eCRDVersionNumber:
					{
						VersionNumber = *(unsigned short*)CDRData;
						CDRData += DNode->datalength;
						break;
					}

				case eCRDApplicationsRecord:
					{
						TNodeHeader *AppNodeHeader = (TNodeHeader*)CDRData;
						if(AppNodeHeader->magic == NodeMagicNum)
						{
							const char* AppRBinary = CDRData;
							const char* AppNodeEnd = AppRBinary + AppNodeHeader->datalength;
							AppRBinary += sizeof(TNodeHeader);

							while(AppRBinary < AppNodeEnd)
							{
								AppRBinary += sizeof(TDescriptorNode);
								CAppRecord* newApp = new CAppRecord();
								AppRBinary = newApp->Enumerate(AppRBinary);
								ApplicationRecords.push_back(newApp);
							}
						}
						CDRData += DNode->datalength;
						break;
					}

				case eCRDSubscriptionsRecord :
					{
						TNodeHeader *SubNodeHeader = (TNodeHeader*)CDRData;
						if(SubNodeHeader->magic == NodeMagicNum)
						{
							const char* SubRBinary = CDRData;
							const char* SubNodeEnd = SubRBinary + SubNodeHeader->datalength;
							SubRBinary += sizeof(TNodeHeader);

							while(SubRBinary < SubNodeEnd)
							{
								SubRBinary += sizeof(TDescriptorNode);
								CSubscriptionRecord* newSub = new CSubscriptionRecord();
								SubRBinary = newSub->Enumerate(SubRBinary);
								SubscriptionsRecord.push_back(newSub);
							}
						}
						CDRData += DNode->datalength;
						break;
					}

				case eCRDLastChangedExistingAppOrSubscriptionTime:
					CDRData += DNode->datalength;
					break;

				case eCRDIndexAppIdToSubscriptionIdsRecord:
					CDRData += DNode->datalength;
					break;

				case eCRDAllAppsPublicKeysRecord:
					{
						TNodeHeader *APKRNodeHeader = (TNodeHeader*)CDRData;
						if(APKRNodeHeader->magic == NodeMagicNum)
						{
							const char* APKRBinary = CDRData;
							const char* APKRNodeEnd = APKRBinary + APKRNodeHeader->datalength;
							APKRBinary += sizeof(TNodeHeader);

							while(APKRBinary < APKRNodeEnd)
							{
								TDescriptorNode *APKRDNode = (TDescriptorNode*)APKRBinary;
								APKRBinary += sizeof(TDescriptorNode);
								unsigned int uAppId = APKRDNode->type;
								char* szKeyData = new char[APKRDNode->datalength];
								memcpy(szKeyData, APKRBinary, APKRDNode->datalength);
								APKRBinary += APKRDNode->datalength;
								AllAppsPublicKeysRecord[uAppId] = szKeyData;
							}
						}
						CDRData += DNode->datalength;
						break;
					}

				case eCRDAllAppsEncryptedPrivateKeysRecord:
					CDRData += DNode->datalength;
					break;

				default:
					CDRData += DNode->datalength;
					break;
			}
		}

		if (zBuffer)
			delete[] zBuffer;
	}

	~CContentDescriptionRecord()
	{
		for (size_t i = 0; i < ApplicationRecords.size(); i++)
			delete ApplicationRecords[i];

		for (size_t i = 0; i < SubscriptionsRecord.size(); i++)
			delete SubscriptionsRecord[i];

		for (auto &record : AllAppsPublicKeysRecord)
			delete[] record.second;
	}

	static CContentDescriptionRecord* LoadFromFile(const char* FileName)
	{
		FILE* File = fopen(FileName, "rb");
		if (!File)
			return NULL;

		fseek(File, 0, SEEK_END);
		size_t FileLen = ftell(File);
		fseek(File, 0, SEEK_SET);

		// Last known CDR is 3 812 255 bytes.
		if (FileLen > 8 * 1024 * 1024)
		{
			fclose(File);
			return NULL;
		}

		char* FileData = new char[FileLen];
		fread(FileData, 1, FileLen, File);
		fclose(File);
		CContentDescriptionRecord* newCDR = new CContentDescriptionRecord(FileData);
		delete[] FileData;
		return newCDR;
	}
};
