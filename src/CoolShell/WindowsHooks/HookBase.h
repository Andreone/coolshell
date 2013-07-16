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

#include <boost\signals.hpp>
#include "InstallationPolicies.h"

namespace WindowsHooks
{

/**
 * \class HookBase
 * \brief Base implementation class for Windows hooks.
 * Due to the fact that Windows hook uses callbacks without any user parameter,
 * only one instance of HookBase for a given TMonitor type can exists.
 */
template<typename TMonitor, typename TEventArgs>
class HookBase
{
    //////////////////////////////////////////////////////////////////////////
private:
    static TMonitor* m_pInstance;

public:
	typedef boost::signal<void (TEventArgs&)> HookEvent;
    static TMonitor& Instance()
    {
        if(!m_pInstance)
        {
            m_pInstance = new TMonitor();
        }
        return (*m_pInstance);
    }

    static void DeleteInstance()
    {
        if(m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    //////////////////////////////////////////////////////////////////////////
public:
    void Install()
    {
        if(IsInstalled())
            return;

        m_hHook = ::SetWindowsHookEx(m_hookType, 
                                     &HookHandler, 
                                     m_installationPolicy.GetModuleHandle(), 
                                     m_installationPolicy.GetThreadId());

        if(!m_hHook)
        {
            // TODO: throw exact error code returned by GetLastError -> create windows_error(DWORD error)?
            throw std::runtime_error("Failed to install hook");
        }
    }

    void Uninstall()
    {
        if(!IsInstalled())
            return;
        ::UnhookWindowsHookEx(m_hHook);
        m_hHook = nullptr;
    }

    bool IsInstalled() const { return m_hHook != nullptr; }
    HookEvent& GetEvent()    { return m_event; }

    //////////////////////////////////////////////////////////////////////////
protected:
    HookBase(int hookType, InstallationPolicy& policy) :
        m_hHook(nullptr),
        m_event(),
        m_hookType(hookType),
        m_installationPolicy(policy)
    {
    }

    virtual ~HookBase()
    {
        Uninstall();
    }

    //////////////////////////////////////////////////////////////////////////
private:
    static LRESULT CALLBACK HookHandler(int nCode, WPARAM wParam, LPARAM lParam)
    {
        ASSERT(m_pInstance);

        if(nCode < 0)
            return ::CallNextHookEx(m_pInstance->m_hHook, nCode, wParam, lParam);

        TEventArgs* pArgs = m_pInstance->CreateEventArgs(wParam, lParam);
        ASSERT(pArgs);

        m_pInstance->m_event(*pArgs);

        return (!pArgs->IsHandled() ? ::CallNextHookEx(m_pInstance->m_hHook, nCode, wParam, lParam) : 1);
    }

    //////////////////////////////////////////////////////////////////////////
    HHOOK m_hHook;
    HookEvent m_event;
    int m_hookType;
    InstallationPolicy& m_installationPolicy;
};

template<typename TMonitor, typename TEventArgs> 
TMonitor* HookBase<TMonitor, TEventArgs>::m_pInstance = nullptr;
}