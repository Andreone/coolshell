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
#include "WindowTitleBarService.h"

#include <boost/bind.hpp>

#include "CoolShellLib\Logging.h"
#include "CoolShellLib\WinApi.h"

#include "CoolShellConfiguration.h"
#include "WindowAction.h"
#include "WindowMinimizer.h"
#include "WindowRoller.h"

WindowTitleBarService::WindowTitleBarService(std::shared_ptr<IMouseEventDispatcher> mouseEventDispatcher) :
	m_mouseEventDispatcher(mouseEventDispatcher),
    m_actions()
{ 
}

void WindowTitleBarService::RegisterAction( UINT area, UINT mouseEvent, MouseAction action )
{
    m_actions.insert(MouseActionMapPair(std::make_tuple(area, mouseEvent), action));
}

void WindowTitleBarService::Initialize(const WindowTitleBarServiceConfiguration& configuration)
{
	if(!configuration.enabled)
		return;

	m_mouseEventDispatcher->WheelEvent().connect(boost::bind(&WindowTitleBarService::OnMouseEvent, this, _1));
    m_mouseEventDispatcher->MButtonUpEvent().connect(boost::bind(&WindowTitleBarService::OnMouseEvent, this, _1));
    m_mouseEventDispatcher->RButtonUpEvent().connect(boost::bind(&WindowTitleBarService::OnMouseEvent, this, _1));
    m_mouseEventDispatcher->RButtonDownEvent().connect(boost::bind(&WindowTitleBarService::OnMouseEvent, this, _1));

    RegisterAction(HTMINBUTTON, WM_RBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        TheWindowMinimizer::Instance().MinimizeWindow(hWnd);
        args.SetHandled(true);
    } );

    RegisterAction(HTMINBUTTON, WM_RBUTTONDOWN, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        args.SetHandled(true);
    } );

    RegisterAction(HTMAXBUTTON, WM_RBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        TheWindowRoller::Instance().ToggleRollWindow(hWnd);
        } );

    RegisterAction(HTCLOSE, WM_RBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        WindowAction::ToggleWindowState(hWnd);
    } );

    RegisterAction(HTMINBUTTON, WM_MBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        WinApi::EnableWindowTransparency(hWnd);
        WinApi::SetWindowTransparencyLevel(hWnd, 50);
        LOG_INFO("Opacity set to 20%%");
    } );

    RegisterAction(HTMAXBUTTON, WM_MBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        WinApi::EnableWindowTransparency(hWnd);
        WinApi::SetWindowTransparencyLevel(hWnd, 200);
        LOG_INFO("Opacity set to 80%%");
    } );

    RegisterAction(HTCLOSE, WM_MBUTTONUP, [] (HWND hWnd, WindowsHooks::LowLevelMouseEventArgs& args) {
        WinApi::EnableWindowTransparency(hWnd);
        WinApi::SetWindowTransparencyLevel(hWnd, 255);
        LOG_INFO("Opacity set to 100%%");
    } );
}

void WindowTitleBarService::OnMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args)
{
    if(args.IsHandled() || args.IsInjected())
        return;

    HWND hWnd = ::WindowFromPoint(args.GetPoint());
    if(!::IsWindow(hWnd))
        return;

    LRESULT area = ::SendMessage(hWnd, WM_NCHITTEST, NULL, MAKELPARAM(args.GetPoint().x, args.GetPoint().y));

    auto key = std::make_tuple(static_cast<UINT>(area), args.GetMessage());
    auto i = m_actions.find(key);
    if(i != m_actions.end())
    {
        LOG_TRACE(_T("Found registered action"));
        i->second(hWnd, args);
    }
}
