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
#include "ClickManager.h"
#include "boost/bind.hpp"

ClickManager::ClickManager() :
    ClickAlgo(::GetDoubleClickTime()),
    m_timer(),
    m_simpleClickFunc(),
    m_doubleClickFunc()
{
}

ClickManager::~ClickManager()
{
    StopTimer();
}

void ClickManager::OnButtonDown()
{
    StartTimer();
    ClickAlgo::OnButtonDown();
}

void ClickManager::OnButtonUp()
{
    ClickAlgo::OnButtonUp();
}

void ClickManager::StartTimer()
{
    if (!m_timer.is_running())
    {
        m_timer.start(::GetDoubleClickTime() / 2, [this] { return this->Tick(); });
    }
}

void ClickManager::StopTimer()
{
    if (m_timer.is_running())
    {
        m_timer.stop();
    }
}

void ClickManager::OnSingleClick()
{
    StopTimer();
    if (m_simpleClickFunc)
        m_simpleClickFunc();
}

void ClickManager::OnDoubleClick()
{
    StopTimer();
    if (m_doubleClickFunc)
        m_doubleClickFunc();
}
