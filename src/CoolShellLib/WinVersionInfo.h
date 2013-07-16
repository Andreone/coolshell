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

/**
 * \class WindowsVersionInfo
 * Determines the current running Windows version.
 *		  
 * Gives detailed information about the running windows version:
 *  - windows version 3.1, 95, 98, NT3.51, NT4.0, 2000, 2003, XP, Vista, ...
 *	- major version number (if version is 5.1, then 5)
 *	- minor version number (if version is 5.1, then 1)
 *	- build number
 *	- installed service pack string (like "Service Pack 6")
 *	- installed service pack major version number
 *	- installed service pack version number
 */

class WinVersionInfo
{
public:
    enum winver
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

    WinVersionInfo();
    ~WinVersionInfo();

    winver GetWindowsVersion() const;
    LPCTSTR GetWindowsVersionStr() const;

    UINT GetMajorVersion() const;
    UINT GetMinorVersion() const;
    UINT GetBuildNumber() const;

    LPCTSTR GetServicePack() const;
    UINT GetMajorSPVersion() const;
    UINT GetMinorSPVersion() const;

private:
    void LoadInfo();

    OSVERSIONINFOEX m_vi;	/**< WIN32 API structure that contains operating system version information */
    winver m_winver;		/**< enumerated value representing the current operating system */
};
