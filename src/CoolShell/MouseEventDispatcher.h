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

#include <boost/noncopyable.hpp>

#include "IMouseEventDispatcher.h"
#include "CoolShellLib/WindowsHooks/LowLevelMouseEventArgs.h"

class MouseEventDispatcher : public IMouseEventDispatcher,
                             private boost::noncopyable
{
public:
    typedef IMouseEventDispatcher::LowLevelMouseEvent LowLevelMouseEvent;

    MouseEventDispatcher();
	virtual ~MouseEventDispatcher(); 

    void OnLowLevelMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args);

    // IMouseEventDispatcher interface
    LowLevelMouseEvent& LButtonDownEvent() override       { return m_LButtonDownEvent; }
    LowLevelMouseEvent& LButtonUpEvent() override         { return m_LButtonUpEvent; }
    LowLevelMouseEvent& RButtonDownEvent() override       { return m_RButtonDownEvent; }
    LowLevelMouseEvent& RButtonUpEvent() override         { return m_RButtonUpEvent; }
    LowLevelMouseEvent& MButtonDownEvent() override       { return m_MButtonDownEvent; }
    LowLevelMouseEvent& MButtonUpEvent() override         { return m_MButtonUpEvent; }
    LowLevelMouseEvent& WheelEvent() override             { return m_WheelEvent; }
    LowLevelMouseEvent& HWheelEvent() override            { return m_HWheelEvent; }
    LowLevelMouseEvent& MouseMoveEvent() override         { return m_MouseMoveEvent; }

private:
    LowLevelMouseEvent m_LButtonDownEvent;
    LowLevelMouseEvent m_LButtonUpEvent;
    LowLevelMouseEvent m_RButtonDownEvent;
    LowLevelMouseEvent m_RButtonUpEvent;
    LowLevelMouseEvent m_MButtonDownEvent;
    LowLevelMouseEvent m_MButtonUpEvent;
    LowLevelMouseEvent m_WheelEvent;
    LowLevelMouseEvent m_HWheelEvent;
    LowLevelMouseEvent m_MouseMoveEvent;
};
