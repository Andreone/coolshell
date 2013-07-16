//  This file is part of CoolShell
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/.

#pragma once

#include <cassert>

/**
 * \name Debugging macros
 *
 * The debugging macros are:
 *  - TRACE
 *      Sends text to the debug output window a la printf
 *      Example:
 *      TRACE("state=%d\n", state);
 *  - STRACE
 *      Sends text to the debug output window a la std stream
 *      Example:
 *      STRACE("state=" << state);
 *  - INFO
 *      Same as TRACE, but prepends the output with the file name and the line number.
 *      Example:
 *      INFO("state=%d", state);
 *  - ASSERT
 *  - VERIFY
 *  - BREAK
 *      Forces a program to break for debug builds.
 *      Example:
 *      BREAK();
 *  - BREAK_IF
 *      Same as BREAK, but only if the condition is true.
 *      Example:
 *      BREAK_IF(i == 10);
 *  - DEBUG_ONLY
 *      Invokes code fragment for debug builds only.
 *      Example:
 *      DEBUG_ONLY(::MessageBox(NULL, "hello", "debug msg", MB_OK);
 */

/**
 * TRACE, ASSERT, VERIFY, DEBUG_ONLY macros
 */

// take care of MFCs because it already defines TRACE, ASSERT & VERIFY
#ifndef _MFC_VER

#ifdef _DEBUG

    #if _UNICODE
        void _traceW(LPCWSTR format, ...);
        #define _trace _traceW
    #else
        void _traceA(LPCSTR format, ...);
        #define _trace _traceA
    #endif

    #define TRACE           _trace
    #define ASSERT(x)       assert(x);
    #define VERIFY(f)       ASSERT(f)
    #define DEBUG_ONLY(f)   (f)
#else // !_DEBUG

    #if _UNICODE
        inline void _traceW(TCHAR*, ...) { }
        #define _trace _traceW
    #else
        inline void _traceA(TCHAR*, ...) { }
        #define _trace _traceA
    #endif

    #define TRACE           1 ? ((void)0) : _trace
    #define ASSERT(x)       ((void)0)
    #define VERIFY(f)       ((void)(f))
    #define DEBUG_ONLY(f)   ((void)0)
#endif // _DEBUG

#endif // _MFC_VER

/**
 * STRACE macro
 */
#ifdef _DEBUG

    void _trace_stream(const std::ostringstream&);
    void _trace_stream(const std::wostringstream&);

    #if _UNICODE
        #define STRACE(x) { std::wostringstream os; os << x << std::endl; _trace_wstream(os); }
    #else
        #define STRACE(x) { std::ostringstream os; os << x << std::endl; _trace_stream(os); }
    #endif

#else // !_DEBUG

    inline void _trace_stream(const std::ostringstream&) { }
    inline void _trace_wstream(const std::wostringstream&) { }

    #define STRACE(x) 

#endif // _DEBUG

/**
 * BREAK, BREAK_IF, INFO
 */
#if _DEBUG
    #define BREAK()     ::DebugBreak()
    #define BREAK_IF(x) { if(x) BREAK(); }
    #define INFO(x)     TRACE(_T("%s (%s)"), __FILE__, __LINE__); TRACE(x);
#else // !_DEBUG

    #define BREAK()     ((void)0)
    #define BREAK_IF(x) ((void)0)
    #define INFO(x)     TRACE(x);
#endif // _DEBUG
