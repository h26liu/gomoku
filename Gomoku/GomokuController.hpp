/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuController.hpp
*/

#include "GomokuModel.hpp"
#include "GomokuView.hpp"
#include <haohan/Command.hpp>
#include <vector>
#pragma once


class GomokuController : Command {
private:
	GomokuModel* model;
	HomeView* hv;
	TitleView* tv;
	BoardView* bv;
	ScoreView* sv;
	PlayerView* pv;
	MoveListView* mlv;
public:
	GomokuController();
	void homeController();
	void gameController();
	void MouseEventProc(MOUSE_EVENT_RECORD const& mer) override;
	void ProcessKeyEvent(KEY_EVENT_RECORD const& ker) override;
};