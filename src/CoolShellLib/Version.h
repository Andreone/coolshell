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
 * \brief Compares string represented version information
 *	This class allows to compare two version information and deals with string 
 *	comparison problem (by performing numerical comparisons).
 *	The version "1.9" is "lower" than version "1.10", but if strings are
 *	compared "1.9" will be the greatest, which is wrong.
 *
 *	A version is is constituted of 4 integer numbers (or less) separated by a dot.
 *	Thus, "1", "1.2", "1.2.3" and "1.2.3.4" are correct version representations.
 *
 * \note "1" is considered to be the same than "1.0" (which is the same than "1.0.0", etc.)
 *	just like "1.1" is the same than "1.01"
 */

class Version
{
public:
    static Version GetFileVersion(const CString& sFileName);

    Version(WORD wMSMajor=0, WORD wMSMinor=0, WORD wLSMajor=0, WORD wLSMinor=0);
    Version(const Version& other);
    Version(Version&& other);
    explicit Version(LPCTSTR str);

    Version& operator = (Version&& other);
    Version& operator = (const Version& other);
    bool operator == (const Version& other) const;
    bool operator != (const Version& other) const;
    bool operator <	 (const Version& other) const;
    bool operator <= (const Version& other) const;
    bool operator >	 (const Version& other) const;
    bool operator >= (const Version& other) const;

    WORD GetMSMajor() const { return m_versionItems[0]; }
    void SetMSMajor(WORD v) { m_versionItems[0] = v; }
    WORD GetMSMinor() const { return m_versionItems[1]; }
    void SetMSMinor(WORD v) { m_versionItems[1] = v; }
    WORD GetLSMajor() const { return m_versionItems[2]; }
    void SetLSMajor(WORD v) { m_versionItems[2] = v; }
    WORD GetLSMinor() const { return m_versionItems[3]; }
    void SetLSMinor(WORD v) { m_versionItems[3] = v; }

    template <typename T> T ToString() const;
    void Parse(LPCTSTR str);
    void Copy(const Version& other);

private:
    WORD m_versionItems[4];
};
