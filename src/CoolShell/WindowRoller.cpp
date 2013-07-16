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
#include "WindowRoller.h"

#include "CoolShellLib\WinApi.h"

WindowRoller::WindowRoller() :
    WindowClassFilter(),
    m_rolledWindows()
{
}

WindowRoller::~WindowRoller()
{
    HWND hWnd;

    for(auto i = m_rolledWindows.cbegin(); i != m_rolledWindows.cend(); ++i)
    {
        hWnd = i->first;
        if(::IsWindow(hWnd))
        {
            CRect currentRect = WinApi::GetWindowRect(hWnd);
            UnrollWindow(hWnd, currentRect, i->second);
        }
    }

    TRACE("Deleted WindowRoller\n");
}

void WindowRoller::ToggleRollWindow(HWND hWnd)
{
    CRect currentRect = WinApi::GetWindowRect(hWnd);

    auto i = m_rolledWindows.find(hWnd);
    if (i == m_rolledWindows.end())
    {
        CRect rolledRect(currentRect);
        rolledRect.bottom = rolledRect.top + ::GetSystemMetrics(SM_CYCAPTION);
        RollWindow(hWnd, currentRect, rolledRect);
        m_rolledWindows.insert(std::pair<HWND, RECT>(hWnd, currentRect));
    }
    else
    {
        UnrollWindow(hWnd, currentRect, i->second);
        m_rolledWindows.erase(i);
    }
}

void WindowRoller::RollWindow( HWND hWnd, const CRect& unrolledRect, const CRect& rolledRect )
{
    ::SetWindowPos(hWnd, NULL, 0, 0, unrolledRect.Width(), rolledRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
    TRACE("Rolled window\n");
}

void WindowRoller::UnrollWindow( HWND hWnd, const CRect& rolledRect, const CRect& unrolledRect )
{
    ::SetWindowPos(hWnd, NULL, 0, 0, rolledRect.Width(), unrolledRect.Height(), SWP_NOMOVE|SWP_NOZORDER);
    TRACE("Unrolled window\n");
}
