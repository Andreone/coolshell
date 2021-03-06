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
class MouseEventArgs : public HookEventArgsBase
{
public:
    MouseEventArgs() :
        m_msg(0),
        m_pHookStruct(nullptr)
    {
    }

    void SetMessage(UINT val)               { m_msg = val; }
    UINT GetMessage() const		            { return m_msg; }

    void SetHookStruct(MOUSEHOOKSTRUCT* p)	{ m_pHookStruct = p; }
    const POINT& GetPoint() const			{ return m_pHookStruct->pt; }
    HWND GetHwnd() const			        { return m_pHookStruct->hwnd; }
    UINT GetHitTestCode() const			    { return m_pHookStruct->wHitTestCode; }

protected:
    UINT m_msg;
    MOUSEHOOKSTRUCT* m_pHookStruct;
};

}