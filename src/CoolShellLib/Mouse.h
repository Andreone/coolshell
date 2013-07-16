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

/**
 * Mouse API helpers
 */
namespace Mouse
{
    struct Mousestroke
    {
        Mousestroke() {}
        Mousestroke(int vk, std::vector<int> vkModifiers) :
            vkButton(vk),
            modifierKeys(vkModifiers)
        {
        }

        int vkButton;
        std::vector<int> modifierKeys;
    };

    enum EventType
    {
        LeftButtonDown,
        LeftButtonUp,
        RightButtonDown,
        RightButtonUp,
        MiddleButtonDown,
        MiddleButtonUp,
        WheelDown,
        WheelUp,
        HWheelRight,
        HWheelLeft,
        XButton1Down,
        XButton1Up,
        XButton2Down,
        XButton2Up
    };
    DWORD ConvertEventTypeToApiCode(const EventType& event);

    enum CoordinateMode
    {
        Relative    = 0,
        Absolute    = MOUSEEVENTF_ABSOLUTE,
        VirtualDesk = MOUSEEVENTF_VIRTUALDESK
    };

    UINT Move(int x, int y, CoordinateMode mode);
    UINT SendEvent(EventType event, int x, int y, CoordinateMode mode, UINT count = 1);
    void SendDownUpEvent(int vkButton, int x, int y, CoordinateMode mode, UINT count = 1);
    CPoint GetCursorPos();
} // namespace Mouse