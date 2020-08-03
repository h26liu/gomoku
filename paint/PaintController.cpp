/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		PaintController.cpp
*/

#include "PaintController.hpp"

#include <Windows.h>
#undef min
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

bool applicationQuitting = false;

bool CtrlHandler(DWORD ctrlType) {
	switch (ctrlType) {
	case CTRL_C_EVENT:
		applicationQuitting = true;
		return TRUE;
	}

	return FALSE;
}

PaintController::PaintController()
{
	paintModel = new PaintModel();
	paintView = new PaintView(*paintModel);

	outerWrapper.SaveConsoleState();

	outerWrapper.SetCtrlHandler(CtrlHandler, true);

	outerWrapper.SetConsoleWindowTitle("Paint");

	outerWrapper.ResizeConsoleWindow(40, 40);

	outerWrapper.HideConsoleWindowCursor();

	outerWrapper.PaintConsoleBackground(BACKGROUND_WHITE);

	// draw RGB color
	SetColorPalette();

	// logic here
	// read input buffer repeatedly
	vector<INPUT_RECORD> inBuffer(128);
	while (!applicationQuitting) {
		DWORD numEvents;
		if (!ReadConsoleInput(outerWrapper.GetConsoleWindowInput(), inBuffer.data(), (DWORD)inBuffer.size(), &numEvents)) {
			cerr << "Failed to read console input\n";
			break;
		}

		for (size_t iEvent = 0; iEvent < numEvents; ++iEvent) {
			switch (inBuffer[iEvent].EventType) {
			case MOUSE_EVENT:
				MouseEventProc(inBuffer[iEvent].Event.MouseEvent);
				break;
			case KEY_EVENT:
				ProcessKeyEvent(inBuffer[iEvent].Event.KeyEvent);
				break;
			}
		}
	}

	outerWrapper.RestoreConsoleState();

}

void PaintController::MouseEventProc(MOUSE_EVENT_RECORD const& mer) {
#if !defined(MOUSE_HWHEELED)
#define MOUSE_HWHEELED 0x0008
#endif
	// Mouse Location
	paintModel->bufferLoc = mer.dwMousePosition;

	if (mer.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED) {
		if (paintModel->bufferLoc.Y <= 2) {
			// RGB RED
			if (paintModel->bufferLoc.X <= 8 && paintModel->bufferLoc.X >= 5 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->C_RED;
			}
			else if (paintModel->bufferLoc.X <= 13 && paintModel->bufferLoc.X >= 9 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->C_GREEN;
			}
			else if (paintModel->bufferLoc.X <= 18 && paintModel->bufferLoc.X >= 15 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->C_BLUE;
			}
			else if (paintModel->bufferLoc.X <= 23 && paintModel->bufferLoc.X >= 20 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->RGB_C;
			}
			else if (paintModel->bufferLoc.X <= 28 && paintModel->bufferLoc.X >= 25 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->RGB_Y;
			}
			else if (paintModel->bufferLoc.X <= 33 && paintModel->bufferLoc.X >= 30 && paintModel->bufferLoc.Y == 1) {
				PaintController::paintModel->cColour = PaintController::paintModel->RGB_M;
			}
		}
		else {
			PaintController::paintView->Update();
			PaintController::paintModel->Notify();
		}
	}
}

void PaintController::ProcessKeyEvent(KEY_EVENT_RECORD const& ker) {
	if (isgraph(ker.uChar.AsciiChar) && ker.uChar.AsciiChar == 'c')
	{
		// Clear the console window
		outerWrapper.PaintConsoleBackground(BACKGROUND_WHITE);
		SetColorPalette();
	}
}

void PaintController::SetColorPalette()
{
	COORD loc;
	loc.Y = 1;

	for (unsigned i = 4; i < 8; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->C_RED, loc);
	}

	for (unsigned i = 9; i < 13; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->C_GREEN, loc);
	}

	for (unsigned i = 14; i < 18; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->C_BLUE, loc);
	}

	for (unsigned i = 19; i < 23; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->RGB_C, loc);
	}

	for (unsigned i = 24; i < 28; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->RGB_Y, loc);
	}

	for (unsigned i = 29; i < 33; ++i) {
		loc.X = i + 1;
		outerWrapper.PaintCellClicked(PaintController::paintModel->RGB_M, loc);
	}
}
