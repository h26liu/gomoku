/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		PaintController.hpp
*/

#include "PaintModel.hpp"
#include "PaintView.hpp"
#include <haohan/Command.hpp>

#pragma once

class PaintController: Command {
private:
	PaintModel* paintModel;
	PaintView* paintView;
public:
	PaintController();

	void MouseEventProc(MOUSE_EVENT_RECORD const& mer) override;
	void ProcessKeyEvent(KEY_EVENT_RECORD const& ker) override;

	void SetColorPalette();
};