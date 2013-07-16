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

#include <functional>

class ScopeExitFunction
{
public:
    typedef std::function<void ()> function_type;

    ScopeExitFunction(function_type func) throw() :
        m_func(func)
    {
    }

    ~ScopeExitFunction() throw()
    {
        safe_call();
    }

    void release() throw()
    {
        m_func = function_type();
    }

    void reset(function_type other = function_type()) throw()
    {
        safe_call();
        m_func = other;
    }

private:
    void safe_call()
    {
        try
        {
            if(m_func)
                m_func();
        }
        catch(...)
        { }
    }

    function_type m_func;
};
