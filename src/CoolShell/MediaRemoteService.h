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

#include <boost/noncopyable.hpp>

#include "IMouseEventDispatcher.h"

class MediaRemote
{
public:
	void VolumeUp()
	{
		SendInput(VK_VOLUME_UP);
	}

	void VolumeDown()
	{
		SendInput(VK_VOLUME_DOWN);
	}

	void VolumeMute()
	{
		SendInput(VK_VOLUME_MUTE);
	}

	void Stop()
	{
		SendInput(VK_MEDIA_STOP);
	}

	void PlayPause()
	{
		SendInput(VK_MEDIA_PLAY_PAUSE);
	}

	void NextTrack()
	{
		SendInput(VK_MEDIA_NEXT_TRACK);
	}

	void PreviousTrack()
	{
		SendInput(VK_MEDIA_PREV_TRACK);
	}

private:
	void SendInput(WORD vk)
	{
		INPUT input;
		memset(&input, 0, sizeof(INPUT));

		input.type = INPUT_KEYBOARD;
		input.ki.wVk = vk;

		// key down
		::SendInput(1, &input, sizeof(INPUT));

		// key up
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		::SendInput(1, &input, sizeof(INPUT));
	}
};

struct MediaRemoteServiceConfiguration;

class MediaRemoteService : public boost::noncopyable
{
public:
	MediaRemoteService(std::shared_ptr<IMouseEventDispatcher> mouseEventDispatcher);

	void Initialize(const MediaRemoteServiceConfiguration& configuration);

private:
    void OnMouseEvent(WindowsHooks::LowLevelMouseEventArgs& args);
	bool IsWindowToHandle(const CString& s) const;
		
	std::shared_ptr<IMouseEventDispatcher> m_mouseEventDispatcher;
	MediaRemote m_mediaRemote;
	std::list<CString> m_wndClasses;
};
