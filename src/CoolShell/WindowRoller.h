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

class WindowRoller : public WindowClassFilter,
                     private boost::noncopyable
{
public:
    void ToggleRollWindow(HWND hWnd);

protected:
    WindowRoller();
    virtual ~WindowRoller();
    friend struct Loki::CreateUsingNew<WindowRoller>;

    void RollWindow(HWND hWnd, const CRect& unrolledRect, const CRect& rolledRect);
    void UnrollWindow(HWND hWnd, const CRect& rolledRect, const CRect& unrolledRect);

private:
    std::map<HWND, CRect> m_rolledWindows;
};

typedef Loki::SingletonHolder<WindowRoller> TheWindowRoller;