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

#include "boost/noncopyable.hpp"
#include "boost/signal.hpp"
#include "boost/optional.hpp"
#include <map>

/**
 * \brief Represents an icon into the system notification area
 *
 * \class NotifyIcon
 *      Wraps the Windows shell Api functions to display a notify icon into the system notification area.
 *      WM_xxx messages are exposed through events.
 *      Icons are automatically recreated in the case of task bar recreation.
 */
class NotifyIcon : private boost::noncopyable
{
public:
    typedef boost::signal<void (NotifyIcon&, UINT, UINT, UINT)> NotifyIconEvent;
    typedef boost::signal<void (NotifyIcon&, UINT)> MenuItemSelectedEvent;

protected:
    static UINT WM_NOTIFYICON_MSG;                           /**< registered window message for notify icon notification */
    static UINT WM_TASKBAR_CREATED;                          /**< registered window message for system wide task bar creation notification */
    class NotifyIconWnd;
    static NotifyIconWnd s_internalWnd;                      /**< a window to process Windows messages */
    static UINT s_IDGen;                                     /**< unique identifier generator */
    static std::map<UINT, NotifyIcon*> s_allIcons;           /**< map of created instance (e.g. installed in the tray) */
    static boost::optional<UINT> s_notifyIconVersion;

    static UINT GetNextID();
    static HWND GetPrivateWindow();
    static void SetNotifyIconVersion();

public:
    NotifyIcon();
    virtual ~NotifyIcon();

    virtual BOOL Create(HICON hIcon, LPCTSTR szToolTip);
    virtual BOOL Destroy();

    BOOL SetIcon(HICON hIcon);
    BOOL SetToolTip(LPCTSTR szToolTip);
    BOOL ShowBalloonTip(LPCTSTR szTitle, LPCTSTR szText, UINT uFlags, UINT uTimeout, HICON hUserIcon);
    BOOL HideBalloonTip();

    void SetMenu(HMENU menu) { m_hMenu = menu; }
    HMENU GetMenu() const    { return m_hMenu; }
    HWND GetOwnerWindow() const { return m_hOwnerWnd; }
    
    NotifyIconEvent& OnNotifyIconEvent()            { return m_notifyIconEvent; }
    MenuItemSelectedEvent& OnMenuItemSelected()     { return m_menuItemSelectedEvent; }

protected:
    virtual void Recreate() const;
    virtual void HandleWindowsMsg(UINT notifyIconId, UINT notifyIconMsg, UINT xAnchor, UINT yAnchor);

private:
    UINT m_nID;                                      /**< unique identifier of the icon */
    HWND m_hOwnerWnd;                                /**< handle to the window that owns the icon */
    HICON m_hIcon;                                   /**< handle to the icon */
    CString m_toolTip;                               /**< tooltip text */
    HMENU m_hMenu;                                   /**< handle to the attached icon context menu */
    MenuItemSelectedEvent m_menuItemSelectedEvent;   /**< event fired when a context menu item is selected */
    NotifyIconEvent m_notifyIconEvent;               /**< event fired a Windows notification is received */
};
