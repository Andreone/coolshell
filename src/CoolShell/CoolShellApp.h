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

#include "CoolShellMainWnd.h"
#include "IApplication.h"

#include "DragWindowModule.h"
#include "WindowTitleBarService.h"
#include "MouseEventDispatcher.h"
#include "MediaRemoteService.h"
#include "WheelUnderCursorService.h"

class CoolShellApp : public CWinApp,
                     public IApplication
{
public:
    CoolShellApp();
    virtual ~CoolShellApp();

	// CWinApp
    BOOL InitInstance() override;
    
    // IApplication implementation
    void Quit(UINT exitCode) override;
    void TogglePause() override;
    void Restart() override;
    bool IsPaused() const override;
    bool IsRegisteredToStartWithWindows() const override;
    void StartWithWindows() override;
    void DoNotStartWithWindows() override;

private:
	void ExitInstance();

	std::shared_ptr<MouseEventDispatcher> m_mouseEventDispatcher;
    std::shared_ptr<WheelUnderCursorService> m_wheelUnderCursorService;
    std::shared_ptr<WindowTitleBarService> m_windowTitleBarService;
    std::shared_ptr<DragWindowModule> m_dragWindowService;
	std::shared_ptr<MediaRemoteService> m_mediaRemoteService;
	CoolShellMainWnd m_mainWindow;
    bool m_isPaused;
};
