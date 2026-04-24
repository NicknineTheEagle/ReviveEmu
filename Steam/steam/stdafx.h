// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(_WIN32)
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <climits>
#include <vector>
#include <map>
#include <mutex>
#include <algorithm>

#if defined(_WIN32)
#include <io.h>
#include <direct.h>
#include <shellapi.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
#include <ctime>
#include <dlfcn.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <fnmatch.h>
#if defined(_LINUX)
#include <sys/syscall.h>
#endif
#endif

#include "SteamCommon.h"
#include "RevCommon.h"
#define INCLUDED_STEAM2_USERID_STRUCTS
#include "Steam3ID.h"
#include "strtools.h"
#define SI_NO_CONVERSION
#include "simpleini/SimpleIni.h"
#include "logFile.h"

#if defined(_WIN32)
#include "registry.h"
#endif
