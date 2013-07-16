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

    // IMouseHookListener interface
    virtual void OnLowLevelMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args);

    // IMouseEventDispatcher interface
    virtual LowLevelMouseEvent& LButtonDownEvent()        { return m_LButtonDownEvent; }
    virtual LowLevelMouseEvent& LButtonUpEvent()          { return m_LButtonUpEvent; }
    virtual LowLevelMouseEvent& RButtonDownEvent()        { return m_RButtonDownEvent; }
    virtual LowLevelMouseEvent& RButtonUpEvent()          { return m_RButtonUpEvent; }
    virtual LowLevelMouseEvent& MButtonDownEvent()        { return m_MButtonDownEvent; }
    virtual LowLevelMouseEvent& MButtonUpEvent()          { return m_MButtonUpEvent; }
    virtual LowLevelMouseEvent& WheelEvent()              { return m_WheelEvent; }
    virtual LowLevelMouseEvent& HWheelEvent()             { return m_HWheelEvent; }
    virtual LowLevelMouseEvent& MouseMoveEvent()          { return m_MouseMoveEvent; }

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
