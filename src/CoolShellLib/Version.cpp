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
#include "Version.h"

#include "boost/algorithm/string.hpp"
#include <string>
#include <vector>
#include <sstream>
#include "WinError.h"

//////////////////////////////////////////////////////////////////////////

#define MAX_VER_ITEMS	        4
#define VERSION_SEPARATOR_CHAR  _T('.')
#define VERSION_SEPARATOR_STR   _T(".")

//////////////////////////////////////////////////////////////////////////

Version Version::GetFileVersion(const CString& sFileName)
{
    DWORD dwZero = 0;
    DWORD dwFileVersionInfoSize = ::GetFileVersionInfoSize((LPTSTR)(LPCTSTR)sFileName, &dwZero);
    if (!dwFileVersionInfoSize)
        throw WinError();

    CString sBufferVersionInfo;
    TCHAR* pszBufferVersionInfo = sBufferVersionInfo.GetBuffer(dwFileVersionInfoSize);
    if (!::GetFileVersionInfo((LPTSTR)(LPCTSTR)sFileName, NULL, dwFileVersionInfoSize, pszBufferVersionInfo))
        throw WinError();
    sBufferVersionInfo.ReleaseBuffer();

    VS_FIXEDFILEINFO* pvsVersionInfo = nullptr;
    unsigned int nSize = 0;
    if (!::VerQueryValue(pszBufferVersionInfo, _T("\\"), (void**)&pvsVersionInfo, &nSize))
        throw WinError();

    // the first WORD of dwFileVersionMS is the major number of the most significant part of the version number
    WORD wMSMajor = (WORD) (pvsVersionInfo->dwFileVersionMS >> 16);
    // the second WORD of dwFileVersionMS is the minor number of the most significant part of the version number
    WORD  wMSMinor = (WORD) pvsVersionInfo->dwFileVersionMS;
    // idem for the less significant parts of the version number
    WORD wLSMajor = (WORD) (pvsVersionInfo->dwFileVersionLS >> 16);
    WORD wLSMinor = (WORD) pvsVersionInfo->dwFileVersionLS;

    return Version (wMSMajor, wMSMinor, wLSMajor, wLSMinor);
}

//////////////////////////////////////////////////////////////////////////

Version::Version(WORD wMSMajor, WORD wMSMinor, WORD wLSMajor, WORD wLSMinor)
{
    SetMSMajor(wMSMajor);
    SetMSMinor(wMSMinor);
    SetLSMajor(wLSMajor);
    SetLSMinor(wLSMinor);
}

Version::Version(Version&& other)
{
    *this = std::move(other);
}

Version::Version(const Version& other)
{
    Copy(other);
}

Version::Version(LPCTSTR str)
{
    Parse(str);
}

Version& Version::operator=(const Version& other)
{
    if(this != &other)
        Copy(other);
    return *this;
}

void Version::Copy(const Version& other)
{
    memcpy(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS);
}

Version& Version::operator=(Version&& other)
{
    if(this != &other)
    {
        memcpy(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS);
    }
    return *this;
}

bool Version::operator == (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) == 0;
}

bool Version::operator != (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) != 0;
}

bool Version::operator < (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) < 0;
}

bool Version::operator > (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) > 0;
}

bool Version::operator <= (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) <= 0;
}

bool Version::operator >= (const Version& other) const
{
    return memcmp(&m_versionItems, other.m_versionItems, sizeof(WORD)*MAX_VER_ITEMS) >= 0;
}

void Version::Parse(LPCTSTR str)
{
    // for now, the version is zero
    memset(&m_versionItems, 0, sizeof(WORD)*MAX_VER_ITEMS);

    std::vector<std::string> items;
    std::string input(str);
    boost::split(items, input, boost::is_any_of(VERSION_SEPARATOR_STR));
    for (size_t i = 0; i < items.size() && i < MAX_VER_ITEMS; ++i)
    {
        m_versionItems[i] = static_cast<WORD>(atoi(items[i].c_str()));
    }
}

template<>
CString Version::ToString() const
{
    CString s;
    s.Format(_T("%d%c%d%c%d%c%d"), 
        GetMSMajor(), VERSION_SEPARATOR_CHAR, 
        GetMSMinor(), VERSION_SEPARATOR_CHAR, 
        GetLSMajor(), VERSION_SEPARATOR_CHAR, 
        GetLSMinor());
    return s;
}

template <>
std::string Version::ToString() const
{
    std::ostringstream stream;
    stream << GetMSMajor() << VERSION_SEPARATOR_CHAR 
           << GetMSMinor() << VERSION_SEPARATOR_CHAR 
           << GetLSMajor() << VERSION_SEPARATOR_CHAR 
           << GetLSMinor();
    return stream.str();
}