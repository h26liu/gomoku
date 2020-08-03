/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuView.hpp
*/

#pragma once
#include <haohan/Observer.hpp>
#include "GomokuModel.hpp"
#include <string>

class HomeView : public Observer {
private:
	GomokuModel* model;
public:
	HomeView(GomokuModel& m) { model = &m; }
	void Update() override;
};

class TitleView : public Observer {
private:
	GomokuModel* model;
public:
	TitleView(GomokuModel& m) { model = &m; }
	void Update() override;
};


class BoardView : public Observer {
private:
	GomokuModel* model;
public:
	BoardView(GomokuModel& m) { model = &m; }
	void Update() override;
};


class ScoreView : public Observer {
private:
	GomokuModel* model;
public:
	ScoreView(GomokuModel& m) { model = &m; }
	void Update() override;
};

class PlayerView : public Observer {
private:
	GomokuModel* model;
public:
	PlayerView(GomokuModel& m) { model = &m; }
	void Update() override;
};

class MoveListView : public Observer {
private:
	GomokuModel* model;
public:
	MoveListView(GomokuModel& m) { model = &m; }
	void Update() override;
};
