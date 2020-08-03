/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		Gomoku.hpp
*/

#pragma once

#include <haohan/InnerWrapper.hpp>
#include <haohan/OuterWrapper.hpp>
#include <haohan/Application.hpp>
#include "GomokuController.hpp"

class Gomoku : public Application {
public:
	Gomoku();
	~Gomoku();

	int execute() override;
} gomoku;