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
#include "MouseEventDispatcher.h"

#include "CoolShellLib\Logging.h"
#include "CoolShellLib\WinApi.h"

MouseEventDispatcher::MouseEventDispatcher() :
    IMouseEventDispatcher(),
    IMouseHookListener(),
    m_LButtonDownEvent(),
    m_LButtonUpEvent(),
    m_RButtonDownEvent(),
    m_RButtonUpEvent(),
    m_MButtonDownEvent(),
    m_MButtonUpEvent(),
    m_WheelEvent(),
    m_HWheelEvent(),
    m_MouseMoveEvent()
{
}

MouseEventDispatcher::~MouseEventDispatcher()
{
}

void MouseEventDispatcher::OnLowLevelMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args)
{
    try
    {
        if(args.GetMessage() != WM_MOUSEMOVE)
        {
            LOG_TRACE(_T("Received %s at (%d,%d) - Injected=%s"), 
                WinApi::GetWindowsMessageString(args.GetMessage()), 
                args.GetPoint().x, 
                args.GetPoint().y, 
                args.IsInjected() ? _T("yes") : _T("no"));
        }

        switch(args.GetMessage())
        {
        case WM_LBUTTONDOWN:
            m_LButtonDownEvent(args);
            break;
        case WM_LBUTTONUP:
            m_LButtonUpEvent(args);
            break;
        case WM_RBUTTONDOWN:
            m_RButtonDownEvent(args);
            break;
        case WM_RBUTTONUP:
            m_RButtonUpEvent(args);
            break;
        case WM_MBUTTONDOWN:
            m_MButtonDownEvent(args);
            break;
        case WM_MBUTTONUP:
            m_MButtonUpEvent(args);
            break;
        case WM_MOUSEWHEEL:
            m_WheelEvent(args);
            break;
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
        case WM_MOUSEHWHEEL:
            m_HWheelEvent(args);
            break;
#endif
        case WM_MOUSEMOVE:
            m_MouseMoveEvent(args);
            break;
        default:
            break;
        }
    }
    catch(std::exception& e)
    {
        LOG_ERROR(_T("Exception caught during event dispatching: %s"), e.what());
    }
}
