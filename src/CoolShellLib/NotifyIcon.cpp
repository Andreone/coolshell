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
#include "NotifyIcon.h"

#include <ShellAPI.h> // Windows Shell functions
#include "WinVersionInfo.h"

// simply define the symbol but it won't be used unless the OS is Vista or later
#ifndef NOTIFYICON_VERSION_4
#define NOTIFYICON_VERSION_4 4
#endif

#define DECLARE_NOFIFYICONDATA(__nid) \
    NOTIFYICONDATA __nid; \
    memset(&__nid, 0, sizeof(NOTIFYICONDATA)); \
    __nid.cbSize = sizeof(NOTIFYICONDATA);

#define DECLARE_NOFIFYICONDATA_2(__nid, __ID, __HWND) \
    NOTIFYICONDATA __nid; \
    memset(&__nid, 0, sizeof(NOTIFYICONDATA)); \
    __nid.cbSize = sizeof(NOTIFYICONDATA); \
    nid.uID = __ID; \
    nid.hWnd = __HWND

//////////////////////////////////////////////////////////////////////////

UINT NotifyIcon::WM_NOTIFYICON_MSG = ::RegisterWindowMessage(_T("{A4D2B94F-96A4-47a6-BABD-5059D2E3A44F}"));
UINT NotifyIcon::WM_TASKBAR_CREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));
UINT NotifyIcon::s_IDGen = 0;
boost::optional<UINT> NotifyIcon::s_notifyIconVersion;

void NotifyIcon::SetNotifyIconVersion()
{
    WinVersionInfo info;
    if (info.GetWindowsVersion() >= WinVersionInfo::WinVersion::win_vista)
        s_notifyIconVersion = NOTIFYICON_VERSION_4;
    else if (info.GetWindowsVersion() >= WinVersionInfo::WinVersion::win_2000)
        s_notifyIconVersion = NOTIFYICON_VERSION;
    else
        s_notifyIconVersion = 0;

    ASSERT(s_notifyIconVersion.is_initialized());
}

//////////////////////////////////////////////////////////////////////////

NotifyIcon::NotifyIcon() :
    m_id(0),
    m_hOwnerWnd(NULL),
    m_hIcon(NULL),
    m_toolTip(),
    m_notifyMouseEvent()
{
    if(!s_notifyIconVersion.is_initialized())
        SetNotifyIconVersion();
}

NotifyIcon::~NotifyIcon()
{
    try
    {
        Destroy();
    }
    catch(...)
    { }
}

/**
 * \brief Creates (e.g. display) the notify icon.
 *
 * \param hIcon [in] : a handle to the icon to be displayed.
 * \param szToolTip [in] : null-terminated string that specifies the text of the tooltip
 * \return Returns TRUE if successful, or FALSE otherwise
 */

BOOL NotifyIcon::Create(HWND hWndParent, HICON hIcon, LPCTSTR szToolTip)
{
    if(m_id)
        throw std::logic_error("The NotifyIcon is already created");

    DECLARE_NOFIFYICONDATA(nid);

    nid.uID = ++s_IDGen;
    nid.hWnd = hWndParent;
    nid.hIcon = hIcon;
    nid.uCallbackMessage = WM_NOTIFYICON_MSG;
    nid.uFlags = NIF_ICON | NIF_MESSAGE;
    if (szToolTip)
    {
        nid.uFlags |= NIF_TIP;
        lstrcpyn(nid.szTip, szToolTip, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
    }

    if(!::Shell_NotifyIcon(NIM_ADD, &nid))
    {
        TRACE(_T("Failed to add icon #%d\n"), m_id);
        return FALSE;
    }

    nid.uVersion = s_notifyIconVersion.get();
    if(!::Shell_NotifyIcon(NIM_SETVERSION, &nid))
        TRACE(_T("NIM_SETVERSION failed for icon #%d\n"), m_id);

    m_hOwnerWnd = hWndParent;
    m_id = nid.uID;
    m_hIcon = nid.hIcon;
    m_toolTip = nid.szTip;

    TRACE(_T("Icon #%d added\n"), m_id);
    return TRUE;
}


BOOL NotifyIcon::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == NotifyIcon::WM_NOTIFYICON_MSG)
    {
        UINT notifyIconId = ((*s_notifyIconVersion > NOTIFYICON_VERSION) ? HIWORD(lParam) : static_cast<UINT>(wParam));

        if (notifyIconId == m_id)
        {
            UINT xAnchor = ((*s_notifyIconVersion > NOTIFYICON_VERSION) ? GET_X_LPARAM(lParam) : 0);
            UINT yAnchor = ((*s_notifyIconVersion > NOTIFYICON_VERSION) ? GET_Y_LPARAM(lParam) : 0);
            UINT msg = ((*s_notifyIconVersion > NOTIFYICON_VERSION) ? LOWORD(lParam) : static_cast<UINT>(lParam));

            m_notifyMouseEvent(*this, msg, xAnchor, yAnchor);
            return TRUE;
        }
    }
    else if (uMsg == NotifyIcon::WM_TASKBAR_CREATED)
    {
        TRACE(_T("WM_TASKBAR_CREATED received\n"));
        Recreate();
    }

    return FALSE;
}

/**
 * \brief Recreates the icon. Useful when WM_TASKBAR_CREATED is received.
 */

void NotifyIcon::Recreate() const
{
    ASSERT(m_id);

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    nid.hIcon = m_hIcon;
    nid.uCallbackMessage = WM_NOTIFYICON_MSG;
    nid.uFlags = NIF_ICON | NIF_MESSAGE;
    if (!m_toolTip.IsEmpty())
    {
        nid.uFlags |= NIF_TIP;
        lstrcpyn(nid.szTip, (LPCTSTR)m_toolTip, m_toolTip.GetLength() / sizeof(TCHAR));
    }

    if(::Shell_NotifyIcon(NIM_ADD, &nid))
        TRACE(_T("Icon #%d recreated\n"), m_id);
    else
        TRACE(_T("Failed to recreate icon #%d\n"), m_id);
}

/**
 * \brief Destroys the notify icon and removes it from the notification area.
 * \return Returns TRUE if successful, or FALSE otherwise
 */
BOOL NotifyIcon::Destroy()
{
    if (!m_id)
        return FALSE;

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    
    BOOL success = ::Shell_NotifyIcon(NIM_DELETE, &nid);
    if(success)
    {
        TRACE(_T("Icon #%d deleted\n"), m_id);
        m_id = 0;
    }
    else
        TRACE(_T("Failed to delete icon #%d\n"), m_id);
    return success;
}

/**
 * \brief Changes the icon of the notify icon.
 *      The notify icon must be created first, otherwise the method does nothing.
 * \param hIcon [in] : a handle to the icon to be displayed.
 * \return Returns TRUE if successful, or FALSE otherwise
 */
BOOL NotifyIcon::SetIcon(HICON hIcon)
{
    if (!m_id)
        throw std::logic_error("The NotifyIcon is not created");

    m_hIcon = hIcon;

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    nid.hIcon = m_hIcon;
    nid.uFlags = NIF_ICON;

    BOOL success = ::Shell_NotifyIcon(NIM_MODIFY, &nid);
    if(success)
        TRACE(_T("Icon #%d modified\n"), m_id);
    else
        TRACE(_T("Failed to modify icon #%d\n"), m_id);
    return success;
}

/**
 * \brief Changes the tooltip text of the notify icon.
 *      The notify icon must be created first, otherwise the method does nothing.
 * \param szToolTip [in] : null-terminated string that specifies the text of the tooltip
 * \return Returns TRUE if successful, or FALSE otherwise
 */
BOOL NotifyIcon::SetToolTip(LPCTSTR szToolTip)
{
    if (!m_id)
        throw std::logic_error("The NotifyIcon is not created");

    m_toolTip = szToolTip;

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    lstrcpyn(nid.szTip, (LPCTSTR)m_toolTip, m_toolTip.GetLength() / sizeof(TCHAR));
    nid.uFlags = NIF_TIP;

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

/**
 * \brief Displays a balloon tip.
 *
 * \param szTitle [in] : null-terminated string that specifies the title of the balloon tip
 * \param szText [in] : null-terminated string that specifies the text of the balloon tip
 * \param uFlags [in] : shell api flags (NIIF_xxx) see MSDN for possible values
 * \param uTimeout [in] : Timeout in milliseconds to hide automatically the balloon tip.
 *          The system enforces minimum and maximum values [10s, 30s].
 * \param hUserIcon [in] : When uFlags contains NIIF_USER, the method uses this parameter as the icon
 *          displayed into the balloon tip
 * \return Returns TRUE if successful, or FALSE otherwise
 */

BOOL NotifyIcon::ShowBalloonTip(LPCTSTR szTitle, LPCTSTR szText, UINT uFlags, UINT uTimeout, HICON hUserIcon)
{
    if (!m_id)
        throw std::logic_error("The NotifyIcon is not created");

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = uFlags;
    nid.uTimeout = uTimeout;
    if (uFlags & NIIF_USER)
        nid.hIcon = hUserIcon;
    lstrcpyn(nid.szInfo, szText, sizeof(nid.szInfo) / sizeof(nid.szInfo[0]));
    lstrcpyn(nid.szInfoTitle, szTitle, sizeof(nid.szInfoTitle) / sizeof(nid.szInfoTitle[0]));

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}

/**
 * \brief Hides the balloon tip of the notify icon.
 *      There's no error if no balloon tip is currently displayed.
 *      The notify icon must be created first, otherwise the method does nothing.
 * \return Returns TRUE if successful, or FALSE otherwise
 */
BOOL NotifyIcon::HideBalloonTip()
{
    if (!m_id)
        throw std::logic_error("The NotifyIcon is not created");

    DECLARE_NOFIFYICONDATA_2(nid, m_id, m_hOwnerWnd);
    nid.uFlags = NIF_INFO;
    lstrcpyn(nid.szInfo, _T(""), sizeof(nid.szInfo) / sizeof(nid.szInfo[0]));

    return ::Shell_NotifyIcon(NIM_MODIFY, &nid);
}
