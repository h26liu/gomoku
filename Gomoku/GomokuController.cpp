/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuController.cpp
*/

#include "GomokuController.hpp"

bool applicationQuitting = false;
bool CtrlHandler(DWORD ctrlType) {
	switch (ctrlType) {
	case CTRL_C_EVENT:
		applicationQuitting = true;
		return TRUE;
	}

	return FALSE;
}

GomokuController::GomokuController()
{
	model = new GomokuModel();
	hv = new HomeView(*model);
	tv = new TitleView(*model);
	bv = new BoardView(*model);
	sv = new ScoreView(*model);
	pv = new PlayerView(*model);
	mlv = new MoveListView(*model);
	model->Attach(hv);
	model->Attach(tv);
	model->Attach(bv);
	model->Attach(sv);
	model->Attach(pv);
	model->Attach(mlv);


	model->outer.SaveConsoleState();

	model->outer.SetCtrlHandler(CtrlHandler, true);

	model->outer.SetConsoleWindowTitle("Gomoku");

	model->outer.ResizeConsoleWindow(80, 30);

	//outerWrapper.HideConsoleWindowCursor();

	model->outer.PaintConsoleBackground(BACKGROUND_WHITE);

	model->gs = GomokuModel::HOMEVIEW;
	model->Notify();

	homeController();

	//PlayerInfoView();
	model->outer.HideConsoleWindowCursor();

	model->outer.PaintConsoleBackground(BACKGROUND_WHITE);


	model->gs = GomokuModel::BOARDVIEW;
	model->Notify();
	//model->DrawGomokuGameBoard();

	model->AddCoordinates();
	model->FillBoardVector();

	model->gs = GomokuModel::TITLEVIEW;
	model->Notify();
	//PrintGreetingMsg();

	model->gs = GomokuModel::SCOREVIEW;
	model->Notify();

	// while loop inside the function body
	gameController();
	
	model->outer.RestoreConsoleState();

}

void GomokuController::homeController()
{
	// read input buffer repeatedly
	std::vector<INPUT_RECORD> inBuffer(128);
	while (!model->hasGameStarted) {
		DWORD numEvents;
		if (!ReadConsoleInput(model->outer.GetConsoleWindowInput(), inBuffer.data(), (DWORD)inBuffer.size(), &numEvents)) {
			//cerr << "Failed to read console input\n";
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
}



void GomokuController::gameController()
{
	// logic here
	// read input buffer repeatedly
	std::vector<INPUT_RECORD> inBuffer(128);
	while (!applicationQuitting) {
		DWORD numEvents;
		if (!ReadConsoleInput(model->outer.GetConsoleWindowInput(), inBuffer.data(), (DWORD)inBuffer.size(), &numEvents)) {
			//cerr << "Failed to read console input\n";
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

}

void GomokuController::MouseEventProc(MOUSE_EVENT_RECORD const & mer)
{
#if !defined(MOUSE_HWHEELED)
#define MOUSE_HWHEELED 0x0008
#endif
	// Mouse Location
	auto bufferLoc = mer.dwMousePosition;

	switch (mer.dwEventFlags) {
	case 0:	// button pressed or released
	{
		if (mer.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED && !model->gameOver && model->hasGameStarted) {
			if (bufferLoc.Y < 10 || bufferLoc.X >= 41 ||
				bufferLoc.Y >= 28 || bufferLoc.X <= 3)
				break;

			if (model->aPieces[bufferLoc.Y][bufferLoc.X] != 0 ||
				model->bPieces[bufferLoc.Y][bufferLoc.X] != 0)
				break;

			if (bufferLoc.X % 2 == 0) {
				if (model->clickCount % 2 == 0) {
					if (model->movesA.size() >= 21) {
						model->movesA.erase(model->movesA.begin());
					}

					// add player A moves
					model->movesA.push_back(bufferLoc);
					model->gs = GomokuModel::MOVELISTVIEW;
					model->Notify();
					//UpdateUserMoves();

					model->scoreA++;
					model->gs = GomokuModel::SCOREVIEW;
					model->Notify();
					//UpdateUserScores();

					model->outer.PaintCellClicked(model->colorA, bufferLoc);
					model->aPieces[bufferLoc.Y][bufferLoc.X] = 1;

					if (model->FiveInRow(model->aPieces)) {
						model->gameOver = true;
						std::string msg = "[" + model->nameA + " Wins!]";
						COORD loc;
						loc.X = (40 - (SHORT)msg.size()) / 2;
						loc.Y = 8;
						model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

						// paint move background
						for (size_t i = 0; i < msg.size(); i++)
						{
							model->outer.PaintCellClicked(model->colorA, loc);
							loc.X++;
						}
					}
				}
				else {
					if (model->movesB.size() >= 21) {
						model->movesB.erase(model->movesB.begin());
					}

					// add player B moves
					model->movesB.push_back(bufferLoc);
					model->gs = GomokuModel::MOVELISTVIEW;
					model->Notify();
					//UpdateUserMoves();

					model->scoreB++;
					model->gs = GomokuModel::SCOREVIEW;
					model->Notify();
					//UpdateUserScores();

					model->outer.PaintCellClicked(model->colorB, bufferLoc);
					model->bPieces[bufferLoc.Y][bufferLoc.X] = 1;

					if (model->FiveInRow(model->bPieces)) {
						model->gameOver = true;
						std::string msg = "[" + model->nameB + " Wins!]";
						COORD loc;
						loc.X = (40 - (SHORT)msg.size()) / 2;
						loc.Y = 8;
						model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

						// paint move background
						for (size_t i = 0; i < msg.size(); i++)
						{
							model->outer.PaintCellClicked(model->colorB, loc);
							loc.X++;
						}
					}
				}

				model->clickCount += 1;
			}
		}
		else if (mer.dwButtonState&RIGHTMOST_BUTTON_PRESSED && !model->gameOver && model->hasGameStarted) {
			// RIGHT-CLICK undo
			model->clickCount -= 1;

			if (model->clickCount % 2 == 0) {
				if (model->movesA.size() > 0) {
					bufferLoc.X = model->movesA.back().X;
					bufferLoc.Y = model->movesA.back().Y;
					model->outer.PaintCellClicked(BACKGROUND_WHITE, bufferLoc);
					model->aPieces[model->movesA.back().Y][model->movesA.back().X] = 0;
					model->movesA.erase(model->movesA.end() - 1);

					model->scoreA--;

					model->gs = GomokuModel::SCOREVIEW;
					model->Notify();
					//UpdateUserScores();

					model->gs = GomokuModel::MOVELISTVIEW;
					model->Notify();
					//UpdateUserMoves();
				}
			}
			else {
				if (model->movesB.size() > 0) {
					bufferLoc.X = model->movesB.back().X;
					bufferLoc.Y = model->movesB.back().Y;
					model->outer.PaintCellClicked(BACKGROUND_WHITE, bufferLoc);
					model->bPieces[model->movesB.back().Y][model->movesB.back().X] = 0;
					model->movesB.erase(model->movesB.end() - 1);

					model->scoreB--;
					model->gs = GomokuModel::SCOREVIEW;
					model->Notify();
					//UpdateUserScores();

					model->gs = GomokuModel::MOVELISTVIEW;
					model->Notify();
					//UpdateUserMoves();
				}
			}
		}
		else if (mer.dwButtonState&FROM_LEFT_1ST_BUTTON_PRESSED && !model->hasGameStarted) {
			if (bufferLoc.X > 14 && bufferLoc.X < 35 && bufferLoc.Y == model->inputLocA.Y)
			{
				model->inputLoc = model->inputLocA;
				// set cursor loc
				COORD cursorLoc{ model->inputLocA.X + (SHORT)model->nameA.size(), model->inputLocA.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
			else if (bufferLoc.X > 20 && bufferLoc.X < 75 && bufferLoc.Y == model->inputLocA.Y)
			{
				model->inputLoc = model->inputLocB;
				// set cursor loc
				COORD cursorLoc{ model->inputLocB.X + (SHORT)model->nameB.size(), model->inputLocB.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
			// color selection
			else if (bufferLoc.Y >= model->inputLocA.Y + 5 && bufferLoc.Y <= model->inputLocA.Y + 7) {
				//*************************
				// PLAYER A
				//*************************
				if (bufferLoc.X > model->inputLocA.X - 7 && bufferLoc.X < model->inputLocA.X - 2) {
					if (bufferLoc.Y == model->inputLocA.Y + 5 && model->colorB != model->RGB_R)
						model->colorA = model->RGB_R;
					else if (bufferLoc.Y == model->inputLocA.Y + 7 && model->colorB != model->RGB_C)
						model->colorA = model->RGB_C;

					model->UpdatePlayerColors();
				}
				else if (bufferLoc.X > model->inputLocA.X - 1 && bufferLoc.X < model->inputLocA.X + 3) {
					if (bufferLoc.Y == model->inputLocA.Y + 5 && model->colorB != model->RGB_G)
						model->colorA = model->RGB_G;
					else if (bufferLoc.Y == model->inputLocA.Y + 7 && model->colorB != model->RGB_Y)
						model->colorA = model->RGB_Y;

					model->UpdatePlayerColors();
				}
				else if (bufferLoc.X > model->inputLocA.X + 4 && bufferLoc.X < model->inputLocA.X + 9) {
					if (bufferLoc.Y == model->inputLocA.Y + 5 && model->colorB != model->RGB_B)
						model->colorA = model->RGB_B;
					else if (bufferLoc.Y == model->inputLocA.Y + 7 && model->colorB != model->RGB_M)
						model->colorA = model->RGB_M;

					model->UpdatePlayerColors();
				}

				//*************************
				// PLAYER B
				//*************************
				if (bufferLoc.X > model->inputLocB.X - 7 && bufferLoc.X < model->inputLocB.X - 2) {
					if (bufferLoc.Y == model->inputLocB.Y + 5 && model->colorA != model->RGB_R)
						model->colorB = model->RGB_R;
					else if (bufferLoc.Y == model->inputLocB.Y + 7 && model->colorA != model->RGB_C)
						model->colorB = model->RGB_C;

					model->UpdatePlayerColors();
				}
				else if (bufferLoc.X > model->inputLocB.X - 1 && bufferLoc.X < model->inputLocB.X + 3) {
					if (bufferLoc.Y == model->inputLocB.Y + 5 && model->colorA != model->RGB_G)
						model->colorB = model->RGB_G;
					else if (bufferLoc.Y == model->inputLocB.Y + 7 && model->colorA != model->RGB_Y)
						model->colorB = model->RGB_Y;

					model->UpdatePlayerColors();
				}
				else if (bufferLoc.X > model->inputLocB.X + 4 && bufferLoc.X < model->inputLocB.X + 9) {
					if (bufferLoc.Y == model->inputLocB.Y + 5 && model->colorA != model->RGB_B)
						model->colorB = model->RGB_B;
					else if (bufferLoc.Y == model->inputLocB.Y + 7 && model->colorA != model->RGB_M)
						model->colorB = model->RGB_M;

					model->UpdatePlayerColors();
				}
			}
		}
		break;
	}
	}
}

void GomokuController::ProcessKeyEvent(KEY_EVENT_RECORD const & ker)
{
	if (model->hasGameStarted) {
		if (isgraph(ker.uChar.AsciiChar) && ker.uChar.AsciiChar == 'r')
		{
			model->clickCount = 0;
			model->outer.PaintConsoleBackground(BACKGROUND_WHITE);
		
		
			model->ResetBoardVector();
			model->gs = GomokuModel::TITLEVIEW;
			model->Notify();
			//PrintGreetingMsg();
			//model->DrawGomokuGameBoard();
			//model->AddCoordinates();
			model->gs = GomokuModel::BOARDVIEW;
			model->Notify();
		}
	}
	else {
		if (ker.wVirtualKeyCode == VK_RETURN && model->nameA != "" && model->nameB != "") {
			// start playing game
			model->hasGameStarted = true;
		}
		// BACKSPACE key pressed
		else if (ker.bKeyDown && ker.wVirtualKeyCode == VK_BACK) {
			if (model->inputLoc.X == model->inputLocA.X) {
				if (model->nameA.size() > 0) {
					for (size_t i = 0; i < model->nameA.size(); i++)
					{
						COORD paintLoc{ model->inputLoc.X + (SHORT)i, model->inputLoc.Y };
						model->outer.RecoverCellClicked(BACKGROUND_WHITE, paintLoc);
					}
					model->nameA = model->nameA.substr(0, model->nameA.size() - 1);
					model->outer.WriteConsoleWindowMsg(model->nameA.c_str(), (DWORD)model->nameA.size(), model->inputLoc);
					// reset cursor location
					COORD cursorLoc{ model->inputLocA.X + (SHORT)model->nameA.size(), model->inputLocA.Y };
					model->outer.SetCursorLocation(cursorLoc);
				}
			}
			else {
				if (model->nameB.size() > 0) {
					for (size_t i = 0; i < model->nameB.size(); i++)
					{
						COORD paintLoc{ model->inputLoc.X + (SHORT)i, model->inputLoc.Y };
						model->outer.RecoverCellClicked(BACKGROUND_WHITE, paintLoc);
					}
					model->nameB = model->nameB.substr(0, model->nameB.size() - 1);
					model->outer.WriteConsoleWindowMsg(model->nameB.c_str(), (DWORD)model->nameB.size(), model->inputLoc);
					// reset cursor location
					COORD cursorLoc{ model->inputLocB.X + (SHORT)model->nameB.size(), model->inputLocB.Y };
					model->outer.SetCursorLocation(cursorLoc);
				}
			}
		}
		// TAB key pressed
		else if (ker.bKeyDown && ker.wVirtualKeyCode == VK_TAB) {
			if (model->inputLoc.X == model->inputLocA.X) {
				model->inputLoc = model->inputLocB;
				// reset cursor location
				COORD cursorLoc{ model->inputLocB.X + (SHORT)model->nameB.size(), model->inputLocB.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
			else {
				model->inputLoc = model->inputLocA;
				// reset cursor location
				COORD cursorLoc{ model->inputLocA.X + (SHORT)model->nameA.size(), model->inputLocA.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
		}
		else if (ker.bKeyDown && ker.wVirtualKeyCode != VK_RETURN && ker.wVirtualKeyCode != VK_CAPITAL) {
			if (model->inputLoc.X == model->inputLocA.X) {
				model->nameA += ker.uChar.AsciiChar;
				model->outer.WriteConsoleWindowMsg(model->nameA.c_str(), (DWORD)model->nameA.size(), model->inputLoc);
				// reset cursor location
				COORD cursorLoc{ model->inputLocA.X + (SHORT)model->nameA.size(), model->inputLocA.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
			else {
				model->nameB += ker.uChar.AsciiChar;
				model->outer.WriteConsoleWindowMsg(model->nameB.c_str(), (DWORD)model->nameB.size(), model->inputLoc);
				// reset cursor location
				COORD cursorLoc{ model->inputLocB.X + (SHORT)model->nameB.size(), model->inputLocB.Y };
				model->outer.SetCursorLocation(cursorLoc);
			}
		}
	}

}
