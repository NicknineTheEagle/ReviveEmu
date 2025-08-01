#pragma once

class CBlobNode
{
public:
	char *Name;
	ESteamBlobNodeType Type;
	std::vector<CBlobNode*> Nodes;
	TNodeValue *KeyValue;

	CBlobNode()
	{
		Name = NULL;
		KeyValue = NULL;
	}

	~CBlobNode()
	{
		if (Name)
			delete[] Name;

		for (size_t i = 0; i < Nodes.size(); i++)
			delete Nodes[i];

		if (KeyValue)
		{
			delete[] KeyValue->Value;
			delete KeyValue;
		}
	}

	void Populate(const char *NodeBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)NodeBinary;
		if(NodeHeader->magic == NodeMagicNum)
		{
			NodeBinary += sizeof(TNodeHeader);
			NodeBinary = this->KeyEntry(NodeBinary);
		}
	}

private:
	const char* SubKeys(const char *NodeBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)NodeBinary;
		if(NodeHeader->magic != NodeMagicNum)
		{
			return NULL;
		}
		else
		{
			NodeBinary += sizeof(TNodeHeader);
			for(int i=0;i<2;i++)
			{
				TDescriptorNode *DNode = (TDescriptorNode*)NodeBinary;
				NodeBinary += sizeof(TDescriptorNode);
				switch (DNode->type)
				{
					case Key:
						NodeBinary = KeyEntries(NodeBinary);
						break;
					case Value:
						NodeBinary = ValueEntries(NodeBinary);
						break;
				}
			}
		}
		return (NodeBinary + NodeHeader->nullpadding);
	}

	const char* KeyEntry(const char *NodeBinary)
	{
		this->Type = Key;
		TNode *Node = (TNode*)NodeBinary;
		short DescriptorLength = Node->descriptorlength;
		this->Name = new char[DescriptorLength+1];
		memset(this->Name, 0, DescriptorLength+1);
		NodeBinary += sizeof(TNode);
		memcpy(this->Name, NodeBinary, DescriptorLength);
		NodeBinary += DescriptorLength;
		NodeBinary = SubKeys(NodeBinary);
		return NodeBinary;
	}

	const char* KeyEntries(const char *NodeBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)NodeBinary;
		if(NodeHeader->magic != NodeMagicNum)
		{
			return NULL;
		}
		else if(NodeHeader->datalength == sizeof(TNodeHeader))
		{
			NodeBinary += sizeof(TNodeHeader);
		}
		else
		{
			const char* NodeEnd = NodeBinary + NodeHeader->datalength;
			NodeBinary += sizeof(TNodeHeader);
			while(NodeBinary != NULL && NodeBinary < NodeEnd)
			{
				CBlobNode *newBlobNode = new CBlobNode();
				this->Nodes.push_back(newBlobNode);
				NodeBinary = newBlobNode->KeyEntry(NodeBinary);
			}
		}
		return (NodeBinary + NodeHeader->nullpadding);
	}

	const char* ValueProperties(const char *NodeBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)NodeBinary;
		if(NodeHeader->magic != NodeMagicNum)
		{
			return NULL;
		}
		else
		{
			this->KeyValue = new TNodeValue();
			NodeBinary += sizeof(TNodeHeader);
			for(int i=0;i<2;i++)
			{
				TDescriptorNode *DNode = (TDescriptorNode*)NodeBinary;
				NodeBinary += sizeof(TDescriptorNode);
				switch (DNode->type)
				{
					case 1:
						{
							this->KeyValue->Type = *(ESteamBlobValueType*)NodeBinary;
							NodeBinary += sizeof(int);
							break;
						}
					case 2:
						{
							this->KeyValue->Value = new char[DNode->datalength];
							this->KeyValue->ValueSize = DNode->datalength;
							memcpy(this->KeyValue->Value, NodeBinary, DNode->datalength);
							NodeBinary += DNode->datalength;
							break;
						}
				}
			}
		}
		return (NodeBinary + NodeHeader->nullpadding);
	}

	const char* ValueEntry(const char *NodeBinary)
	{
		this->Type = Value;
		TNode *Node = (TNode*)NodeBinary;
		short DescriptorLength = Node->descriptorlength;
		this->Name = new char[DescriptorLength+1];
		memset(this->Name, 0, DescriptorLength+1);
		NodeBinary += sizeof(TNode);
		memcpy(this->Name, NodeBinary, DescriptorLength);
		NodeBinary += DescriptorLength;
		NodeBinary = ValueProperties(NodeBinary);
		return NodeBinary;
	}

	const char* ValueEntries(const char *NodeBinary)
	{
		TNodeHeader *NodeHeader = (TNodeHeader*)NodeBinary;
		if(NodeHeader->magic != NodeMagicNum)
		{
			return NULL;
		}
		else if(NodeHeader->datalength == sizeof(TNodeHeader))
		{
			NodeBinary += sizeof(TNodeHeader);
		}
		else
		{
			const char* NodeEnd = NodeBinary + NodeHeader->datalength;
			NodeBinary += sizeof(TNodeHeader);
			while(NodeBinary != NULL && NodeBinary < NodeEnd)
			{
				CBlobNode *newBlobNode = new CBlobNode();
				this->Nodes.push_back(newBlobNode);
				NodeBinary = newBlobNode->ValueEntry(NodeBinary);
			}
		}
		return (NodeBinary + NodeHeader->nullpadding);
	}
};
