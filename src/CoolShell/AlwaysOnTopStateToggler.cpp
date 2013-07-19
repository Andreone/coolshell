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

#include "CoolShellLib/WinApi.h"
#include "CoolShellLib/Logging.h"

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
			WinApi::SetWindowAlwaysOnTop(hWnd, i->second);
        }
    }
}

void AlwaysOnTopStateToggler::ToggleWindowState(HWND hWnd)
{
    auto i = m_modifiedWindows.find(hWnd);
    if (i == m_modifiedWindows.end())
    {
        bool alwaysOnTop = WinApi::IsWindowAlwaysOnTop(hWnd);
		WinApi::SetWindowAlwaysOnTop(hWnd, !alwaysOnTop);
	    LOG_TRACE(_T("Setting on top state to %s\n"), !alwaysOnTop ? _T("alwaysOnTop") : _T("normal"));
        m_modifiedWindows.insert(std::pair<HWND, bool>(hWnd, alwaysOnTop));
    }
    else
    {
		WinApi::SetWindowAlwaysOnTop(hWnd, i->second);
		LOG_TRACE(_T("Setting on top state to %s\n"), i->second ? _T("alwaysOnTop") : _T("normal"));
		m_modifiedWindows.erase(i);
    }
}
