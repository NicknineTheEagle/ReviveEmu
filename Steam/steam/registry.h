#pragma once

void initStringArray(char ** array, int size);
void clearStringArray(char ** array, int size);
int getRegistry(char * key, char * name, DWORD * value);
int getRegistry(char * key, char * name, char * value, DWORD maxLength);
int getRegistryU(char * key, char * name, char * value, DWORD maxLength);
int getRegistry(char * key, char * name, char ** values,int * nbValues);
void setRegistry(char * key, char * name, DWORD value);
void setRegistry(char * key, char * name, char * value);
void setRegistry(char * key, char * name, char ** values,int nbValues);
