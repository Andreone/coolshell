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
#include "WindowMinimizer.h"

#include "CoolShellLib\NotifyIcon.h"
#include "CoolShellLib\WinApi.h"
#include "CoolShellLib\Handles.h"

//////////////////////////////////////////////////////////////////////////

#define ID_MENU_CLOSE    1
#define ID_MENU_RESTORE  2

WindowMinimizer::WindowMinimizer() :
    WindowClassFilter(),
    m_minimizedWindowIcons()
{
}

WindowMinimizer::~WindowMinimizer()
{
    try
    {
        while (m_minimizedWindowIcons.size())
        {
            RestoreWindow(m_minimizedWindowIcons.begin()->first);
        }
    }
    catch (std::exception& e)
    {
        TRACE("Exception in ~WindowMinimizer: %s\n", e.what());
    }

    TRACE("Deleted WindowMinimizer\n");
}

void WindowMinimizer::MinimizeWindow(HWND hWnd)
{
    if (!::IsWindow(hWnd))
        throw std::runtime_error("Bad window handle");

    ::ShowWindow(hWnd, SW_HIDE);

    if (m_minimizedWindowIcons.find(hWnd) == m_minimizedWindowIcons.end())
    {
        NotifyIconPtr iconPtr(new NotifyIcon());
        iconPtr->Create(WinApi::GetWindowIcon(hWnd), WinApi::GetWindowText(hWnd).c_str());
        ::SetForegroundWindow(iconPtr->GetOwnerWindow());

        iconPtr->OnNotifyIconEvent().connect([ = ] (NotifyIcon& icon, UINT notifyIconMsg, UINT /*xAnchor*/, UINT /*yAnchor*/)
        {
            // it may happen that the window is destroyed by an other way than the tray icon
            if(!::IsWindow(hWnd))
            {
                this->RemoveIcon(hWnd); 
                return;
            }

            switch(notifyIconMsg)
            {
            case WM_RBUTTONUP:
                this->ContextMenu(icon, hWnd);
                break;
            case WM_LBUTTONUP:
                this->ToggleWindowVisibility(hWnd);
                break;
            case WM_LBUTTONDBLCLK:
                this->RestoreWindow(hWnd);
                break;
            }
        });

        m_minimizedWindowIcons.insert(std::pair<HWND, NotifyIconPtr>(hWnd, iconPtr));
    }
}

void WindowMinimizer::RestoreWindow(HWND hWnd)
{
    ::ShowWindow(hWnd, SW_SHOW);
    ::SetForegroundWindow(hWnd);
    RemoveIcon(hWnd);
}

void WindowMinimizer::ToggleWindowVisibility(HWND hWnd)
{
    if(!::IsWindowVisible(hWnd))
    {
        ::ShowWindow(hWnd, SW_SHOW);
        ::SetForegroundWindow(hWnd);
    }
    else
        ::ShowWindow(hWnd, SW_HIDE);
}

void WindowMinimizer::CloseWindow(HWND hWnd)
{
    ::PostMessage(hWnd, WM_CLOSE, NULL, NULL);
    RemoveIcon(hWnd);
}

void WindowMinimizer::RemoveIcon(HWND hWnd)
{
    m_minimizedWindowIcons.erase(hWnd);
}

void WindowMinimizer::ContextMenu(NotifyIcon& icon, HWND hWnd)
{
    Handles::hmenu menu(::CreatePopupMenu());

    ::AppendMenu(menu.get(), MF_STRING, ID_MENU_RESTORE, _T("Restore window"));
    ::AppendMenu(menu.get(), MF_STRING, ID_MENU_CLOSE, _T("Close window"));

    CPoint pt = GetCursorPos();
    ::SetForegroundWindow(icon.GetOwnerWindow());
    UINT selected = ::TrackPopupMenu(menu.get(), TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, icon.GetOwnerWindow(), NULL);

    switch (selected)
    {
    case ID_MENU_RESTORE:
        TheWindowMinimizer::Instance().RestoreWindow(hWnd);
        break;
    case ID_MENU_CLOSE:
        TheWindowMinimizer::Instance().CloseWindow(hWnd);
        break;
    default:
        TRACE("No case for menu command: %d\n", selected);
        break;
    }
}
