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
#include "CoolShellMainWnd.h"

#include "Win32xx\dialog.h"

#include "CoolShellLib\Console.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\Version.h"

#include "AppConstants.h"
#include "IApplication.h"
#include "Resource.h"

class CAboutBox : public CDialog
{
public:
    CAboutBox(CWnd* pParent = NULL) :
        CDialog(IDD_ABOUTBOX, pParent)
    {
    }

protected:
    virtual BOOL OnInitDialog()
    {
        auto v = Version::GetFileVersion(_T("CoolShell.exe"));
        SetDlgItemText(IDC_STATIC_VERSION, v.ToString<CString>());
        return TRUE;
    }
};

class NotifyIconMenu : public CMenu
{
public:
    NotifyIconMenu(CWnd& parentWnd, IApplication& application) :
        CMenu(),
        m_parentWnd(parentWnd),
        m_application(application)
    {
    }

    void Show()
    {
        CreatePopupMenu();
        AppendMenu(MF_STRING, ID_MENU_ABOUT, _T("About"));
        AppendMenu(MF_STRING | (m_application.IsRegisteredToStartWithWindows() ? MF_CHECKED : MF_UNCHECKED),
            ID_MENU_START_WITH_WINDOWS, _T("Start With Windows"));
        AppendMenu(MF_STRING, ID_MENU_CONSOLE, Console::IsVisible() ? _T("Hide Console") : _T("Show Console"));
        AppendMenu(MF_SEPARATOR);
        AppendMenu(MF_STRING, ID_MENU_TOGGLEPAUSE, m_application.IsPaused() ? _T("Resume") : _T("Pause"));
        AppendMenu(MF_STRING, ID_MENU_RESTART, _T("Restart"));
        AppendMenu(MF_STRING, ID_MENU_EXIT, _T("Exit"));

        m_parentWnd.SetForegroundWindow();
        auto pt = GetCursorPos();
        auto selected = this->TrackPopupMenu(TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, &m_parentWnd);

        switch(selected)
        {
        case ID_MENU_EXIT:
            m_application.Quit(0);
            break;
        case ID_MENU_TOGGLEPAUSE:
            m_application.TogglePause();
            break;
        case ID_MENU_RESTART:
            m_application.Restart();
            break;
        case ID_MENU_ABOUT:
            {
                CAboutBox dlg(&m_parentWnd);
                dlg.DoModal();
            }
            break;
        case ID_MENU_CONSOLE:
            Console::IsVisible() ? Console::Hide() : Console::Show();
            break;
        case ID_MENU_START_WITH_WINDOWS:
            if(m_application.IsRegisteredToStartWithWindows())
                m_application.DoNotStartWithWindows();
            else
                m_application.StartWithWindows();
            break;
        default:
            break;
        }
    }

private:
    CWnd& m_parentWnd;
    IApplication& m_application;

    enum MenuItem
    {
        ID_MENU_EXIT = 1,
        ID_MENU_TOGGLEPAUSE,
        ID_MENU_RESTART,
        ID_MENU_ABOUT,
        ID_MENU_CONSOLE,
        ID_MENU_START_WITH_WINDOWS,
    };
};

//////////////////////////////////////////////////////////////////////////

CoolShellMainWnd::CoolShellMainWnd() :
    CWnd(),
    m_application(nullptr),
    m_notifyIcon(),
    m_leftClickManager()
{
}

void CoolShellMainWnd::Setup(IApplication* application)
{
    m_application = application;

    m_leftClickManager.WhenSimpleClick([] { Console::IsVisible() ? Console::Hide() : Console::Show(); } );
    m_leftClickManager.WhenDoubleClick([this] { m_application->TogglePause(); } );
}

HWND CoolShellMainWnd::CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, 
                            int x, int y, int nWidth, int nHeight, CWnd* pParent, CMenu* pMenu, LPVOID lpParam /*= NULL*/)
{
    dwStyle ^= WS_VISIBLE;
    return CWnd::CreateEx(dwExStyle, lpszClassName, lpszWindowName, dwStyle, x, y, nWidth, nHeight, pParent, pMenu, lpParam);
}

void CoolShellMainWnd::OnInitialUpdate()
{
    m_notifyIcon.GetNotifyIconEvent().connect([this] (NotifyIcon& /*icon*/, UINT notifyIconMsg, UINT /*xAnchor*/, UINT /*yAnchor*/)
    {
        switch (notifyIconMsg)
        {
        case WM_RBUTTONUP:
            {
                NotifyIconMenu menu(*this, *m_application);
                menu.Show();
            }
            break;

        case WM_LBUTTONDBLCLK: // WM_LBUTTONDBLCLK is sent instead of WM_LBUTTONDOWN when a double-click occurs
        case WM_LBUTTONDOWN:
            m_leftClickManager.OnButtonDown();
            break;
        case WM_LBUTTONUP:
            m_leftClickManager.OnButtonUp();
            break;
        }
    });

    m_notifyIcon.Create(m_hWnd, ::LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDI_COOLSHELL)), COOLSHELL_APPLICATION_NAME);
}

LRESULT CoolShellMainWnd::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        LOG_INFO(_T("[CoolShellMainWnd] WM_DESTROY received"));
        m_application->Quit(0);
        break;
    }

    if (m_notifyIcon.WndProc(uMsg, wParam, lParam))
        return FALSE;

    return CWnd::WndProc(uMsg, wParam, lParam);
}

void CoolShellMainWnd::ReflectPausedState( bool isPaused )
{
    auto hIcon = ::LoadIcon(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(isPaused ? IDI_COOLSHELL_DISABLED : IDI_COOLSHELL));
    m_notifyIcon.SetIcon(hIcon);
}
