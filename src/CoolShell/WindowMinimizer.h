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

#include "WindowClassFilter.h"
#include "boost/noncopyable.hpp"
#include "loki/Singleton.h"

class NotifyIcon;

class WindowMinimizer : public WindowClassFilter,
                        private boost::noncopyable
{
public:
    virtual void MinimizeWindow(HWND hWnd);

protected:
    WindowMinimizer();
    virtual ~WindowMinimizer();
    friend struct Loki::CreateUsingNew<WindowMinimizer>;

    virtual void RestoreWindow(HWND hWnd);
    virtual void CloseWindow(HWND hWnd);
    virtual void ToggleWindowVisibility(HWND hWnd);
    virtual void RemoveIcon(HWND hWnd);

private:
    virtual void ContextMenu(NotifyIcon& icon, HWND hWnd);

    typedef std::shared_ptr<NotifyIcon> NotifyIconPtr;
    typedef std::pair<HWND, NotifyIconPtr> MinimizedWindowPair;
    std::map<HWND, NotifyIconPtr> m_minimizedWindowIcons;
};

typedef Loki::SingletonHolder<WindowMinimizer> TheWindowMinimizer;