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
#include "DragWindowManager.h"

#include "CoolShellLib\Keyboard.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\Mouse.h"
#include "CoolShellLib\WinApi.h"

#define TRACK_MOUSE_TIMER_TIME_MS   10
#define MOUSE_MOVE_THRESHOLD        1

DragWindowManager::DragWindowManager() :
    m_hWnd(NULL),
    m_beginWndRect(),
    m_beginMousePos(),
    m_lastMousePos(),
    m_hasDragged(false),
    m_dragMode(NONE),
    m_lastButtonDownTime(0),
    m_mousestroke(),
    m_mouseTrackerTimer()
{
}

DragWindowManager::~DragWindowManager()
{
}

bool DragWindowManager::IsWindowToProcess(HWND hWnd) const
{
    if(!::IsWindow(hWnd))
        return false;

    WINDOWPLACEMENT wndPlacement;
    if(::GetWindowPlacement(hWnd, &wndPlacement) && wndPlacement.showCmd == SW_SHOWMAXIMIZED)
        return false;

    auto cursorPosition = Mouse::GetCursorPos();
    // do not allow when over a title bar
    auto area = ::SendMessage(hWnd, WM_NCHITTEST, NULL, MAKELPARAM(cursorPosition.x, cursorPosition.y));
    if(area != HTCLIENT)
        return false;

    if(IsWindowClassFiltered(hWnd))
        return false;

    return true;
}

void DragWindowManager::OnButtonDown(WindowsHooks::LowLevelMouseEventArgs& args)
{
    if(args.IsInjected())
        return;

    LOG_DEBUG(_T("DragWindowManager::OnButtonDown"));

    if(!Keyboard::AreKeysDown(m_mousestroke.modifierKeys))
        return;

    HWND hWnd = WinApi::RootWindowFromPoint(args.GetPoint());
    if(!IsWindowToProcess(hWnd))
    {
        LOG_DEBUG(_T("Not allowed to drag this window."));
        return;
    }

    DWORD currentTime = ::GetTickCount();
    SetDragMode((currentTime - m_lastButtonDownTime <= ::GetDoubleClickTime()) ? RESIZE : MOVE);
    m_lastButtonDownTime = currentTime;

    BeginDrag(hWnd);

    args.SetHandled(true);
}

void DragWindowManager::OnButtonUp(WindowsHooks::LowLevelMouseEventArgs& args)
{
    if(args.IsInjected())
        return;

    LOG_DEBUG(_T("DragWindowManager::OnButtonUp"));

    if(IsDragging())
    {
        EndDrag();
        args.SetHandled(true);
    }
}

void DragWindowManager::BeginDrag(HWND hWnd)
{
    LOG_DEBUG(_T("DragWindowManager::BeginDrag"));

    m_hWnd = hWnd;
    ::GetCursorPos(&m_beginMousePos);
    ::GetCursorPos(&m_lastMousePos);

    m_beginWndRect = WinApi::GetWindowRect(hWnd);
    m_hasDragged = false;
    m_mouseTrackerTimer.start(TRACK_MOUSE_TIMER_TIME_MS, [this]() -> bool { return this->ContinueDrag(); } );
}


void DragWindowManager::EndDrag()
{
    LOG_DEBUG(_T("DragWindowManager::EndDrag"));

    if(!m_hasDragged)
    {
        Mouse::SendDownUpEvent(m_mousestroke.vkButton, m_beginMousePos.x, m_beginMousePos.y, Mouse::Absolute);
    }

    Reset();
}

void DragWindowManager::CancelDrag()
{
    LOG_DEBUG(_T("DragWindowManager::CancelDrag"));

    ::SetWindowPos(m_hWnd, NULL, 
        m_beginWndRect.left, 
        m_beginWndRect.top, 
        m_beginWndRect.Width(), 
        m_beginWndRect.Height(), 
        SWP_NOZORDER);

    LOG_INFO(_T("Drag canceled: window position reset to (x:%d,y:%d,cx:%d,cy%d)"), 
        m_beginWndRect.left, m_beginWndRect.top, m_beginWndRect.Width(), m_beginWndRect.Height());

    Reset();
}

bool DragWindowManager::ContinueDrag()
{
    if(!::IsWindow(m_hWnd))
    {
        EndDrag();
        return false;
    }

    if(Keyboard::IsKeyDown(VK_ESCAPE))
    {
        CancelDrag();
        return false;
    }

    if(Keyboard::AreKeysDown(m_mousestroke.modifierKeys))
        DoDrag();

    return true;
}

void DragWindowManager::Reset()
{
    LOG_DEBUG(_T("DragWindowManager::Reset"));

    m_mouseTrackerTimer.stop();
    m_hasDragged = false;
    SetDragMode(NONE);
    m_hWnd = NULL;
}

void DragWindowManager::DoDrag()
{
    CPoint mousePos = Mouse::GetCursorPos();

    // check the cursor has moved enough to trigger a position change
    if(abs(m_lastMousePos.x - mousePos.x) <= MOUSE_MOVE_THRESHOLD
        &&  abs(m_lastMousePos.y - mousePos.y) <= MOUSE_MOVE_THRESHOLD)
        return;

    if(m_dragMode == MOVE)
    {
        int x = m_beginWndRect.left + mousePos.x - m_beginMousePos.x;
        int y = m_beginWndRect.top + mousePos.y - m_beginMousePos.y;

        ::SetWindowPos(m_hWnd, NULL, x, y, 0, 0, SWP_NOSIZE|SWP_NOZORDER);
        LOG_INFO(_T("Moved window to (%d,%d)"), x, y);
    }
    else
    {
        int w = m_beginWndRect.right - m_beginWndRect.left + mousePos.x - m_beginMousePos.x;
        int h = m_beginWndRect.bottom - m_beginWndRect.top + mousePos.y - m_beginMousePos.y;

        ::SetWindowPos(m_hWnd, NULL, 0, 0, w, h, SWP_NOMOVE|SWP_NOZORDER);
        LOG_INFO(_T("Resized window to (%d,%d)"), w, h);
    }

    m_lastMousePos = mousePos;
    m_hasDragged = true;
}
