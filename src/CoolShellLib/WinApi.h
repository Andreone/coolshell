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

namespace WinApi
{
    CString GetWindowText(HWND hWnd);
    CString GetWindowClassName(HWND hWnd);
    CRect GetWindowRect(HWND hWnd);
    HWND GetRootWindow(HWND hWnd);
    HWND RootWindowFromPoint(const POINT& pt);
    HICON GetWindowIcon(HWND hWnd);

    void EnableWindowTransparency(HWND hWnd);
    BOOL SetWindowTransparency(HWND hWnd, COLORREF color, BYTE alphaLevel);
    BOOL SetWindowTransparencyLevel(HWND hWnd, BYTE alphaLevel);
    BOOL SetWindowTransparencyColor(HWND hWnd, COLORREF color);

    bool IsWindowAlwaysOnTop(HWND hWnd);
    BOOL SetWindowAlwaysOnTop(HWND hWnd, bool alwaysOnTop);

    CString GetCurrentProcessFileName();
    CString GetCurrentProcessExecutableName();

    bool RunCommand(const CString& commandLine, const CString& workingDirectory = CString(), WORD wndOptions = SW_SHOWNORMAL, DWORD dwCreationFlags = NULL);

    CString GetErrorMessage(DWORD apiErrorCode);
    inline CString GetLastErrorMessage() { return GetErrorMessage(::GetLastError()); }
    TCHAR* GetWindowsMessageString(UINT msg);


} // namespace WinApi
