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

namespace ScopeExit
{
    namespace Detail
    {
        template <class F>
        class ScopeExitFunction
        {
        public:
            ScopeExitFunction(F&& func) throw() :
                m_func(std::forward<F>(func))
            {
            }

            ~ScopeExitFunction() throw()
            {
                m_func();
            }

        private:
            F m_func;
        };

        template <class F>
        class ScopeSuccessFunction
        {
        public:
            ScopeSuccessFunction(F&& func) throw() :
                m_func(std::forward<F>(func))
            {
            }

            ~ScopeSuccessFunction() throw()
            {
                if(!std::uncaught_exception())
                    m_func();
            }

        private:
            F m_func;
        };

        template <class F>
        class ScopeFailureFunction
        {
        public:
            ScopeFailureFunction(F&& func) throw() :
                m_func(std::forward<F>(func))
            {
            }

            ~ScopeFailureFunction() throw()
            {
                if(std::uncaught_exception())
                    m_func();
            }

        private:
            F m_func;
        };

    } // namespace Detail

    template <class F>
    Detail::ScopeExitFunction<F> AtScopeExit(F&& func)
    {
        return Detail::ScopeExitFunction<F>(std::forward<F>(func));
    }

    template <class F>
    Detail::ScopeSuccessFunction<F> AtScopeSuccess(F&& func)
    {
        return Detail::ScopeSuccessFunction<F>(std::forward<F>(func));
    }

    template <class F>
    Detail::ScopeFailureFunction<F> AtScopeFailure(F&& func)
    {
        return Detail::ScopeFailureFunction<F>(std::forward<F>(func));
    }

} // namespace ScopeExit
