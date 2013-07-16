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

#include "IMouseModule.h"
#include "boost/noncopyable.hpp"
#include "WindowClassFilter.h"

#include "CoolShellLib\timers.h"
#include "CoolShellLib\Mouse.h"
#include "CoolShellLib\Keyboard.h"

class DragWindowManager : public WindowClassFilter
{
public:
    DragWindowManager();
    virtual ~DragWindowManager();

    void OnButtonDown(WindowsHooks::LowLevelMouseEventArgs& args);
    void OnButtonUp(WindowsHooks::LowLevelMouseEventArgs& args);
    
    void SetMousestroke(const Mouse::Mousestroke& mousestroke) { m_mousestroke = mousestroke; }

private:
    enum DragMode { NONE, MOVE, RESIZE };

    bool IsWindowToProcess(HWND hWnd) const;
    void Reset();
    void BeginDrag(HWND hWnd);
    bool ContinueDrag();
    void EndDrag();
    void CancelDrag();
    void DoDrag();

    void SetDragMode(DragMode mode) { m_dragMode = mode; }
    bool IsDragging() const { return m_dragMode != NONE; }

    HWND m_hWnd;
    CRect m_beginWndRect;
    POINT m_beginMousePos;
    POINT m_lastMousePos;
    DragMode m_dragMode;
    bool m_hasDragged;
    DWORD m_lastButtonDownTime;
    Mouse::Mousestroke m_mousestroke;
    timers::win_timer m_mouseTrackerTimer;
};
