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


LPCTSTR WinVersionInfo::ToString(WinVersion version)
{
    switch (version)
    {
    case WinVersion::win_31:
        return _T("Windows 3.1");
    case WinVersion::win_95:
        return _T("Windows 95");
    case WinVersion::win_98:
        return _T("Windows 98");
    case WinVersion::win_nt351:
        return _T("Windows NT3.51");
    case WinVersion::win_nt4:
        return _T("Windows NT4");
    case WinVersion::win_2000:
        return _T("Windows Server 2000");
    case WinVersion::win_xp:
        return _T("Windows XP");
    case WinVersion::win_2003:
        return _T("Windows Server 2003");
    case WinVersion::win_2003_r2:
        return _T("Windows Server 2003 R2");
    case WinVersion::win_vista:
        return _T("Windows Vista");
    case WinVersion::win_2008:
        return _T("Windows Server 2008");
    case WinVersion::win_2008_r2:
        return _T("Windows Server 2008 R2");
    case WinVersion::win_7:
        return _T("Windows 7");
    case WinVersion::win_8:
        return _T("Windows 8");
    case WinVersion::win_unknown:
    default:
        return _T("Unknown Windows version");
    }
}

WinVersionInfo::WinVersionInfo() :
    m_vi(),
    m_winver(WinVersion::win_unknown)
{
    LoadInfo();
}


void WinVersionInfo::LoadInfo()
{
    memset(&m_vi, 0, sizeof(OSVERSIONINFOEX));
    m_vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (::GetVersionEx((LPOSVERSIONINFO)&m_vi))
    {
        switch (m_vi.dwPlatformId)
        {
        case VER_PLATFORM_WIN32s:
            m_winver = WinVersion::win_31;
            break;

        case VER_PLATFORM_WIN32_WINDOWS:
            m_winver = (m_vi.dwMinorVersion > 0 ? WinVersion::win_98 : WinVersion::win_95);
            break;

        case VER_PLATFORM_WIN32_NT:
            switch (m_vi.dwMajorVersion)
            {
            case 3:
                m_winver = WinVersion::win_nt351;
                break;
            case 4:
                m_winver = WinVersion::win_nt4;
                break;
            case 5:
                switch (m_vi.dwMinorVersion)
                {
                case 0:
                    m_winver = WinVersion::win_2000;
                    break;
                case 1:
                    m_winver = WinVersion::win_xp;
                    break;
                case 2:
                    m_winver = (::GetSystemMetrics(SM_SERVERR2) == 0 ? WinVersion::win_2003 : WinVersion::win_2003_r2);
                    break;
                }
                break;
            case 6:
                switch (m_vi.dwMajorVersion)
                {
                case 0:
                    m_winver = (m_vi.wProductType == VER_NT_WORKSTATION ? WinVersion::win_vista : WinVersion::win_2008);
                    break;
                case 1:
                    m_winver = (m_vi.wProductType == VER_NT_WORKSTATION ? WinVersion::win_7 : WinVersion::win_2008_r2);
                    break;
                case 2:
                    m_winver = WinVersion::win_8;
                    break;
                }
                break;
            }
            break;
        }
    }
}
