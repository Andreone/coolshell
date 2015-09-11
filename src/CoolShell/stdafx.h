// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// This Win32++ file must be included before windows.h (required by Win32++ to set some important macros)
#include "Win32xx\wxx_wincore.h"

// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>

// STL header files
#include <algorithm>
#include <functional>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Useful debugging macros
#include "CoolShellLib\debug.h"
