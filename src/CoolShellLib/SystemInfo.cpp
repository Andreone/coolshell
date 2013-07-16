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
#include "SystemInfo.h"

/**
 * \brief Default class constructor.
 */
SystemInfo::SystemInfo() :
	m_si()
{
	::GetSystemInfo(&m_si);
}

/**
 * \brief class destructor.
 */
SystemInfo::~SystemInfo()
{
}

/**
 * \brief Returns the number of processors available into the system.
 * \return [UINT] - a number between 1 and 32
 */
UINT SystemInfo::GetNumberOfProcessors() const
{
	return m_si.dwNumberOfProcessors;
}

/**
 * \brief Returns the number of processors configured into the system.
 * \return [UINT] - The mask representing the set of processors configured into the system.
 *		   Bit 0 is processor 0; bit 31 is processor 31. 
 */
UINT SystemInfo::GetActiveProcessorMask() const
{
	return static_cast<UINT>(m_si.dwActiveProcessorMask);
}

/**
 * \brief Returns the granularity with which virtual memory is allocated in the system.
 *		  For example, a VirtualAlloc request to allocate 1 byte will reserve an address space
 *		  of dwAllocationGranularity bytes.
 * \return [UINT] - The system memory granularity.
 */
UINT SystemInfo::GetAllocationGranularity() const
{
	return m_si.dwAllocationGranularity;
}

/**
 * \brief Returns the page size and the granularity of page protection and commitment.
 *		  This is the page size used by theVirtualAlloc function.
 * \return [UINT] - the page size
 */
UINT SystemInfo::GetPageSize() const
{
	return m_si.dwPageSize;
}
