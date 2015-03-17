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
#include "MediaRemoteService.h"

#include "CoolShellLib\WinApi.h"
#include "CoolShellLib\Logging.h"
#include "CoolShellLib\Mouse.h"

#include "CoolShellConfiguration.h"

MediaRemoteService::MediaRemoteService(std::shared_ptr<IMouseEventDispatcher> mouseEventDispatcher) :
    m_mouseEventDispatcher(mouseEventDispatcher),
    m_mediaRemote(),
    m_wndClasses()
{
}

void MediaRemoteService::Initialize(const MediaRemoteServiceConfiguration& configuration)
{
    if(!configuration.enabled)
        return;

    m_mouseEventDispatcher->WheelEvent().connect(std::bind(&MediaRemoteService::OnMouseEvent, this, std::placeholders::_1));
    m_mouseEventDispatcher->XButtonDownEvent().connect(std::bind(&MediaRemoteService::OnMouseEvent, this, std::placeholders::_1));
    m_mouseEventDispatcher->MButtonDownEvent().connect(std::bind(&MediaRemoteService::OnMouseEvent, this, std::placeholders::_1));

    m_wndClasses = configuration.windowClassToHandle;
}


bool MediaRemoteService::IsWindowToHandle(const CString& s) const
{
    return m_wndClasses.end() != std::find(m_wndClasses.begin(), m_wndClasses.end(), s);
}

void MediaRemoteService::OnMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args)
{
    HWND hWnd = ::WindowFromPoint(args.GetPoint());
    if(!::IsWindow(hWnd))
        return;

    CString windowClassName = WinApi::GetWindowClassName(WinApi::GetRootWindow(hWnd));
    LOG_INFO(_T("MediaRemoteService - Window class under cursor is '%s'"), (LPCTSTR)windowClassName);

    if (m_wndClasses.end() == std::find(m_wndClasses.begin(), m_wndClasses.end(), windowClassName))
    {
        return;
    }

    switch (args.GetMessage())
    {
    case WM_MBUTTONDOWN:
        m_mediaRemote.PlayPause();
        args.SetHandled(true);
        LOG_INFO(_T("MediaRemoteService - PlayPause"));
        break;

    case WM_MOUSEWHEEL:
        if (args.GetWheelDelta() > 0)
        {
            LOG_INFO(_T("MediaRemoteService - VolumeUp"));
            m_mediaRemote.VolumeUp();
        }
        else
        {
            m_mediaRemote.VolumeDown();
            LOG_INFO(_T("MediaRemoteService - VolumeDown"));
        }
        args.SetHandled(true);
        break;

    case WM_XBUTTONDOWN:
        if (args.GetXButton() == XBUTTON2)
        {
            m_mediaRemote.NextTrack();
            LOG_INFO(_T("MediaRemoteService - NextTrack"));
        }
        else
        {
            m_mediaRemote.PreviousTrack();
            LOG_INFO(_T("MediaRemoteService - PreviousTrack"));
        }
        args.SetHandled(true);
        break;

    default:
        break;
    }
}
