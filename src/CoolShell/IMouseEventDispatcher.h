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

#include <boost/signals2.hpp>
#include "CoolShellLib\WindowsHooks\LowLevelMouseEventArgs.h"

struct IMouseEventDispatcher
{
    typedef boost::signals2::signal<void (WindowsHooks::LowLevelMouseEventArgs&)> LowLevelMouseEvent;

    virtual LowLevelMouseEvent& MouseEvent() = 0;
    virtual LowLevelMouseEvent& LButtonDownEvent() = 0;
    virtual LowLevelMouseEvent& LButtonUpEvent() = 0;
    virtual LowLevelMouseEvent& RButtonDownEvent() = 0;
    virtual LowLevelMouseEvent& RButtonUpEvent() = 0;
    virtual LowLevelMouseEvent& MButtonDownEvent() = 0;
    virtual LowLevelMouseEvent& MButtonUpEvent() = 0;
    virtual LowLevelMouseEvent& XButtonDownEvent() = 0;
    virtual LowLevelMouseEvent& XButtonUpEvent() = 0;
    virtual LowLevelMouseEvent& WheelEvent() = 0;
    virtual LowLevelMouseEvent& HWheelEvent() = 0;
    virtual LowLevelMouseEvent& MouseMoveEvent() = 0;
};
