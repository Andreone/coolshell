#pragma once

#include "CoolShellLib/Mouse.h"

struct WheelUnderCursorServiceConfiguration
{
	bool enabled;
	std::list<CString> windowClassExclusionList;
};

//UINT area, UINT mouseEvent, MouseAction action

enum class CursorLocation
{
	None,
	TitleBar,
	MinimizeButton,
	MaximizeButton,
	CloseButton,
	Desktop,
	Taskbar,
};

CursorLocation fromString(const std::string& value);
std::string toString(CursorLocation value);

struct MouseActionConfiguration
{
	CursorLocation cursorLocation;
	UINT mouseEvent;						/** Mouse button Windows message code (WM_RBUTTONUP, ...) */
	std::string actionName;
	std::vector<std::string> actionArgs;
};

struct HotMouseButtonServiceConfiguration
{
	bool enabled;
	std::list<MouseActionConfiguration> mouseActions;
	std::list<CString> windowClassExclusionList;
};

struct CoolShellConfiguration
{
	HotMouseButtonServiceConfiguration hotMouseButtonServiceConfiguration;
	WheelUnderCursorServiceConfiguration wheelUnderCursorServiceConfiguration;
};
