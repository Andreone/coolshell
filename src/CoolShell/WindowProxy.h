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

#include <string>
#include <boost/optional.hpp>

#include "Win32xx\cstring.h"
#include <Windows.h>

struct IWindowProxy
{
	virtual bool IsWindow() const = 0;
	virtual CString GetWindowClassName() const = 0;
	virtual CString GetWindowText() const = 0;

};

class WindowProxy : public IWindowProxy
{
public:
	static WindowProxy FromPoint(const POINT& point);

	WindowProxy(HWND windowHandle) :
		m_hWnd(windowHandle)
	{
	}
	
	bool IsWindow() const override
	{
		return ::IsWindow(m_hWnd);
	}

	CString GetWindowClassName() const override
	{
		if (!m_windowClassName.is_initialized())
		{

		}

		return m_windowClassName.get();
	}
	
	CString GetWindowText() const override
	{
		if (!m_windowText.is_initialized())
		{

		}

		return m_windowText.get();
	}

private:
	HWND m_hWnd;
	boost::optional<CString> m_windowClassName;
	boost::optional<CString> m_windowText;
};
