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

#include "ClickAlgo.h"
#include "boost/noncopyable.hpp"
#include "timers.h"
#include <functional>

class ClickManager : protected ClickAlgo, protected boost::noncopyable
{
public:
    typedef std::function<void ()> ClickDelegate;

    ClickManager();
    virtual ~ClickManager();

    void WhenSimpleClick(ClickDelegate func) { m_simpleClickFunc = func; }
    void WhenDoubleClick(ClickDelegate func) { m_doubleClickFunc = func; }
    void OnButtonDown();
    void OnButtonUp();

private:
    void StartTimer();
    void StopTimer();
    bool OnTimer();

    virtual void OnSingleClick();
    virtual void OnDoubleClick();

    timers::win_timer m_timer;
    ClickDelegate m_simpleClickFunc;
    ClickDelegate m_doubleClickFunc;
};
