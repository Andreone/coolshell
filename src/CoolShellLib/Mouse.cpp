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
#include "Mouse.h"

namespace Mouse
{

UINT Move(int x, int y, CoordinateMode mode)
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));

    input.type = INPUT_MOUSE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.dwFlags |= mode;

    return SendInput(1, &input, sizeof(INPUT));
}


UINT SendEvent(EventType eventType, int x, int y, CoordinateMode mode, UINT count /*= 1*/)
{
    INPUT input;
    memset(&input, 0, sizeof(INPUT));

    input.type = INPUT_MOUSE;
    input.mi.dx = x;
    input.mi.dy = y;
    input.mi.dwFlags = ConvertEventTypeToApiCode(eventType);
    input.mi.dwFlags |= mode;

    if(eventType == WheelUp || eventType == HWheelRight)
        input.mi.mouseData = WHEEL_DELTA;
    else if(eventType == WheelDown || eventType == HWheelLeft)
        input.mi.mouseData = (DWORD)(WHEEL_DELTA * -1);
    else if(eventType == XButton1Down || eventType == XButton1Up)
        input.mi.mouseData = XBUTTON1;
    else if(eventType == XButton2Down || eventType == XButton2Up)
        input.mi.mouseData = XBUTTON2;

    return ::SendInput(count, &input, sizeof(INPUT));
}

void SendDownUpEvent(int vkButton, int x, int y, CoordinateMode mode, UINT count /*= 1*/)
{
   switch(vkButton)
    {
    case VK_MBUTTON:
        Mouse::SendEvent(MiddleButtonDown, x, y, Mouse::Absolute);
        Mouse::SendEvent(MiddleButtonUp, x, y, Mouse::Absolute);
        break;
    case VK_RBUTTON:
        Mouse::SendEvent(RightButtonDown, x, y, Mouse::Absolute);
        Mouse::SendEvent(RightButtonUp, x, y, Mouse::Absolute);
        break;
    case VK_LBUTTON:
        Mouse::SendEvent(LeftButtonDown, x, y, Mouse::Absolute);
        Mouse::SendEvent(LeftButtonUp, x, y, Mouse::Absolute);
        break;
    case VK_XBUTTON1:
        Mouse::SendEvent(XButton1Down, x, y, Mouse::Absolute);
        Mouse::SendEvent(XButton1Up, x, y, Mouse::Absolute);
        break;
    case VK_XBUTTON2:
        Mouse::SendEvent(XButton2Down, x, y, Mouse::Absolute);
        Mouse::SendEvent(XButton2Up, x, y, Mouse::Absolute);
        break;
    default:
        break;
    }
}

CPoint GetCursorPos()
{
    CPoint p;
    ::GetCursorPos((LPPOINT)p);
    return p;
}

DWORD ConvertEventTypeToApiCode( const EventType& event )
{
    switch(event)
    {
    case LeftButtonDown:
        return MOUSEEVENTF_LEFTDOWN;
    case LeftButtonUp:
        return MOUSEEVENTF_LEFTUP;
    case RightButtonDown:
        return MOUSEEVENTF_RIGHTDOWN;
    case RightButtonUp:
        return MOUSEEVENTF_RIGHTUP;
    case MiddleButtonDown:
        return MOUSEEVENTF_MIDDLEDOWN;
    case MiddleButtonUp:
        return MOUSEEVENTF_MIDDLEUP;
    case WheelDown:
    case WheelUp:
        return MOUSEEVENTF_WHEEL;
    case XButton1Down:
    case XButton2Down:
        return MOUSEEVENTF_XDOWN;
    case XButton1Up:
    case XButton2Up:
        return MOUSEEVENTF_XUP;
    default:
        throw std::runtime_error("Bad mouse event type");
    }
}

} // namespace Mouse