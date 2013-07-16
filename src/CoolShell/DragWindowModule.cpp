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
#include "DragWindowModule.h"

#include <boost/bind.hpp>

#include "DragWindowManager.h"

DragWindowModule::DragWindowModule() :
    IMouseModule(),
    m_managers()
{
}

DragWindowModule::~DragWindowModule()
{
}

void DragWindowModule::Setup(IMouseEventDispatcher& dispatcher)
{
    {
    Mouse::Mousestroke mousestroke;
    auto man = std::make_shared<DragWindowManager> ();
    dispatcher.MButtonDownEvent().connect(boost::bind(&DragWindowManager::OnButtonDown, man.get(), _1));
    dispatcher.MButtonUpEvent().connect(boost::bind(&DragWindowManager::OnButtonUp, man.get(), _1));
    mousestroke.vkButton = VK_MBUTTON;
    man->SetMousestroke(mousestroke);
    m_managers.push_back(man);    
    }

    {
    Mouse::Mousestroke mousestroke;
    auto man = std::make_shared<DragWindowManager> ();
    dispatcher.RButtonDownEvent().connect(boost::bind(&DragWindowManager::OnButtonDown, man.get(), _1));
    dispatcher.RButtonUpEvent().connect(boost::bind(&DragWindowManager::OnButtonUp, man.get(), _1));
    mousestroke.vkButton = VK_RBUTTON;
    mousestroke.modifierKeys.push_back(VK_LMENU);
    man->SetMousestroke(mousestroke);
    m_managers.push_back(man);    
    }
}