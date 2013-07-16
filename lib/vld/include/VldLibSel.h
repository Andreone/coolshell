#pragma once

/*****************************************************
Selective library loader for VisualLeakDetector libraries.

Library path:
Before including this file, you may specify the library root directory as following.
#define __VLD_LIBSEL_ROOT	"..\\vld"
Note: __VLD_LIBSEL_ROOT is undefined at the end of the file.

USE_VLD must be defined to include VLD

******************************************************/

//////////////////////////////////////////////////////////////////////////
// configure
#ifndef __VLD_LIBSEL_ROOT
	#error "You forgot to define __VLD_LIBSEL_ROOT before including " __FILE__
#endif

#if _WIN32
	#define __VLD_PLATFORM_STR "Win32"
#else
	#define __VLD_PLATFORM_STR "Win64"
#endif

#define __VLD_LIB_PATH	__VLD_LIBSEL_ROOT "\\lib\\" __VLD_PLATFORM_STR "\\"

//////////////////////////////////////////////////////////////////////////
// set libraries
#pragma message( "VisualLeakDetector Library " __VLD_LIB_PATH "vld.lib loaded" )
#pragma comment( lib, __VLD_LIB_PATH "vld.lib" )

//////////////////////////////////////////////////////////////////////////
// cleanup symbols
#undef __VLD_LIBSEL_ROOT
#undef __VLD_PLATFORM_STR
#undef __VLD_LIB_PATH
