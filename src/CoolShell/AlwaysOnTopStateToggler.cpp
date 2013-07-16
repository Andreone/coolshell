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
#include "AlwaysOnTopStateToggler.h"

AlwaysOnTopStateToggler::AlwaysOnTopStateToggler() :
    WindowClassFilter(),
    m_modifiedWindows()
{
}

AlwaysOnTopStateToggler::~AlwaysOnTopStateToggler()
{
    HWND hWnd;

    for(auto i = m_modifiedWindows.cbegin(); i != m_modifiedWindows.cend(); ++i)
    {
        hWnd = i->first;
        if(::IsWindow(hWnd))
        {
            SetState(hWnd, i->second);
        }
    }

    TRACE(_T("Deleted AlwaysOnTopStateToggler\n"));
}

bool AlwaysOnTopStateToggler::IsAlwaysOnTop(HWND hWnd)
{
    DWORD dwExStyle = ::GetWindowLong(hWnd, GWL_EXSTYLE);
    return (dwExStyle & WS_EX_TOPMOST) != 0;
}

/**
 * \brief Set the always on top state of a window.
 *
 * \param [in] hWnd: a valid window handle
 * \param [in] onTop: true to set always on top, false to remove always on top
 */
void AlwaysOnTopStateToggler::SetState(HWND hWnd, bool alwaysOnTop)
{
    ASSERT(::IsWindow(hWnd));
    TRACE(_T("Setting on top state to %s\n"), alwaysOnTop ? _T("alwaysOnTop") : _T("normal"));
    ::SetWindowPos(hWnd, (alwaysOnTop ? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
}

void AlwaysOnTopStateToggler::ToggleWindowState(HWND hWnd)
{
    auto i = m_modifiedWindows.find(hWnd);
    if (i == m_modifiedWindows.end())
    {
        bool isOnTop = IsAlwaysOnTop(hWnd);
        SetState(hWnd, !isOnTop);
        m_modifiedWindows.insert(std::pair<HWND, bool>(hWnd, isOnTop));
    }
    else
    { 
        SetState(i->first, i->second);
        m_modifiedWindows.erase(i);
    }
}
