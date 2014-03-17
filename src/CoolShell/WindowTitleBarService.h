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
#include <tuple>

#include <boost/noncopyable.hpp>

#include "IMouseEventDispatcher.h"

struct WindowTitleBarServiceConfiguration;

class WindowTitleBarService : public boost::noncopyable
{
public:
    typedef std::function<void (HWND, WindowsHooks::LowLevelMouseEventArgs&)> MouseAction;

    WindowTitleBarService(std::shared_ptr<IMouseEventDispatcher> mouseEventDispatcher);

	void Initialize(const WindowTitleBarServiceConfiguration& configuration);

protected:
    void RegisterAction(UINT area, UINT mouseEvent, MouseAction action);
    void OnMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args);

private:
	std::shared_ptr<IMouseEventDispatcher> m_mouseEventDispatcher;

	typedef std::map<std::tuple<UINT,UINT>, MouseAction> MouseActionMap;
    typedef std::pair<std::tuple<UINT,UINT>, MouseAction> MouseActionMapPair;
    MouseActionMap m_actions;
};
