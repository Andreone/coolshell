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
#include "CoolShellConfiguration.h"

const char* cursorLocationNames [] =
{
	"None",
	"TitleBar",
	"MinimizeButton",
	"MaximizeButton",
	"CloseButton",
	"Desktop",
	"Taskbar",
	nullptr
};

CursorLocation CursorLocationFromString(const std::string& value)
{
	for(int i=0; cursorLocationNames[i] != nullptr; ++i)
	{
		if(value == cursorLocationNames[i])
			return (CursorLocation)i;
	}
	throw std::runtime_error("Invalid cursor location string value");
}

std::string CursorLocationToString(CursorLocation value)
{
	return cursorLocationNames[(int)value];
}
