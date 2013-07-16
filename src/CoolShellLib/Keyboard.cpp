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
#include "Keyboard.h"

namespace Keyboard
{
bool IsKeyDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) ? true : false;
}

bool AreKeysDown(const std::vector<int>& vkKeys)   
{
    for(auto i = vkKeys.cbegin(); i != vkKeys.cend(); ++i)
    {
        if(!Keyboard::IsKeyDown(*i))
            return false;
    }
    return true;
}

bool IsKeyExtended(int vk)
{
    return vk == VK_NUMLOCK || vk == VK_DIVIDE || (vk >= VK_PRIOR && vk <= VK_DELETE);
}

CString GetKeyNameText(int vk)
{
    long scanCode = (::MapVirtualKey(LOBYTE(vk), MAPVK_VK_TO_VSC) << 16) | (1L << 25);
    if(IsKeyExtended(vk))
        scanCode |= 1L << 24;

    TCHAR szName[128];
    ::GetKeyNameText(scanCode, szName, 128);
    return szName;
}

} // namespace Keyboard
