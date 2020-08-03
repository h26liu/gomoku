/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		PaintView.hpp
*/

#pragma once

#include <haohan/Observer.hpp>
#include "PaintModel.hpp"

class PaintView : public Observer {
private:
	PaintModel* model;
public:
	PaintView(PaintModel& m) { model = &m; }
	void Update() override;
};
