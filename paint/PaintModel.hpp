/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		PaintModel.hpp
*/

#pragma once

#include <haohan/OuterWrapper.hpp>
#include <haohan/ObservablePublisher.hpp>

static OuterWrapper outerWrapper;

class PaintModel : public ObservablePublisher {
public:
	static WORD const C_RED = BACKGROUND_RED;
	static WORD const C_GREEN = BACKGROUND_GREEN;
	static WORD const C_BLUE = BACKGROUND_BLUE;

	static WORD const RGB_Y = C_RED | C_GREEN;
	static WORD const RGB_M = C_RED | C_BLUE;
	static WORD const RGB_C = C_BLUE | C_GREEN;

	WORD cColour = C_RED;
	COORD bufferLoc;
};
