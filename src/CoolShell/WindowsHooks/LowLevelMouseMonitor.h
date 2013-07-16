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

#include "HookBase.h"
#include "InstallationPolicies.h"
#include "LowLevelMouseEventArgs.h"

namespace WindowsHooks
{

/**
 * class LowLevelMouseMonitor
 * Monitor for low level mouse events.
 */
class LowLevelMouseMonitor : public HookBase<LowLevelMouseMonitor, LowLevelMouseEventArgs>
{
    friend HookBase<LowLevelMouseMonitor, LowLevelMouseEventArgs>;

protected:
    LowLevelMouseMonitor();
 
    LowLevelMouseEventArgs* CreateEventArgs(WPARAM wParam, LPARAM lParam);

private:
    LowLevelMouseEventArgs m_eventArgs;	/** event arguments passed to event watchers */
};

}