// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// This Win32++ file must be included before windows.h (required by Win32++ to set some important macros)
#include "Win32xx\wincore.h"

// Windows Header Files
#include <Windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// STL header files
#include <iostream>

#include <boost/test/unit_test.hpp>