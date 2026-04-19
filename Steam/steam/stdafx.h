// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(_WIN32)
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>
#elif defined(__linux__)
#define LINUX 1
#define _LINUX 1
#define POSIX 1
#define GNUC 1
#endif

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <vector>
#include <map>
#include <mutex>
#include <algorithm>

#if defined(_WIN32)
#include <io.h>
#include <direct.h>
#else
#include <dlfcn.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/limits.h>
#endif

#include "SteamCommon.h"
#include "RevCommon.h"
#define INCLUDED_STEAM2_USERID_STRUCTS
#include "Steam3ID.h"
#include "strtools.h"
#define SI_NO_CONVERSION
#include "simpleini/SimpleIni.h"
#include "logFile.h"
