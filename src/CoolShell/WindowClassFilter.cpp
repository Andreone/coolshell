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
#include "WindowClassFilter.h"

#include "CoolShellLib\Logging.h"
#include "CoolShellLib\WinApi.h"

WindowClassFilter::WindowClassFilter() :
    m_excludedWndClasses()
{
}

void WindowClassFilter::FilterWindowClass(const CString& s)
{
    m_excludedWndClasses.push_back(s);
}

bool WindowClassFilter::IsWindowClassFiltered(const CString& s) const
{
    return m_excludedWndClasses.end() != std::find(m_excludedWndClasses.begin(), m_excludedWndClasses.end(), s);
}

bool WindowClassFilter::IsWindowClassFiltered(HWND hWnd) const
{
    if (!::IsWindow(hWnd))
        return false;

    if (!m_excludedWndClasses.size())
        return false;

    CString className = WinApi::GetWindowClassName(hWnd);
    LOG_DEBUG(_T("Window class name: %s"), (LPCTSTR)className);

    return IsWindowClassFiltered(className);
}
