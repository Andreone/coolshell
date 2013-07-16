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

#include "HookEventArgsBase.h"

namespace WindowsHooks
{

class LowLevelKeyboardEventArgs : public HookEventArgsBase
{
public:
    static void Trace(const LowLevelKeyboardEventArgs& args)
    {
        TRACE(_T("Hook: VKey=%c Injected=%s AltDown=%s KeyReleased=%s ExtendedKey=%s\n"),
            (TCHAR)args.GetVirtualKeyCode(),
            args.IsInjected() ? _T("true") : _T("false"),
            args.IsAltDown() ? _T("true") : _T("false"),
            args.IsExtendedKey() ? _T("true") : _T("false"),
            args.IsKeyReleased() ? _T("true") : _T("false"));
    }

    LowLevelKeyboardEventArgs() :
        m_msg(0),
        m_pHookStruct(nullptr)
    {
    }

    void SetMessage(UINT val)	            { m_msg = val; }
    UINT GetMessage() const                 { return m_msg; }

    void SetHookStruct(KBDLLHOOKSTRUCT* p)	{ m_pHookStruct = p; }
    DWORD GetVirtualKeyCode() const			{ return m_pHookStruct->vkCode; }
    DWORD GetScanCode() const				{ return m_pHookStruct->scanCode; }
    DWORD GetTimeStamp() const				{ return m_pHookStruct->time; }
    bool IsInjected() const					{ return (m_pHookStruct->flags & LLKHF_INJECTED) != 0; }
    bool IsExtendedKey() const				{ return (m_pHookStruct->flags & LLKHF_EXTENDED) != 0; }
    bool IsAltDown() const					{ return (m_pHookStruct->flags & LLKHF_ALTDOWN) != 0; }
    bool IsKeyReleased() const				{ return (m_pHookStruct->flags & LLKHF_UP) != 0; }

protected:
    UINT m_msg;
    KBDLLHOOKSTRUCT* m_pHookStruct;
};
}