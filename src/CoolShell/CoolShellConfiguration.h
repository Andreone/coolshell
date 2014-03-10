#pragma once

#include "CoolShellLib/Mouse.h"

struct WheelUnderCursorServiceConfiguration
{
	bool enabled;
	std::list<CString> windowClassExclusionList;
};

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

CursorLocation CursorLocationFromString(const std::string& value);
std::string CursorLocationToString(CursorLocation value);

enum class ActionType
{
	None,
	ToggleAlwaysOnTop,
	SetAlwaysOnTop,
	UnsetAlwaysOnTop,
	MinimizeToTray,
	DecreaseTransparency,
	IncreaseTransparency,
	SetTransparency,
	VolumeUp,
	VolumeDown,
	MediaNext,
	MediaPrevious,
	MediaPlay,
	MediaPause,
	MediaStop,
	MediaTogglePlayPause,
};

ActionType ActionTypeFromString(const std::string& value);
std::string ActionTypeToString(ActionType value);

struct MouseActionConfiguration
{
	CursorLocation cursorLocation;
	UINT mouseEvent;						/** Mouse button Windows message code (WM_RBUTTONUP, ...) */
	ActionType actionType;
	std::vector<std::string> actionArgs;
	std::vector<int> modifierKeys;
};

struct WindowTitleBarServiceConfiguration
{
	bool enabled;
	std::list<MouseActionConfiguration> mouseActions;
	std::list<CString> windowClassExclusionList;
};

struct MediaRemoteServiceConfiguration
{
	bool enabled;
	std::list<CString> windowClassToHandle;
}; 

struct CoolShellConfiguration
{
	MediaRemoteServiceConfiguration mediaRemoteServiceConfiguration;
	WindowTitleBarServiceConfiguration hotMouseButtonServiceConfiguration;
	WheelUnderCursorServiceConfiguration wheelUnderCursorServiceConfiguration;
};
