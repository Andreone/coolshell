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

#include <sstream>

/**
 * \class	WinError
 * \brief	Exception that wraps Windows API error codes.
 *
 * This exception type (stl compliant) wraps a Windows API error code
 * most often retrieved by calling ::GetLastError() after a call to a
 * Windows function that failed.
 *
 * \par Usage
 * \code
 *          if(!some_windows_api_call())
 *              throw WinError();
 *  \endcode
 */

class WinError : public std::runtime_error
{
public:
   WinError(DWORD code = ::GetLastError(), const char* userMessage=nullptr) throw() :
        std::runtime_error(GetErrorMessage(code, userMessage)),
        m_errCode(code)
    { }

    WinError(const char* userMessage, DWORD code = ::GetLastError()) throw() :
        std::runtime_error(GetErrorMessage(code, userMessage)),
        m_errCode(code)
    { }

    virtual ~WinError() throw()
    { }

    DWORD GetErrorCode() throw()
    {
        return m_errCode;
    }

    static std::string GetErrorMessage(DWORD errCode, const char* userMessage = nullptr) throw()
    {
        std::ostringstream stream;
        LPSTR pBuffer = nullptr;

        if(userMessage)
            stream << userMessage << ": ";
        
        if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                            NULL,
                            errCode,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) &pBuffer,
                            0,
                            NULL ))
        {
            stream << "[0x" << std::uppercase << std::hex << errCode << "] " << pBuffer;
            LocalFree(pBuffer);
        }
        else
        {
            stream << "No system message found for the error code " << "[0x" << std::uppercase << std::hex << errCode << "] " ;
        }

        return stream.str();
    }

private:
    DWORD m_errCode;		/**< Windows API error code */
};
