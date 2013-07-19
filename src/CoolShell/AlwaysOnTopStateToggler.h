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

class AlwaysOnTopStateToggler : public WindowClassFilter,
                                private boost::noncopyable
{
public:
    void ToggleWindowState(HWND hWnd);

protected:
    AlwaysOnTopStateToggler();
    virtual ~AlwaysOnTopStateToggler();
    friend struct Loki::CreateUsingNew<AlwaysOnTopStateToggler>;

private:
    std::map<HWND, bool> m_modifiedWindows;

};

typedef Loki::SingletonHolder<AlwaysOnTopStateToggler> TheAlwaysOnTopStateToggler;