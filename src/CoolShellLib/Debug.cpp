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

#include "stdafx.h"
#include "Debug.h"

#ifndef MAX_TRACE_LEN
#define MAX_TRACE_LEN 4096
#endif

#ifdef _DEBUG

void _traceA(LPCSTR format, ...)
{
    CHAR text[MAX_TRACE_LEN];
    va_list args;
    va_start(args, format);
    _vsnprintf_s(text, MAX_TRACE_LEN, _TRUNCATE, format, args);

    ::OutputDebugStringA(text);
    std::cout << text;
}


void _traceW(LPCWSTR format, ...)
{
    WCHAR text[MAX_TRACE_LEN];
    va_list args;
    va_start(args, format);
    _vsnwprintf_s(text, MAX_TRACE_LEN, _TRUNCATE, format, args);

    ::OutputDebugStringW(text);
    std::wcout << text;
}

#endif // _DEBUG
