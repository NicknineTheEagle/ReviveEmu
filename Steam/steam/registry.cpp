#include "stdafx.h"
#include "registry.h"

void clearStringArray(char ** array, int size)
{
	for (int ind=0;ind<size;ind++)
	{
		if (array[ind])
		{
			free(array[ind]);
			array[ind]=0;
		}
	}
}

void initStringArray(char ** array, int size)
{
	for (int ind=0;ind<size;ind++)
	{
		array[ind]=0;
	}
}

int getRegistry(char * key, char * name, DWORD * value)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=sizeof(DWORD);
	DWORD type=0;
	int result=RegQueryValueExA(root, name, NULL,&type, (PBYTE)value, &dwLength);
	if (result == ERROR_SUCCESS && type!=REG_DWORD) result=1;
		
	RegCloseKey(root);
	return result;
}

int getRegistry(char * key, char * name, char * value, DWORD maxLength)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0, KEY_QUERY_VALUE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=maxLength;
	DWORD type=0;
	int result=RegQueryValueExA(root, name, NULL,&type, (PBYTE)value, &dwLength);
	if (result == ERROR_SUCCESS && type!=REG_SZ) result=1;

	_strupr(value);	

	RegCloseKey(root);
	return result;
}

int getRegistryU(char * key, char * name, char * value, DWORD maxLength)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_CURRENT_USER, key, 0, KEY_QUERY_VALUE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=maxLength;
	DWORD type=0;
	int result=RegQueryValueExA(root, name, NULL,&type, (PBYTE)value, &dwLength);
	if (result == ERROR_SUCCESS && type!=REG_SZ) result=1;

	_strupr(value);	

	RegCloseKey(root);
	return result;
}

int getRegistry(char * key, char * name, char ** values,int * nbValues)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0,	KEY_QUERY_VALUE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_QUERY_VALUE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=65535;
	DWORD type=0;
	char * vals=(char *)malloc(65535);
	
	int result=RegQueryValueExA(root, name, NULL,&type, (PBYTE)vals, &dwLength);
	if (result == ERROR_SUCCESS && type!=REG_MULTI_SZ) result=1;
	
	if (result == ERROR_SUCCESS)
	{
		char * temp=vals;
		
		while (strlen(temp))
		{
			values[*nbValues]=(char*)malloc((strlen(temp)+1)*sizeof(char));
			strcpy(values[*nbValues],temp);
			_strupr(values[*nbValues]);
			temp+=strlen(temp)+1;
			(*nbValues)++;
		}
	}
	free(vals);
	
	RegCloseKey(root);
	return result;
}

void setRegistry(char * key, char * name, DWORD value)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_CURRENT_USER, key, 0,	KEY_WRITE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=sizeof(DWORD);
	RegSetValueExA(root, name, NULL, REG_DWORD, (PBYTE)&value, dwLength);
	
	RegCloseKey(root);
}

void setRegistry(char * key, char * name, char * value)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_CURRENT_USER, key, 0,	KEY_WRITE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_CURRENT_USER, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=sizeof(DWORD);
	dwLength=strlen(value)+1;
	RegSetValueExA(root, name, NULL, REG_SZ, (PBYTE)value, dwLength);
	
	RegCloseKey(root);
}

void setRegistry(char * key, char * name, char ** values,int nbValues)
{
	HKEY root;
	DWORD dwDisp;
	
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, key, 0,	KEY_WRITE, &root) != ERROR_SUCCESS)
	{
		RegCreateKeyExA(HKEY_LOCAL_MACHINE, key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &root, &dwDisp);
	}
	
	DWORD dwLength=1;
	for (int ind = 0; ind < nbValues; ind++)
	{
		dwLength+=strlen(values[ind])+1;
	}
	
	char * vals=(char *)malloc(dwLength);
	
	char * temp=vals;
	for (int ind = 0; ind < nbValues; ind++)
	{
		strcpy(temp,values[ind]);
		temp+=strlen(temp)+1;
	}
	strcpy(temp,"");
	
	RegSetValueExA(root, name, NULL, REG_MULTI_SZ, (PBYTE)vals, dwLength);
	
	free(vals);
	
	RegCloseKey(root);
}
