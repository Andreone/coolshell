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
#include "WheelUnderCursorService.h"

#include "CoolShellLib\WinApi.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\Mouse.h"

#include "CoolShellConfiguration.h"

WheelUnderCursorService::WheelUnderCursorService(std::shared_ptr<IMouseEventDispatcher> mouseEventDispatcher) :
	m_mouseEventDispatcher(mouseEventDispatcher),
	m_excludedWndClasses()
{
}

void WheelUnderCursorService::Initialize(const WheelUnderCursorServiceConfiguration& configuration)
{
	if(!configuration.enabled)
		return;

	m_mouseEventDispatcher->WheelEvent().connect(std::bind(&WheelUnderCursorService::OnWheel, this, std::placeholders::_1));
    m_mouseEventDispatcher->HWheelEvent().connect(std::bind(&WheelUnderCursorService::OnWheel, this, std::placeholders::_1));

	m_excludedWndClasses = configuration.windowClassExclusionList;
}


bool WheelUnderCursorService::IsWindowClassExcluded(const CString& s) const
{
    return m_excludedWndClasses.end() != std::find(m_excludedWndClasses.begin(), m_excludedWndClasses.end(), s);
}


void WheelUnderCursorService::OnWheel(WindowsHooks::LowLevelMouseEventArgs& args)
{
    HWND hWnd = ::WindowFromPoint(args.GetPoint());
    if(!::IsWindow(hWnd))
        return;

	CString windowClassName = WinApi::GetWindowClassName(WinApi::GetRootWindow(hWnd));
	LOG_INFO(_T("Window class under cursor is '%s'"), (LPCTSTR)windowClassName);

	if(m_excludedWndClasses.end() != std::find(m_excludedWndClasses.begin(), m_excludedWndClasses.end(), windowClassName))
	{
		LOG_INFO(_T("Ignored event: the window class '%s' is excluded"), (LPCTSTR)windowClassName);
		return;
	}

    CPoint mousePos = Mouse::GetCursorPos();

    auto delta = ((short)HIWORD(args.GetMouseData())) > 0 ? 1 : -1;
    if(::PostMessage(hWnd, args.GetMessage(), WHEEL_DELTA * delta << 16, MAKELPARAM(mousePos.x, mousePos.y)))
    {
        LOG_INFO(_T("Posted wheel message at (%d,%d), delta:%d"), mousePos.x, mousePos.y, delta);
        args.SetHandled(true);
    }
    else
    {
        LOG_ERROR(_T("Failed to post mouse wheel: %s"), (LPCTSTR)WinApi::GetLastErrorMessage());
    }
}
