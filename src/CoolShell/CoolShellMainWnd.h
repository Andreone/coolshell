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

class IApplication;
#include "CoolShellLib\NotifyIcon.h"
#include "CoolShellLib\ClickManager.h"

class CoolShellMainWnd : public CWnd
{
public:
    CoolShellMainWnd();
    
    void Setup(IApplication* application);
    void ReflectPausedState(bool isPaused);

    HWND CreateEx(DWORD dwExStyle, LPCTSTR lpszClassName, LPCTSTR lpszWindowName, 
        DWORD dwStyle, int x, int y, int nWidth, int nHeight, 
        CWnd* pParent, CMenu* pMenu, LPVOID lpParam = NULL) override;
        
protected:
    virtual void OnInitialUpdate();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    IApplication* m_application;
    NotifyIcon m_notifyIcon;
    ClickManager m_leftClickManager;
};
