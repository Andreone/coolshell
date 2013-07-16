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

#pragma once

#include <functional>
#include <list>
#include <concrt.h>

/**
 * \class Console
 * \brief Windows native Console API wrapper
 */ 
class Console
{
public:
    typedef std::function<BOOL (DWORD)> CtrlHandler;

    enum Devices
    {
        In  = 1, /** stdin */
        Out = 2, /** stdout */
        Err = 4, /** stderr */
        All = In | Out | Err /** all std streams */
    };

    enum Color
    {
        None = -1,
        Black,
        DarkGray,
        Gray,
        White,
        DarkBlue,
        DarkRed,
        DarkGreen,
        DarkCyan,
        DarkYellow,
        DarkPink,
        Blue,
        Red,
        Green,
        Cyan,
        Yellow,
        Pink
    };

    static void Create();
    static void AttachToParentConsole();
    static void Destroy();
    static void ReplaceHandles(Devices devices);
    static void RegisterCtrlHandler(CtrlHandler handler);

    static void Write(LPCTSTR text);
    static void Write(Color foregroundColor, Color backgroundColor, LPCTSTR text);
    static void WriteFormat(LPCTSTR format, ...);
    static void WriteFormat(Color foregroundColor, Color backgroundColor, LPCTSTR format, ...);
    static void Clear();
    static void SetForegroundColor(Color color);
    static void SetBackgroundColor(Color color);
    static Color GetForegroundColor();
    static Color GetBackgoundColor();

    static void Show();
    static void Hide();
    static bool IsVisible();
    static void SetTitle(const CString& title);
    static CString GetTitle();
#if _WIN32_WINNT >= 0x0600
    static CString GetOriginalTitle();
#endif
#if _DEBUG
    static void RunTests();
#endif
private:
    static void Write(HANDLE hOutput, LPCTSTR text);
    static void Write(HANDLE hOutput, Color foregroundColor, Color backgroundColor, LPCTSTR text);
    static BOOL WINAPI CtrlHandlerRoutine(DWORD dwCtrlType);
    static void SetColors(WORD colors);
    static WORD GetColors();

    static Concurrency::critical_section m_cs;
    static std::list<CtrlHandler> m_ctrlHandlers;
    static WORD m_nativeFgColorCodes[];
    static WORD m_nativeBgColorCodes[];
};
