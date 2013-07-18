#pragma once

struct WheelUnderCursorConfiguration
{
	bool enabled;
	std::list<CString> windowClassExclusionList;
};

struct CoolShellConfiguration
{
	WheelUnderCursorConfiguration wheelUnderCursorConfiguration;
};
