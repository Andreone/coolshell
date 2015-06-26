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

class WinVersionInfo
{
public:
    enum class WinVersion
    {
        win_unknown, 
        win_31, 
        win_95, 
        win_98, 
        win_nt351, 
        win_nt4, 
        win_2000,
        win_xp,
        win_2003, 
        win_2003_r2, 
        win_vista,
        win_2008, 
        win_2008_r2, 
        win_7,
        win_8
    };

    static LPCTSTR ToString(WinVersion version);

    WinVersionInfo();

    WinVersion GetWindowsVersion() const{ return m_winver; }

    UINT GetMajorVersion() const    { return m_vi.dwMajorVersion; }
    UINT GetMinorVersion() const    { return m_vi.dwMinorVersion; }
    UINT GetBuildNumber() const     { return m_vi.dwBuildNumber; }

    LPCTSTR GetServicePack() const  { return m_vi.szCSDVersion; }
    UINT GetMajorSPVersion() const  { return m_vi.wServicePackMajor; }
    UINT GetMinorSPVersion() const  { return m_vi.wServicePackMinor; }

private:
    void LoadInfo();

    OSVERSIONINFOEX m_vi;
    WinVersion m_winver;
};
