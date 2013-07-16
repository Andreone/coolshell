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

#include "StdAfx.h"
#include "Logging.h"

#include <sys\timeb.h>
#include <time.h>

volatile Logging::LogLevel Logging::s_logLevel = None;

#define FORMAT_AND_WRITE \
    va_list args; \
    va_start(args, lpszFormat); \
    CString line; \
    GetFormatedLine(lpszFormat, args, line); \
    WriteLine(line);


void Logging::SetLogLevel(LogLevel level)
{
    s_logLevel = level;
}

void Logging::GetFormatedLine(LPCTSTR lpszFormat, va_list& args, CString& line)
{
    CString msg;
    msg.FormatV(lpszFormat, args);

    struct __timeb64 time;
    _ftime64_s(&time);

    struct tm tmTime;
    _localtime64_s(&tmTime, &time.time);

    line.Format(_T("%02d:%02d:%02d.%03u 0x%04X %s"),
        tmTime.tm_hour,
        tmTime.tm_min,
        tmTime.tm_sec,
        time.millitm,
        ::GetCurrentThreadId(),
        (LPCTSTR)msg);
}

void Logging::WriteLine(const CString& line)
{
    ::OutputDebugString((LPCTSTR)line);
    ::OutputDebugString(_T("\n"));

    std::cout << (LPCTSTR)line << std::endl;
}

void Logging::LogFatal(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Fatal)
        return;
    FORMAT_AND_WRITE
}

void Logging::LogError(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Error)
        return;
    FORMAT_AND_WRITE
}

void Logging::LogWarning(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Warning)
        return;
    FORMAT_AND_WRITE
}

void Logging::LogInfo(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Info)
        return;
    FORMAT_AND_WRITE
}

void Logging::LogDebug(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Debug)
        return;
    FORMAT_AND_WRITE
}

void Logging::LogTrace(LPCTSTR lpszFormat, ...)
{
    if(s_logLevel < Trace)
        return;
    FORMAT_AND_WRITE
}
