/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		paint.hpp
*/

#pragma once

#include <haohan/InnerWrapper.hpp>
#include <haohan/Application.hpp>

class Paint: public Application {
public:
	Paint();
	~Paint();

	int execute() override;
} paint;