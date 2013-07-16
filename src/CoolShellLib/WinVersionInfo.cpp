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
#include "WinVersionInfo.h"

/**
 * \brief Default class constructor.
 */
WinVersionInfo::WinVersionInfo() :
    m_vi(),
    m_winver(win_unknown)
{
    LoadInfo();
}

/**
 * \brief class destructor.
 */
WinVersionInfo::~WinVersionInfo()
{
}

/**
 * \brief Loads the version of the current running Windows information.
 */
void WinVersionInfo::LoadInfo()
{
    memset(&m_vi, 0, sizeof(OSVERSIONINFOEX));
    m_vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (::GetVersionEx((LPOSVERSIONINFO)&m_vi))
    {
        switch (m_vi.dwPlatformId)
        {
        case VER_PLATFORM_WIN32s:
            m_winver = win_31;
            break;

        case VER_PLATFORM_WIN32_WINDOWS:
            m_winver = (m_vi.dwMinorVersion > 0 ? win_98 : win_95);
            break;

        case VER_PLATFORM_WIN32_NT:
            switch (m_vi.dwMajorVersion)
            {
            case 3:
                m_winver = win_nt351;
                break;
            case 4:
                m_winver = win_nt4;
                break;
            case 5:
                switch (m_vi.dwMinorVersion)
                {
                case 0:
                    m_winver = win_2000;
                    break;
                case 1:
                    m_winver = win_xp;
                    break;
                case 2:
                    m_winver = (::GetSystemMetrics(SM_SERVERR2) == 0 ? win_2003 : win_2003_r2);
                    break;
                }
                break;
            case 6:
                switch (m_vi.dwMajorVersion)
                {
                case 0:
                    m_winver = (m_vi.wProductType == VER_NT_WORKSTATION ? win_vista : win_2008);
                    break;
                case 1:
                    m_winver = (m_vi.wProductType == VER_NT_WORKSTATION ? win_7 : win_2008_r2);
                    break;
                case 2:
                    m_winver = win_8;
                    break;
                }
                break;
            }
            break;
        }
    }
}

/**
 * \brief Returns a string representation of the current windows operating system.
 * \return a null-terminated string
 */
LPCTSTR WinVersionInfo::GetWindowsVersionStr() const
{
    switch (m_winver)
    {
    case win_31:
        return _T("Windows 3.1");
    case win_95:
        return _T("Windows 95");
    case win_98:
        return _T("Windows 98");
    case win_nt351:
        return _T("Windows NT3.51");
    case win_nt4:
        return _T("Windows NT4");
    case win_2000:
        return _T("Windows Server 2000");
    case win_xp:
        return _T("Windows XP");
    case win_2003:
        return _T("Windows Server 2003");
    case win_2003_r2:
        return _T("Windows Server 2003 R2");
    case win_vista:
        return _T("Windows Vista");
    case win_2008:
        return _T("Windows Server 2008");
    case win_2008_r2:
        return _T("Windows Server 2008 R2");
    case win_7:
        return _T("Windows 7");
    case win_8:
        return _T("Windows 8");
    case win_unknown:
    default:
        return _T("Unknown Windows version");
    }
}

/**
 * \brief Returns the enumerated value of the windows version.
 * \return the enumerated value of the windows version
 */
WinVersionInfo::winver WinVersionInfo::GetWindowsVersion() const
{
    return m_winver;
}

/**
 * \brief Returns the current windows major version number.
 * \return [UINT] - the windows major version number
 */
UINT WinVersionInfo::GetMajorVersion() const
{
    return m_vi.dwMajorVersion;
}

/**
 * \brief Returns the current windows minor version number.
 * \return [UINT] - the windows minor version number
 */
UINT WinVersionInfo::GetMinorVersion() const
{
    return m_vi.dwMinorVersion;
}

/**
 * \brief Returns the current windows build number.
 * \return [UINT] - the windows build number
 */
UINT WinVersionInfo::GetBuildNumber() const
{
    return m_vi.dwBuildNumber;
}

/**
 * \brief Returns the current installed windows service pack, such as "Service Pack #x".
 * \return [LPCTSTR] - null-terminated string that represents the service pack label
 */
LPCTSTR WinVersionInfo::GetServicePack() const
{
    return m_vi.szCSDVersion;
}

/**
 * \brief Returns the current windows service pack major version number.
 * \return [UINT] - the service pack major version number
 */
UINT WinVersionInfo::GetMajorSPVersion() const
{
    return m_vi.wServicePackMajor;
}

/**
 * \brief Returns the current windows service pack minor version number.
 * \return [UINT] - the service pack minor version number
 */
UINT WinVersionInfo::GetMinorSPVersion() const
{
    return m_vi.wServicePackMinor;
}
