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

class IApplication
{
public:
    virtual ~IApplication() { };

    virtual void Quit(UINT exitCode) = 0;
    virtual void Restart() = 0;
    virtual void TogglePause() = 0;
    virtual bool IsPaused() const = 0;

    virtual bool IsRegisteredToStartWithWindows() const = 0;
    virtual void StartWithWindows() = 0;
    virtual void DoNotStartWithWindows() = 0;
};
