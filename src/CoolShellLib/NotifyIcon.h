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
#include <boost/signals2.hpp>
#include <boost/optional.hpp>

/**
 * \brief Represents an icon into the system notification area
 *
 * \class NotifyIcon
 *      Wraps the Windows shell Api functions to display a notify icon into the system notification area.
 *      WM_xxx messages are exposed through events.
 *      Icons are automatically recreated in the case of task bar recreation.
 *      Menu is displayed automatically when set.
 */
class NotifyIcon : private boost::noncopyable
{
public:
    typedef boost::signals2::signal<void (NotifyIcon&, UINT, UINT, UINT)> NotifyIconEvent;
    typedef boost::signals2::signal<void (NotifyIcon&, UINT)> MenuItemSelectedEvent;

    static UINT WM_NOTIFYICON_MSG;      /**< registered window message for notify icon notification */
    static UINT WM_TASKBAR_CREATED;     /**< registered window message for system wide task bar creation notification */

protected:
    static UINT s_IDGen;
    static boost::optional<UINT> s_notifyIconVersion;
    static void SetNotifyIconVersion();

public:
    NotifyIcon();
    ~NotifyIcon();

    BOOL Create(HWND hWndParent, HICON hIcon, LPCTSTR szToolTip);
    BOOL Destroy();
    LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    BOOL SetIcon(HICON hIcon);
    BOOL SetToolTip(LPCTSTR szToolTip);
    BOOL ShowBalloonTip(LPCTSTR szTitle, LPCTSTR szText, UINT uFlags, UINT uTimeout, HICON hUserIcon);
    BOOL HideBalloonTip();

    void SetMenu(HMENU menu) { m_hMenu = menu; }
    HWND GetOwnerWindow() const { return m_hOwnerWnd; }
    
    NotifyIconEvent& GetNotifyIconEvent()            { return m_notifyIconEvent; }
    MenuItemSelectedEvent& GetMenuItemSelected()     { return m_menuItemSelectedEvent; }

protected:
    void Recreate() const;
    void HandleWindowsMsg(UINT uMsg, UINT xAnchor, UINT yAnchor);

private:
    UINT m_nID;                                      /**< unique identifier of the icon */
    HWND m_hOwnerWnd;                                /**< handle to the window that owns the icon */
    HICON m_hIcon;                                   /**< handle to the icon */
    CString m_toolTip;                               /**< tooltip text */
    HMENU m_hMenu;                                   /**< handle to the attached icon context menu */
    MenuItemSelectedEvent m_menuItemSelectedEvent;   /**< event fired when a context menu item is selected */
    NotifyIconEvent m_notifyIconEvent;               /**< event fired a Windows notification is received */
};
