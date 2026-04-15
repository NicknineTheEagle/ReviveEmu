// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <windows.h>

#include <cstdio>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <vector>
#include <map>
#include <mutex>
#include <algorithm>

#include <io.h>
#include <direct.h>

#include "SteamCommon.h"
#include "RevCommon.h"
#define INCLUDED_STEAM2_USERID_STRUCTS
#include "Steam3ID.h"
#include "strtools.h"
#include "IniFile.h"
#include "logFile.h"
