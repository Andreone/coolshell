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

class Logging
{
public:
    enum LogLevel
    {
        None, Fatal, Error, Warning, Info, Debug, Trace
    };

    static void SetLogLevel(LogLevel level);

    static void LogFatal(LPCTSTR lpszFormat, ...);
    static void LogError(LPCTSTR lpszFormat, ...);
    static void LogWarning(LPCTSTR lpszFormat, ...);
    static void LogInfo(LPCTSTR lpszFormat, ...);
    static void LogDebug(LPCTSTR lpszFormat, ...);
    static void LogTrace(LPCTSTR lpszFormat, ...);

private:
    static void WriteLine(const CString& line);
    static void GetFormatedLine(LPCTSTR lpszFormat, va_list& args, CString& line);
    static volatile LogLevel s_logLevel;
};

#define LOG_FATAL   Logging::LogFatal
#define LOG_ERROR   Logging::LogError
#define LOG_WARN    Logging::LogWarning
#define LOG_INFO    Logging::LogInfo
#define LOG_DEBUG   Logging::LogDebug
#define LOG_TRACE   Logging::LogTrace
