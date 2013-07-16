// This file is part of CoolShell
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
#include "Console.h"

#include <io.h>
#include <fcntl.h>
#include "WinError.h"

////////////////////////////////////////////////////////////////////////////////

Concurrency::critical_section Console::m_cs;
std::list<Console::CtrlHandler> Console::m_ctrlHandlers;

WORD Console::m_nativeFgColorCodes[] =
{
    /* Black */         0,
    /* DarkGray */      FOREGROUND_INTENSITY,
    /* Gray */          FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN,
    /* White */         FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    /* Dark blue */     FOREGROUND_BLUE,
    /* Dark red */      FOREGROUND_RED,
    /* Dark green */    FOREGROUND_GREEN,
    /* Dark cyan */     FOREGROUND_BLUE  | FOREGROUND_GREEN,
    /* Dark yellow */   FOREGROUND_GREEN | FOREGROUND_RED,
    /* Dark pink */     FOREGROUND_BLUE  | FOREGROUND_RED,
    /* Blue */          FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
    /* Red */           FOREGROUND_RED   | FOREGROUND_INTENSITY,
    /* Green */         FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    /* Cyan */          FOREGROUND_BLUE  | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    /* Yellow */        FOREGROUND_GREEN | FOREGROUND_RED   | FOREGROUND_INTENSITY,
    /* Pink */          FOREGROUND_BLUE  | FOREGROUND_RED   | FOREGROUND_INTENSITY
};

WORD Console::m_nativeBgColorCodes[] =
{
    /* Black */         0,
    /* DarkGray */      BACKGROUND_INTENSITY,
    /* Gray */          BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN,
    /* White */         BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    /* Dark blue */     BACKGROUND_BLUE,
    /* Dark red */      BACKGROUND_RED,
    /* Dark green */    BACKGROUND_GREEN,
    /* Dark cyan */     BACKGROUND_BLUE  | BACKGROUND_GREEN,
    /* Dark yellow */   BACKGROUND_GREEN | BACKGROUND_RED,
    /* Dark pink */     BACKGROUND_BLUE  | BACKGROUND_RED,
    /* Blue */          BACKGROUND_BLUE  | BACKGROUND_INTENSITY,
    /* Red */           BACKGROUND_RED   | BACKGROUND_INTENSITY,
    /* Green */         BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    /* Cyan */          BACKGROUND_BLUE  | BACKGROUND_GREEN | BACKGROUND_INTENSITY,
    /* Yellow */        BACKGROUND_GREEN | BACKGROUND_RED   | BACKGROUND_INTENSITY,
    /* Pink */          BACKGROUND_BLUE  | BACKGROUND_RED   | BACKGROUND_INTENSITY
};

#define FGMASK (FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define BGMASK (BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY)
#define COLOR_COUNT 16

////////////////////////////////////////////////////////////////////////////////

BOOL WINAPI Console::CtrlHandlerRoutine(DWORD dwCtrlType)
{
    TRACE(_T("Received console signal: %d\n"), dwCtrlType);

    BOOL handled = FALSE;
    Concurrency::critical_section::scoped_lock lock(m_cs);

    for(auto i = m_ctrlHandlers.begin(); i != m_ctrlHandlers.end() && !handled; ++i)
        handled = (*i)(dwCtrlType);

    return handled;
}

void Console::RegisterCtrlHandler(CtrlHandler handler)
{
    Concurrency::critical_section::scoped_lock lock(m_cs);
    if(m_ctrlHandlers.size() == 0 && !::SetConsoleCtrlHandler(&CtrlHandlerRoutine, TRUE /*Add*/))
        throw WinError();
    m_ctrlHandlers.push_front(handler);
}

void Console::Create()
{
    if (!::AllocConsole())
        throw WinError();

    Console::ReplaceHandles(Console::All);
}

void Console::AttachToParentConsole()
{
    if (!::AttachConsole(ATTACH_PARENT_PROCESS))
        throw WinError();

    Console::ReplaceHandles(Console::All);
}

void Console::Destroy()
{
    ::FreeConsole();
}

void Console::ReplaceHandles(Devices devices)
{
    if (devices & In)
    {
        int crtIn = _open_osfhandle((long) ::GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
        if (crtIn == -1)
            return;

        FILE* fIn = _fdopen(crtIn, "r");
        if (!fIn)
            throw WinError();

        *stdin = *fIn;
    }

    if (devices & Out)
    {
        int crtOut = _open_osfhandle((long) ::GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
        if (crtOut == -1)
            return;

        FILE* fOut = _fdopen(crtOut, "w");
        if (!fOut)
            throw WinError();

        *stdout = *fOut;
    }

    if (devices & Err)
    {
        int crtErr = _open_osfhandle((long) ::GetStdHandle(STD_ERROR_HANDLE), _O_TEXT);
        if (crtErr == -1)
            return;

        FILE* fErr = _fdopen(crtErr, "w");
        if (!fErr)
            throw WinError();

        *stderr = *fErr;
    }
}

////////////////////////////////////////////////////////////////////////////////

void Console::Show()
{
    HWND hConsole = ::GetConsoleWindow();
    if (hConsole)
    {
        ::ShowWindow(hConsole, SW_SHOW);
        ::SetForegroundWindow(hConsole);
    }
}

void Console::Hide()
{
    HWND hConsole = ::GetConsoleWindow();
    if (hConsole)
        ::ShowWindow(hConsole, SW_HIDE);
}

bool Console::IsVisible()
{
    HWND hConsole = ::GetConsoleWindow();
    if (!hConsole)
        return false;

    return (::IsWindowVisible(hConsole) ? true : false);
}

void Console::SetTitle(const CString& title)
{
    if (!::SetConsoleTitle(title))
        throw WinError();
}

CString Console::GetTitle()
{
    CString title;
    if (!::GetConsoleTitle(title.GetBuffer(MAX_PATH-1), MAX_PATH))
        throw WinError();
    title.ReleaseBuffer();
    return title;
}

#if _WIN32_WINNT >= 0x0600

CString Console::GetOriginalTitle()
{
    CString title;
    if(!::GetConsoleOriginalTitle(title.GetBuffer(MAX_PATH), MAX_PATH))
        throw WinError();
    title.ReleaseBuffer();
    return title;
}

#endif

////////////////////////////////////////////////////////////////////////////////

void Console::Write(LPCTSTR text)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    Write(hOutput, text);
}

void Console::Write(Color foregroundColor, Color backgroundColor, LPCTSTR text)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    Write(hOutput, foregroundColor, backgroundColor, text);
}

void Console::WriteFormat(LPCTSTR format, ...)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    va_list args;
    va_start(args, format);
    CString text;
    text.FormatV(format, args);

    Write(hOutput, text);
}

void Console::WriteFormat(Color foregroundColor, Color backgroundColor, LPCTSTR format, ...)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    va_list args;
    va_start(args, format);
    CString text;
    text.FormatV(format, args);

    Write(hOutput, foregroundColor, backgroundColor, text);
}

inline void Console::Write(HANDLE hOutput, LPCTSTR text)
{
    DWORD writtenCount;
    ::WriteConsole(hOutput, text, static_cast<DWORD>(strlen(text)), &writtenCount, NULL);
}

inline void Console::Write(HANDLE hOutput, Color foregroundColor, Color backgroundColor, LPCTSTR text)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    if(foregroundColor != None)
        SetForegroundColor(foregroundColor);

    if(backgroundColor != None)
        SetBackgroundColor(backgroundColor);

    Write(hOutput, text);

    // restore previous colors
    ::SetConsoleTextAttribute(hOutput, info.wAttributes);
}

////////////////////////////////////////////////////////////////////////////////

Console::Color Console::GetForegroundColor()
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return None;

    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    WORD nativeColor = info.wAttributes & FGMASK;
    for(int i = 0; i < COLOR_COUNT; ++i)
        if(m_nativeFgColorCodes[i] == nativeColor)
            return (Color)(i);
    
    return None;
}

Console::Color Console::GetBackgoundColor()
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return None;

    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    WORD nativeColor = info.wAttributes & BGMASK;
    for(int i = 0; i < COLOR_COUNT; ++i)
        if(m_nativeBgColorCodes[i] == nativeColor)
            return (Color)(i);
    
    return None;
}

void Console::SetForegroundColor(Color color)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    info.wAttributes &= BGMASK;
    info.wAttributes |= m_nativeFgColorCodes[color];

    ::SetConsoleTextAttribute(hOutput, info.wAttributes);
}

void Console::SetBackgroundColor(Color color)
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    info.wAttributes &= FGMASK;
    info.wAttributes |= m_nativeBgColorCodes[color];

    ::SetConsoleTextAttribute(hOutput, info.wAttributes);
}

////////////////////////////////////////////////////////////////////////////////

void Console::Clear()
{
    HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOutput <= 0)
        return;

    CONSOLE_SCREEN_BUFFER_INFO info;
    ::GetConsoleScreenBufferInfo(hOutput, &info);

    COORD zeroPoint = {0,0};
    DWORD consoleSize = info.dwSize.X * info.dwSize.Y;
    DWORD writtenCount;

    ::FillConsoleOutputCharacter(hOutput, _T(' '), consoleSize, zeroPoint, &writtenCount);
    ::FillConsoleOutputAttribute(hOutput, info.wAttributes, consoleSize, zeroPoint, &writtenCount);
    ::SetConsoleCursorPosition(hOutput, zeroPoint);
}

////////////////////////////////////////////////////////////////////////////////

#if _DEBUG
void Console::RunTests()
{
    Console::Write(_T("\n\n*** THIS TEXT SHOULD NOT BE VISIBLE BECAUSE THE SCREEN WILL BE CLEARED ***\n\n"));
    Console::Clear();
    Console::Write(_T("If this is not the first line then check Console::Clear() because it just failed.\n\n"));

    Console::Write(Console::Black, Console::White, _T("Black text on white background\n"));
    Console::Write(Console::DarkGray, Console::Gray, _T("Dark gray text on gray background\n"));
    Console::Write(Console::Gray, Console::DarkGray, _T("Gray text on dark gray background\n"));
    Console::Write(Console::White, Console::Black, _T("White text on black background\n"));

    Console::Write(Console::DarkBlue, Console::White, _T("Dark blue text\n"));
    Console::Write(Console::DarkRed, Console::White, _T("Dark red text\n"));
    Console::Write(Console::DarkGreen, Console::White, _T("Dark green text\n"));
    Console::Write(Console::DarkCyan, Console::White, _T("Dark cyan text\n"));
    Console::Write(Console::DarkYellow, Console::White, _T("Dark yellow text\n"));
    Console::Write(Console::DarkPink, Console::White, _T("Dark pink text\n"));
    Console::Write(Console::Blue, Console::White, _T("Blue text\n"));
    Console::Write(Console::Red, Console::Black, _T("Red text\n"));
    Console::Write(Console::Green, Console::Black, _T("Green text\n"));
    Console::Write(Console::Cyan, Console::Black, _T("Cyan text\n"));
    Console::Write(Console::Yellow, Console::Black, _T("Yellow text\n"));
    Console::Write(Console::Pink, Console::Black, _T("Pink text\n"));

    Console::Write(Console::White, Console::DarkBlue, _T("Dark blue background\n"));
    Console::Write(Console::White, Console::DarkRed, _T("Dark red background\n"));
    Console::Write(Console::White, Console::DarkGreen, _T("Dark green background\n"));
    Console::Write(Console::White, Console::DarkCyan, _T("Dark cyan background\n"));
    Console::Write(Console::White, Console::DarkYellow, _T("Dark yellow background\n"));
    Console::Write(Console::White, Console::DarkPink, _T("Dark pink background\n"));
    Console::Write(Console::White, Console::Blue, _T("Blue background\n"));
    Console::Write(Console::Black, Console::Red, _T("Red background\n"));
    Console::Write(Console::Black, Console::Green, _T("Green background\n"));
    Console::Write(Console::Black, Console::Cyan, _T("Cyan background\n"));
    Console::Write(Console::Black, Console::Yellow, _T("Yellow background\n"));
    Console::Write(Console::Black, Console::Pink, _T("Pink background\n"));

    Color fgColor = GetForegroundColor();
    Color bgColor = GetBackgoundColor();
    Console::SetBackgroundColor(fgColor);
    Console::SetForegroundColor(bgColor);
    Console::Write(_T("\nForeground and background colors has been swapped.\n"));
}
#endif // _DEBUG
