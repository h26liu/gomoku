/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		PaintView.cpp
*/

#include "PaintView.hpp"

void PaintView::Update()
{
	outerWrapper.PaintCellClicked(model->cColour, model->bufferLoc);
}
