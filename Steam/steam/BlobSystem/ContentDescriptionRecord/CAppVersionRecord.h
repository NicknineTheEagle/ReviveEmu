#pragma once

typedef enum
{
	eVRDescription = 1,
	eVRVersionId = 2,
	eVRIsNotAvailable = 3,
	eVRLaunchOptionIdsRecord = 4,
	eVRDepotEncryptionKey = 5,
	eVRIsEncryptionKeyAvailable = 6,
	eVRIsRebased = 7,
	eVRIsLongVersionRoll = 8
}EVersionRecordFields;

class CAppVersionRecord
{
public:

	char* Description;
	unsigned int VersionId;
	bool IsNotAvailable;
	std::vector<unsigned int> LaunchOptionIdsRecord;
	char* DepotEncryptionKey;
	bool IsEncryptionKeyAvailable;
	bool IsRebased;
	bool IsLongVersionRoll;

	CAppVersionRecord()
	{
		Description = NULL;
		VersionId = 0;
		IsNotAvailable = false;
		DepotEncryptionKey = NULL;
		IsEncryptionKeyAvailable = false;
		IsRebased = false;
		IsLongVersionRoll = false;
	}

	~CAppVersionRecord()
	{
		if (Description)
			delete[] Description;

		if (DepotEncryptionKey)
			delete[] DepotEncryptionKey;
	}

	const char* Enumerate(const char* VRBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)VRBinary;
		if(NodeHeader->magic != NodeMagicNum)
		{
			return NULL;
		}
		else
		{
			const char* NodeEnd = VRBinary + NodeHeader->datalength;
			VRBinary += sizeof(TNodeHeader);

			while(VRBinary < NodeEnd)
			{
				TDescriptorNode *DNode = (TDescriptorNode*)VRBinary;
				VRBinary += sizeof(TDescriptorNode);
				switch (DNode->type)
				{

					case eVRDescription:
						this->Description = new char[DNode->datalength];
						memcpy(this->Description, VRBinary, DNode->datalength);
						VRBinary += DNode->datalength;
						break;

					case eVRVersionId:
						this->VersionId = *(unsigned int*)VRBinary;
						VRBinary += DNode->datalength;
						break;

					case eVRIsNotAvailable:
						this->IsNotAvailable = *(bool*)VRBinary;
						VRBinary += DNode->datalength;
						break;

					case eVRLaunchOptionIdsRecord:
						{
							TNodeHeader *LOIRNodeHeader = (TNodeHeader*)VRBinary;

							if(LOIRNodeHeader->magic == NodeMagicNum)
							{
								const char* LOIRBinary = VRBinary;
								const char* LOIRNodeEnd = LOIRBinary + LOIRNodeHeader->datalength;
								LOIRBinary += sizeof(TNodeHeader);

								while(LOIRBinary < LOIRNodeEnd)
								{
									LOIRBinary += sizeof(TDescriptorNode);
								    LaunchOptionIdsRecord.push_back(*(unsigned int*)LOIRBinary);
								}
							}

							VRBinary += DNode->datalength;
							break;
						}

					case eVRDepotEncryptionKey:
						this->DepotEncryptionKey = new char[DNode->datalength];
						memcpy(this->DepotEncryptionKey, VRBinary, DNode->datalength);
						VRBinary += DNode->datalength;
						break;

					case eVRIsEncryptionKeyAvailable:
						this->IsEncryptionKeyAvailable = *(bool*)VRBinary;
						VRBinary += DNode->datalength;
						break;

					case eVRIsRebased:
						this->IsRebased = *(bool*)VRBinary;
						VRBinary += DNode->datalength;
						break;

					case eVRIsLongVersionRoll:
						this->IsLongVersionRoll = *(bool*)VRBinary;
						VRBinary += DNode->datalength;
						break;
				}
			}

			return (VRBinary + NodeHeader->nullpadding);
		}
	}
};
