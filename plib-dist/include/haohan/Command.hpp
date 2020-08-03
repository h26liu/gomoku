/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		Command.hpp
*/

#pragma once

#include <Windows.h>

class Command {
public:
	virtual void MouseEventProc(MOUSE_EVENT_RECORD const& mer) = 0;
	virtual void ProcessKeyEvent(KEY_EVENT_RECORD const& ker) = 0;
};