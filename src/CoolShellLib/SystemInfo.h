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
 * \class SystemInfo
 * SYSTEM_INFO structure wrapper. Retrieves information about the current system such as the number of processors.
 */

class SystemInfo
{
public:
    enum cpuid
    {
        CPUID0	= 0x00000001,
        CPUID1	= 0x00000002,
        CPUID2	= 0x00000004,
        CPUID3	= 0x00000008,
        CPUID4	= 0x00000010,
        CPUID5	= 0x00000020,
        CPUID6	= 0x00000040,
        CPUID7	= 0x00000080,
        CPUID8	= 0x00000100,
        CPUID9	= 0x00000200,
        CPUID10	= 0x00000400,
        CPUID11	= 0x00000800,
        CPUID12	= 0x00001000,
        CPUID13	= 0x00002000,
        CPUID14	= 0x00004000,
        CPUID15	= 0x00008000,
        CPUID16	= 0x00010000,
        CPUID17	= 0x00020000,
        CPUID18	= 0x00040000,
        CPUID19	= 0x00080000,
        CPUID20	= 0x00100000,
        CPUID21	= 0x00200000,
        CPUID22	= 0x00400000,
        CPUID23	= 0x00800000,
        CPUID24	= 0x01000000,
        CPUID25	= 0x02000000,
        CPUID26	= 0x04000000,
        CPUID27	= 0x08000000,
        CPUID28	= 0x10000000,
        CPUID29	= 0x20000000,
        CPUID30	= 0x40000000,
        CPUID31	= 0x80000000
    };

    SystemInfo();
    ~SystemInfo();

    UINT GetNumberOfProcessors() const;
    UINT GetActiveProcessorMask() const;
    UINT GetAllocationGranularity() const;
    UINT GetPageSize() const;

private:
    SYSTEM_INFO m_si;		/**< WIN32 API structure that contains information about the current computer system */
};
