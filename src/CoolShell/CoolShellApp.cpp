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
#include "CoolShellApp.h"

#include <boost/bind.hpp>

#include "CoolShellLib\Handles.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\WinApi.h"

#include "AppConstants.h"
#include "DragWindowModule.h"
#include "WindowsHooks/LowLevelMouseMonitor.h"
#include "WindowsHooks/LowLevelKeyboardMonitor.h"

using namespace WindowsHooks;


CoolShellApp::CoolShellApp() :
    CWinApp(),
    IApplication(),
    m_mouseEventDispatcher(),
    m_wheelManager(),
    m_mouseOverTitleBarManager(),
    m_dragModule(),
    m_mainWindow(),
    m_isPaused(false)
{
    m_mainWindow.Setup(this);
}

CoolShellApp::~CoolShellApp()
{
    ExitInstance();
}

void OnLLKeyboardEvent(LowLevelKeyboardEventArgs args)
{
    TRACE(_T("LLKEY %c\n"), (TCHAR)args.GetVirtualKeyCode());
}

BOOL CoolShellApp::InitInstance()
{
    // create the main application window that holds the tray icon
    m_mainWindow.Create();

    m_mouseOverTitleBarManager = std::make_shared<MouseOverTitleBar>();
    m_mouseOverTitleBarManager->Setup(m_mouseEventDispatcher);

    m_wheelManager = std::make_shared<WheelUnderCursor>();
    m_wheelManager->Setup(m_mouseEventDispatcher);
    //m_wheelManager->FilterWindowClass(_T("Progman"));
    //m_wheelManager->FilterWindowClass(_T("Shell_TrayWnd"));

    m_dragModule.reset(new DragWindowModule());
    m_dragModule->Setup(m_mouseEventDispatcher);

    LowLevelMouseMonitor::Instance().GetEvent().connect(boost::bind(&MouseEventDispatcher::OnLowLevelMouseEvent, &m_mouseEventDispatcher, _1));
    LowLevelMouseMonitor::Instance().Install();
    LOG_INFO(_T("LowLevelMouseMonitor installed"));
    //LowLevelKeyboardMonitor::Instance().GetEvent().connect(boost::bind(&OnLLKeyboardEvent, _1));
    //LowLevelKeyboardMonitor::Instance().Install();
    return TRUE;
}

void CoolShellApp::ExitInstance()
{
    LOG_DEBUG(_T("Deleting LowLevelMouseMonitor..."));
    LowLevelMouseMonitor::DeleteInstance();
    LOG_DEBUG(_T("LowLevelMouseMonitor deleted"));
}

void CoolShellApp::Quit( UINT exitCode )
{
    ::PostQuitMessage(exitCode);
}

void CoolShellApp::TogglePause()
{
    m_isPaused = !m_isPaused;
    m_mainWindow.ReflectPausedState(m_isPaused);

    if(m_isPaused)
        LowLevelMouseMonitor::Instance().Uninstall();
    else
        LowLevelMouseMonitor::Instance().Install();
}

void CoolShellApp::Restart()
{
    if(WinApi::RunCommand(::GetCommandLine(), CString(), SW_NORMAL, ::GetPriorityClass(::GetCurrentProcess())))
        Quit(0);
    else
        LOG_ERROR("Failed to create process: %s", (LPCTSTR)WinApi::GetLastErrorMessage());
}

bool CoolShellApp::IsPaused() const
{
    return m_isPaused;
}

bool CoolShellApp::IsRegisteredToStartWithWindows() const
{
#if _WIN32_WINNT < _WIN32_WINNT_WS08
    auto found = false;
    Handles::hkey key;
    auto result = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), NULL, KEY_READ, key.get_ptr());
    if(result == ERROR_SUCCESS)
    {
        TCHAR valueName[256];
        DWORD charCount;
        for(DWORD index = 0; !found; index++)
        {
            charCount = 256;
            result = ::RegEnumValue((HKEY)key, index, valueName, &charCount, NULL, NULL, NULL, NULL);
            if(result != ERROR_SUCCESS)
                break;
            if(!strcmp(valueName, COOLSHELL_APPLICATION_NAME))
                found = true;
        }
    }
    else
        LOG_ERROR(_T("Failed to open the registry key"));
    return found;
#else
    auto result = ::RegGetValue(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), COOLSHELL_APPLICATION_NAME, RRF_RT_REG_SZ, NULL, NULL, NULL);
    return (result != ERROR_FILE_NOT_FOUND);
#endif
}

void CoolShellApp::StartWithWindows()
{
    Handles::hkey key;
    auto result = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), NULL, KEY_WRITE, key.get_ptr());
    if(result == ERROR_SUCCESS)
    {
        auto path = WinApi::GetCurrentProcessFileName();
        ::RegSetValueEx((HKEY)key, COOLSHELL_APPLICATION_NAME, NULL, REG_SZ, (const BYTE*)(LPCTSTR)path, path.GetLength());
        LOG_INFO(_T("Registry key created"));
    }
    else
        LOG_ERROR(_T("Failed to open the registry key"));
}

void CoolShellApp::DoNotStartWithWindows()
{
    Handles::hkey key;
    auto result = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), NULL, KEY_WRITE, key.get_ptr());
    if(result == ERROR_SUCCESS)
    {
        ::RegDeleteValue((HKEY)key, COOLSHELL_APPLICATION_NAME);
        LOG_INFO(_T("Registry key deleted"));
    }
    else
        LOG_ERROR(_T("Failed to open the registry key"));
}
