#pragma once

void initStringArray(char ** array, int size);
void clearStringArray(char ** array, int size);
int getRegistry(const char * key, const char * name, DWORD * value);
int getRegistry(const char * key, const char * name, char * value, DWORD maxLength);
int getRegistryU(const char * key, const char * name, char * value, DWORD maxLength);
int getRegistry(const char * key, const char * name, char ** values, int * nbValues);
void setRegistry(const char * key, const char * name, DWORD value);
void setRegistry(const char * key, const char * name, const char * value);
void setRegistry(const char * key, const char * name, const char ** values, int nbValues);
