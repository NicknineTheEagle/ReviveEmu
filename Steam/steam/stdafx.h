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
#include <vector>
#include <map>

#include <io.h>
#include <direct.h>

#include "SteamCommon.h"
#include "RevCommon.h"
#define INCLUDED_STEAM2_USERID_STRUCTS
#include "Steam3ID.h"
#include "iniFile.h"
#include "logFile.h"
