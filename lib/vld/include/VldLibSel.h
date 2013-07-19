#pragma once

/*****************************************************
Selective library loader for VisualLeakDetector libraries.

Library path:
Before including this file, you may specify the library root directory as following.
#define VLD_LIBSEL_ROOT	"..\\vld"
Note: VLD_LIBSEL_ROOT is undefined at the end of the file.

USE_VLD must be defined to include VLD

******************************************************/

//////////////////////////////////////////////////////////////////////////
// configure
#ifndef VLD_LIBSEL_ROOT
	#error "You forgot to define VLD_LIBSEL_ROOT before including " __FILE__
#endif

#if _WIN64
	#define VLD_PLATFORM_STR "Win64"
#else
	#define VLD_PLATFORM_STR "Win32"
#endif

#define VLD_LIB_PATH	VLD_LIBSEL_ROOT "\\lib\\" VLD_PLATFORM_STR "\\"

//////////////////////////////////////////////////////////////////////////
// set libraries
#pragma message( "VisualLeakDetector Library " VLD_LIB_PATH "vld.lib loaded" )
#pragma comment( lib, VLD_LIB_PATH "vld.lib" )

//////////////////////////////////////////////////////////////////////////
// cleanup symbols
#undef VLD_LIBSEL_ROOT
#undef VLD_PLATFORM_STR
#undef VLD_LIB_PATH
