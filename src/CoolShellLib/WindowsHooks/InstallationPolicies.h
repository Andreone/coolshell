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

namespace WindowsHooks
{
    class InstallationPolicy
    {
    public:
        virtual HINSTANCE GetModuleHandle() = 0;
        virtual DWORD GetThreadId() = 0;
    };

    class LocalThreadInstallationPolicy : public InstallationPolicy
    {
    public:
        virtual HINSTANCE GetModuleHandle() { return (HINSTANCE)NULL; }
        virtual DWORD GetThreadId()         { return ::GetCurrentThreadId(); }
    };

    class GlobalScopeInstallationPolicy : public InstallationPolicy
    {
    public:
        virtual HINSTANCE GetModuleHandle() { return ::GetModuleHandle(NULL); }
        virtual DWORD GetThreadId()         { return NULL; }
    };

    class InstallationPolicies
    {
    public:
        static GlobalScopeInstallationPolicy& GetGlobalScopeInstallationPolicy() { return m_globalPolicy; }
        static LocalThreadInstallationPolicy& GetLocalThreadInstallationPolicy() { return m_threadPolicy; }

    private:
        static GlobalScopeInstallationPolicy m_globalPolicy;
        static LocalThreadInstallationPolicy m_threadPolicy;
    };
}