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
#include "WheelUnderCursor.h"

#include <boost/bind.hpp>
#include "CoolShellLib\WinApi.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\Mouse.h"

WheelUnderCursor::WheelUnderCursor() :
    WindowClassFilter()
{
}

WheelUnderCursor::~WheelUnderCursor()
{
    LOG_TRACE(_T("Deleted WheelUnderCursor"));
}

void WheelUnderCursor::Setup(IMouseEventDispatcher& dispatcher)
{
    dispatcher.WheelEvent().connect(boost::bind(&WheelUnderCursor::OnWheel, this, _1));
    dispatcher.HWheelEvent().connect(boost::bind(&WheelUnderCursor::OnWheel, this, _1));
}

void WheelUnderCursor::OnWheel(WindowsHooks::LowLevelMouseEventArgs& args)
{
    HWND hWnd = ::WindowFromPoint(args.GetPoint());
    if(!::IsWindow(hWnd))
        return;

    if(IsWindowClassFiltered(WinApi::GetRootWindow(hWnd)))
        return;

    CPoint mousePos = Mouse::GetCursorPos();

    short delta = (short)HIWORD(args.GetMouseData()) > 0 ? 1 : -1;
    if(::PostMessage(hWnd, args.GetMessage(), WHEEL_DELTA * delta << 16, MAKELPARAM(mousePos.x, mousePos.y)))
    {
        LOG_INFO(_T("Posted wheel message at (%d,%d)"), mousePos.x, mousePos.y);
        args.SetHandled(true);
    }
    else
    {
        LOG_ERROR(_T("Failed to post mouse wheel: %s"), (LPCTSTR)WinApi::GetLastErrorMessage());
    }
}

