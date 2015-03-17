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

#include "CoolShellLib/Logging.h"
#include "CoolShellLib/WinApi.h"

MouseEventDispatcher::MouseEventDispatcher() :
    IMouseEventDispatcher(),
    m_mouseEvent(),
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

void MouseEventDispatcher::OnLowLevelMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args)
{
    try
    {
        if (args.GetMessage() != WM_MOUSEMOVE)
        {
            LOG_TRACE(_T("Received %s at (%d,%d) - Injected=%s"), 
                WinApi::GetWindowsMessageString(args.GetMessage()), 
                args.GetPoint().x, 
                args.GetPoint().y, 
                args.IsInjected() ? _T("yes") : _T("no"));
        }

        //if (!args.IsInjected())
        //{
        //	LOG_WARN("Ignored injected event");
        //	return;
        //}

        m_mouseEvent(args);
        if (args.IsHandled())
            return;

        Dispatch(args);
    }
    catch (std::exception& e)
    {
        LOG_ERROR(_T("Exception caught during event processing: %s"), e.what());
    }
}


void MouseEventDispatcher::Dispatch(WindowsHooks::LowLevelMouseEventArgs& args)
{
    switch (args.GetMessage())
    {
    case WM_LBUTTONDOWN:
        if (!args.IsInjected())
            m_LButtonDownEvent(args);
        break;
    case WM_LBUTTONUP:
        if (!args.IsInjected())
            m_LButtonUpEvent(args);
        break;
    case WM_RBUTTONDOWN:
        if (!args.IsInjected())
            m_RButtonDownEvent(args);
        break;
    case WM_RBUTTONUP:
        if (!args.IsInjected())
            m_RButtonUpEvent(args);
        break;
    case WM_MBUTTONDOWN:
        if (!args.IsInjected())
            m_MButtonDownEvent(args);
        break;
    case WM_MBUTTONUP:
        if (!args.IsInjected())
            m_MButtonUpEvent(args);
        break;
    case WM_XBUTTONDOWN:
        m_XButtonDownEvent(args);
        break;
    case WM_XBUTTONUP:
        m_XButtonUpEvent(args);
        break;
    case WM_MOUSEWHEEL:
        if (!args.IsInjected())
            m_WheelEvent(args);
        break;
//#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
    case WM_MOUSEHWHEEL:
        if (!args.IsInjected())
            m_HWheelEvent(args);
        break;
//#endif
    case WM_MOUSEMOVE:
        if (!args.IsInjected())
            m_MouseMoveEvent(args);
        break;
    default:
        break;
    }

}