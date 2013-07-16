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
#include "WinApi.h"

#include "WinError.h"

namespace WinApi
{
CString GetWindowText(HWND hWnd)
{
    CString title;
    TCHAR szTitle[512];
    if (::GetWindowText(hWnd, szTitle, 512) > 0)
        title = szTitle;
    return title;
}

CString GetWindowClassName(HWND hWnd)
{
    CString className;
    TCHAR szClassName[MAX_CLASS_NAME+1];
    if (::GetClassName(hWnd, szClassName, MAX_CLASS_NAME+1) > 0)
        className = szClassName;
    return className;
}

CRect GetWindowRect(HWND hWnd)
{
    CRect rect;
    ::GetWindowRect(hWnd, (LPRECT)rect);
    return rect;
}

HWND RootWindowFromPoint(const POINT& pt)
{
    HWND hWnd = ::WindowFromPoint(pt);
    if(!::IsWindow(hWnd))
        return NULL;
    return GetRootWindow(hWnd);
}

HWND GetRootWindow(HWND hWnd)
{
    return ::GetAncestor(hWnd, GA_ROOT);
}

void EnableWindowTransparency(HWND hWnd)
{
    ::SetWindowLong(hWnd, GWL_EXSTYLE, ::GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
}

BOOL SetWindowTransparencyLevel(HWND hWnd, BYTE alphaLevel)
{
    return ::SetLayeredWindowAttributes(hWnd, RGB(255,255,255), alphaLevel, LWA_ALPHA);
}

BOOL SetWindowTransparencyColor(HWND hWnd, COLORREF color)
{
    return ::SetLayeredWindowAttributes(hWnd, color, 0, LWA_COLORKEY);
}

BOOL SetWindowTransparency(HWND hWnd, COLORREF color, BYTE alphaLevel)
{
    return ::SetLayeredWindowAttributes(hWnd, color, alphaLevel, LWA_COLORKEY|LWA_ALPHA);
}

HICON GetWindowIcon(HWND hWnd)
{
    HICON icon = NULL;
    if(icon = (HICON) ::SendMessage(hWnd, WM_GETICON, ICON_SMALL2, NULL)) return icon;
    if(icon = (HICON) ::SendMessage(hWnd, WM_GETICON, ICON_SMALL, NULL)) return icon;
    if(icon = (HICON) ::SendMessage(hWnd, WM_GETICON, ICON_BIG, NULL)) return icon;
    if(icon = (HICON) ::GetClassLong(hWnd, GCLP_HICONSM)) return icon;
    if(icon = (HICON) ::GetClassLong(hWnd, GCLP_HICON)) return icon;
    return ::LoadIcon(NULL, IDI_WINLOGO);
}

CString GetErrorMessage(DWORD apiErrorCode /*= ::GetLastError()*/)
{
    LPVOID pBuffer = nullptr;
    CString errMsg;

    if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                      NULL,
                      apiErrorCode,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      (LPTSTR) &pBuffer,
                      0,
                      NULL))
    {
        errMsg = (LPSTR)pBuffer;
        ::LocalFree(pBuffer);
    }
    else
    {
        errMsg.Format(_T("No system message defined for the error code %d"), apiErrorCode);
    }

    return errMsg;
}

CString GetCurrentProcessExecutableName()
{
    CString filePath = GetCurrentProcessFileName();
    int index = filePath.ReverseFind(_T("\\"));
    return index != -1 ? filePath.Mid(index+1, filePath.GetLength()-1) : filePath;
}

CString GetCurrentProcessFileName()
{
    HMODULE hCurrentModule = ::GetModuleHandle (NULL);
    if (!hCurrentModule)
        throw WinError();

    TCHAR pFilePath[MAX_PATH];
    if (!::GetModuleFileName(hCurrentModule, pFilePath, MAX_PATH))
        throw WinError();

    return CString(pFilePath);
}

bool RunCommand(const CString& commandLine,
                const CString& workingDirectory,
                WORD wndOptions /*= SW_SHOWNORMAL*/,
                DWORD dwCreationFlags /*= NULL*/)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ::ZeroMemory( &si, sizeof(si));
    ::ZeroMemory( &pi, sizeof(pi));

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = wndOptions;

    CString commandLineCopy = commandLine;
    BOOL success = ::CreateProcess(
                       NULL,                            // No module name (use command line)
                       commandLineCopy.GetBuffer(commandLineCopy.GetLength()),    // Command line
                       NULL,                            // Process handle inheritance
                       NULL,                            // Thread handle inheritance
                       FALSE,                           // handle inheritance
                       dwCreationFlags,                 // creation flags
                       NULL,                            // Use parent's environment block
                       workingDirectory.IsEmpty() ? NULL : (LPCTSTR)workingDirectory,
                       &si,
                       &pi);
    commandLineCopy.ReleaseBuffer();
    
    if (success)
    {
        TRACE("Process %s created successfully\n", (LPCTSTR)commandLine);
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
        return true;
    }
    else
    {
        TRACE("Error when creating process %s: %s\n", (LPCTSTR)commandLine, (LPCTSTR)WinApi::GetLastErrorMessage());
        return false;
    }
}

/**
 * \brief Returns a string label for a given Windows message id (WM_...)
 *
 * \param [in] id: a mouse message id
 * \return the label
 */
TCHAR* GetWindowsMessageString(UINT msg)
{
#define CASE_ENTRY(msg) case msg: return _T(#msg);

    switch(msg)
    {
    // Window
    CASE_ENTRY(WM_NULL)
    CASE_ENTRY(WM_CREATE)
    CASE_ENTRY(WM_DESTROY)
    CASE_ENTRY(WM_MOVE)
    CASE_ENTRY(WM_SIZE)

    CASE_ENTRY(WM_SETFOCUS)
    CASE_ENTRY(WM_KILLFOCUS)
    CASE_ENTRY(WM_ENABLE)
    CASE_ENTRY(WM_SETREDRAW)
    CASE_ENTRY(WM_SETTEXT)
    CASE_ENTRY(WM_GETTEXT)
    CASE_ENTRY(WM_GETTEXTLENGTH)
    CASE_ENTRY(WM_PAINT)
    CASE_ENTRY(WM_CLOSE)
#ifndef _WIN32_WCE
    CASE_ENTRY(WM_QUERYENDSESSION)
    CASE_ENTRY(WM_QUERYOPEN)
    CASE_ENTRY(WM_ENDSESSION)
#endif
    CASE_ENTRY(WM_QUIT)
    CASE_ENTRY(WM_ERASEBKGND)
    CASE_ENTRY(WM_SYSCOLORCHANGE)
    CASE_ENTRY(WM_SHOWWINDOW)
    CASE_ENTRY(WM_WININICHANGE)
    CASE_ENTRY(WM_DEVMODECHANGE)
    CASE_ENTRY(WM_ACTIVATEAPP)
    CASE_ENTRY(WM_FONTCHANGE)
    CASE_ENTRY(WM_TIMECHANGE)
    CASE_ENTRY(WM_CANCELMODE)
    CASE_ENTRY(WM_SETCURSOR)
    CASE_ENTRY(WM_MOUSEACTIVATE)
    CASE_ENTRY(WM_CHILDACTIVATE)
    CASE_ENTRY(WM_QUEUESYNC)

    CASE_ENTRY(WM_GETMINMAXINFO)
    CASE_ENTRY(WM_PAINTICON)
    CASE_ENTRY(WM_ICONERASEBKGND)
    CASE_ENTRY(WM_NEXTDLGCTL)
    CASE_ENTRY(WM_SPOOLERSTATUS)
    CASE_ENTRY(WM_DRAWITEM)
    CASE_ENTRY(WM_MEASUREITEM)
    CASE_ENTRY(WM_DELETEITEM)
    CASE_ENTRY(WM_VKEYTOITEM)
    CASE_ENTRY(WM_CHARTOITEM)
    CASE_ENTRY(WM_SETFONT)
    CASE_ENTRY(WM_GETFONT)
    CASE_ENTRY(WM_SETHOTKEY)
    CASE_ENTRY(WM_GETHOTKEY)
    CASE_ENTRY(WM_QUERYDRAGICON)
    CASE_ENTRY(WM_COMPAREITEM)
#if(WINVER >= 0x0500)
#ifndef _WIN32_WCE
    CASE_ENTRY(WM_GETOBJECT)
#endif
#endif /* WINVER >= 0x0500 */
    CASE_ENTRY(WM_COMPACTING)
    CASE_ENTRY(WM_COMMNOTIFY)
    CASE_ENTRY(WM_WINDOWPOSCHANGING)
    CASE_ENTRY(WM_WINDOWPOSCHANGED)
    CASE_ENTRY(WM_POWER)
    CASE_ENTRY(WM_COPYDATA)
    CASE_ENTRY(WM_CANCELJOURNAL)

#if(WINVER >= 0x0400)
    CASE_ENTRY(WM_NOTIFY)
    CASE_ENTRY(WM_INPUTLANGCHANGEREQUEST)
    CASE_ENTRY(WM_INPUTLANGCHANGE)
    CASE_ENTRY(WM_TCARD)
    CASE_ENTRY(WM_HELP)
    CASE_ENTRY(WM_USERCHANGED)
    CASE_ENTRY(WM_NOTIFYFORMAT)
                                         
    CASE_ENTRY(WM_CONTEXTMENU)
    CASE_ENTRY(WM_STYLECHANGING)
    CASE_ENTRY(WM_STYLECHANGED)
    CASE_ENTRY(WM_DISPLAYCHANGE)
    CASE_ENTRY(WM_GETICON)
    CASE_ENTRY(WM_SETICON)
#endif /* WINVER >= 0x0400 */            
                                         
    CASE_ENTRY(WM_NCCREATE)
    CASE_ENTRY(WM_NCDESTROY)
    CASE_ENTRY(WM_NCCALCSIZE)
    CASE_ENTRY(WM_NCHITTEST)
    CASE_ENTRY(WM_NCPAINT)
    CASE_ENTRY(WM_NCACTIVATE)
    CASE_ENTRY(WM_GETDLGCODE)
#ifndef _WIN32_WCE                       
    CASE_ENTRY(WM_SYNCPAINT)
#endif                                   
    CASE_ENTRY(WM_NCMOUSEMOVE)
    CASE_ENTRY(WM_NCLBUTTONDOWN)
    CASE_ENTRY(WM_NCLBUTTONUP)
    CASE_ENTRY(WM_NCLBUTTONDBLCLK)
    CASE_ENTRY(WM_NCRBUTTONDOWN)
    CASE_ENTRY(WM_NCRBUTTONUP)
    CASE_ENTRY(WM_NCRBUTTONDBLCLK)
    CASE_ENTRY(WM_NCMBUTTONDOWN)
    CASE_ENTRY(WM_NCMBUTTONUP)
    CASE_ENTRY(WM_NCMBUTTONDBLCLK)
#if(_WIN32_WINNT >= 0x0500)              
    CASE_ENTRY(WM_NCXBUTTONDOWN)
    CASE_ENTRY(WM_NCXBUTTONUP)
    CASE_ENTRY(WM_NCXBUTTONDBLCLK)
#endif /* _WIN32_WINNT >= 0x0500 */

    // Keyboard
    CASE_ENTRY(WM_KEYDOWN)
    CASE_ENTRY(WM_KEYUP)
    CASE_ENTRY(WM_CHAR)
    CASE_ENTRY(WM_DEADCHAR)
    CASE_ENTRY(WM_SYSKEYDOWN)
    CASE_ENTRY(WM_SYSKEYUP)
    CASE_ENTRY(WM_SYSCHAR)
    CASE_ENTRY(WM_SYSDEADCHAR)
#if(_WIN32_WINNT >= 0x0501)
    CASE_ENTRY(WM_UNICHAR)
    CASE_ENTRY(UNICODE_NOCHAR)
#endif /* _WIN32_WINNT >= 0x0501 */

    CASE_ENTRY(WM_CTLCOLORMSGBOX)
    CASE_ENTRY(WM_CTLCOLOREDIT)
    CASE_ENTRY(WM_CTLCOLORLISTBOX)
    CASE_ENTRY(WM_CTLCOLORBTN)
    CASE_ENTRY(WM_CTLCOLORDLG)
    CASE_ENTRY(WM_CTLCOLORSCROLLBAR)
    CASE_ENTRY(WM_CTLCOLORSTATIC)
    CASE_ENTRY(MN_GETHMENU)

    // Mouse
    CASE_ENTRY(WM_LBUTTONDOWN)
    CASE_ENTRY(WM_LBUTTONUP)
    CASE_ENTRY(WM_LBUTTONDBLCLK)
    CASE_ENTRY(WM_RBUTTONDOWN)
    CASE_ENTRY(WM_RBUTTONUP)
    CASE_ENTRY(WM_RBUTTONDBLCLK)
    CASE_ENTRY(WM_MBUTTONDOWN)
    CASE_ENTRY(WM_MBUTTONUP)
    CASE_ENTRY(WM_MBUTTONDBLCLK)
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400)
    CASE_ENTRY(WM_MOUSEWHEEL)
#endif
#if (_WIN32_WINNT >= 0x0500)
    CASE_ENTRY(WM_XBUTTONUP)
    CASE_ENTRY(WM_XBUTTONDOWN)
    CASE_ENTRY(WM_XBUTTONDBLCLK)
#endif
#if (_WIN32_WINNT >= 0x0600)
    CASE_ENTRY(WM_MOUSEHWHEEL)
#endif
    CASE_ENTRY(WM_MOUSEMOVE)

    CASE_ENTRY(WM_PARENTNOTIFY)
    CASE_ENTRY(WM_ENTERMENULOOP)
    CASE_ENTRY(WM_EXITMENULOOP)
#if(WINVER >= 0x0400)
    CASE_ENTRY(WM_NEXTMENU)
    CASE_ENTRY(WM_SIZING)
    CASE_ENTRY(WM_CAPTURECHANGED)
    CASE_ENTRY(WM_MOVING)
#endif /* WINVER >= 0x0400 */
    CASE_ENTRY(WM_POWERBROADCAST)
#if(WINVER >= 0x0400)
    CASE_ENTRY(WM_DEVICECHANGE)
#endif /* WINVER >= 0x0400 */
    CASE_ENTRY(WM_MDICREATE)
    CASE_ENTRY(WM_MDIDESTROY)
    CASE_ENTRY(WM_MDIACTIVATE)
    CASE_ENTRY(WM_MDIRESTORE)
    CASE_ENTRY(WM_MDINEXT)
    CASE_ENTRY(WM_MDIMAXIMIZE)
    CASE_ENTRY(WM_MDITILE)
    CASE_ENTRY(WM_MDICASCADE)
    CASE_ENTRY(WM_MDIICONARRANGE)
    CASE_ENTRY(WM_MDIGETACTIVE)
    CASE_ENTRY(WM_MDISETMENU)
    CASE_ENTRY(WM_ENTERSIZEMOVE)
    CASE_ENTRY(WM_EXITSIZEMOVE)
    CASE_ENTRY(WM_DROPFILES)
    CASE_ENTRY(WM_MDIREFRESHMENU)
#if(WINVER >= 0x0601)
    CASE_ENTRY(WM_TOUCH)
#endif /* WINVER >= 0x0601 */
    CASE_ENTRY(WM_CUT)
    CASE_ENTRY(WM_COPY)
    CASE_ENTRY(WM_PASTE)
    CASE_ENTRY(WM_CLEAR)
    CASE_ENTRY(WM_UNDO)
    CASE_ENTRY(WM_RENDERFORMAT)
    CASE_ENTRY(WM_RENDERALLFORMATS)
    CASE_ENTRY(WM_DESTROYCLIPBOARD)
    CASE_ENTRY(WM_DRAWCLIPBOARD)
    CASE_ENTRY(WM_PAINTCLIPBOARD)
    CASE_ENTRY(WM_VSCROLLCLIPBOARD)
    CASE_ENTRY(WM_SIZECLIPBOARD)
    CASE_ENTRY(WM_ASKCBFORMATNAME)
    CASE_ENTRY(WM_CHANGECBCHAIN)
    CASE_ENTRY(WM_HSCROLLCLIPBOARD)
    CASE_ENTRY(WM_QUERYNEWPALETTE)
    CASE_ENTRY(WM_PALETTEISCHANGING)
    CASE_ENTRY(WM_PALETTECHANGED)
    CASE_ENTRY(WM_HOTKEY)
#if(WINVER >= 0x0400)
    CASE_ENTRY(WM_PRINT)
    CASE_ENTRY(WM_PRINTCLIENT)
#endif /* WINVER >= 0x0400 */
#if(_WIN32_WINNT >= 0x0500)
    CASE_ENTRY(WM_APPCOMMAND)
#endif /* _WIN32_WINNT >= 0x0500 */
#if(_WIN32_WINNT >= 0x0501)
    CASE_ENTRY(WM_THEMECHANGED)
#endif /* _WIN32_WINNT >= 0x0501 */
#if(_WIN32_WINNT >= 0x0501)
    CASE_ENTRY(WM_CLIPBOARDUPDATE)
#endif /* _WIN32_WINNT >= 0x0501 */
#if(_WIN32_WINNT >= 0x0600)
    CASE_ENTRY(WM_DWMCOMPOSITIONCHANGED)
    CASE_ENTRY(WM_DWMNCRENDERINGCHANGED)
    CASE_ENTRY(WM_DWMCOLORIZATIONCOLORCHANGED)
    CASE_ENTRY(WM_DWMWINDOWMAXIMIZEDCHANGE)
#endif /* _WIN32_WINNT >= 0x0600 */
#if(_WIN32_WINNT >= 0x0601)
    CASE_ENTRY(WM_DWMSENDICONICTHUMBNAIL)
    CASE_ENTRY(WM_DWMSENDICONICLIVEPREVIEWBITMAP)
#endif /* _WIN32_WINNT >= 0x0601 */
#if(WINVER >= 0x0600)
    CASE_ENTRY(WM_GETTITLEBARINFOEX)
#endif /* WINVER >= 0x0600 */
    default:
        return _T("Unknown message");
    }
}

} // namespace WinApi