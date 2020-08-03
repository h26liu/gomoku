/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuView.cpp
*/

#include "GomokuView.hpp"

void BoardView::Update()
{
	if (model->gs == GomokuModel::BOARDVIEW)
	{
		model->DrawGomokuGameBoard();
		model->AddCoordinates();
	}
}

void HomeView::Update()
{

	if (model->gs == GomokuModel::HOMEVIEW)
	{
		//*************************************
		// set up view
		//*************************************
		COORD loc;
		std::string msg = "";
		// draw layout
		loc.X = 10;
		loc.Y = 7;
		for (size_t i = 0; i < 61; i++)
		{
			msg = "_";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.X++;
		}
		loc.X = 10;
		loc.Y = 10;
		for (size_t i = 0; i < 61; i++)
		{
			msg = "_";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.X++;
		}
		loc.X = 10;
		loc.Y = 22;
		for (size_t i = 0; i < 61; i++)
		{
			msg = "_";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.X++;
		}
		loc.X = 10;
		loc.Y = 8;
		for (size_t i = 0; i < 15; i++)
		{
			msg = "|";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.Y++;
		}
		loc.X = 70;
		loc.Y = 8;
		for (size_t i = 0; i < 15; i++)
		{
			msg = "|";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.Y++;
		}
		// TITLE
		loc.X = 20;
		loc.Y = 4;
		msg = "GOMOKU - by Haohan Liu, April 2019";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		// p1
		loc.X = 15;
		loc.Y = 9;
		msg = "Player 1";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 13;
		msg = "Name: ";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 16;
		msg = "Color: ";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		// p2
		loc.X = 45;
		loc.Y = 9;
		msg = "Player 2";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 13;
		msg = "Name: ";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 16;
		msg = "Color: ";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 10;
		loc.Y = 25;
		msg = "press ENTER to start game...";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

		// display default player colors
		model->UpdatePlayerColors();

		// set cursor loc
		model->outer.SetCursorLocation(model->inputLocA);

		// set player color palette
		model->SetPlayerColorPalette(COORD{ model->inputLocA.X - 7, model->inputLocA.Y + 5 });
		model->SetPlayerColorPalette(COORD{ model->inputLocB.X - 7, model->inputLocB.Y + 5 });

	}

}

void ScoreView::Update()
{
	if (model->gs == GomokuModel::SCOREVIEW)
	{
		// User Scores
		COORD loc;
		loc.X = 58;
		loc.Y = 4;
		std::string msg = "SCORES";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 55;
		loc.Y = 5;
		//msg = "Player Moves (X, Y)";
		model->outer.WriteConsoleWindowMsg(std::to_string(model->scoreA).c_str(), (DWORD)std::to_string(model->scoreA).size(), loc);
		loc.X = 60;
		msg = " : ";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 67;
		model->outer.WriteConsoleWindowMsg(std::to_string(model->scoreB).c_str(), (DWORD)std::to_string(model->scoreB).size(), loc);
	}
}

void PlayerView::Update()
{
	if (model->gs == GomokuModel::PLAYERVIEW)
	{
	}
}

void MoveListView::Update()
{
	if (model->gs == GomokuModel::MOVELISTVIEW)
	{

		// print out moves
		COORD loc;

		std::string moves = "";
		loc.X = 50;
		loc.Y = 7;

		// clear the User Moves
		for (size_t i = 0; i < 21; i++)
		{
			loc.X = 50;
			for (size_t j = 0; j < 6; j++)
			{
				model->outer.RecoverCellClicked(BACKGROUND_WHITE, loc);
				loc.X++;
			}

			loc.X = 66;
			for (size_t j = 0; j < 6; j++)
			{
				model->outer.RecoverCellClicked(BACKGROUND_WHITE, loc);
				loc.X++;
			}

			loc.Y++;
		}

		loc.X = 50;
		loc.Y = 7;

		std::vector<COORD>::iterator it = model->movesA.begin();
		for (; it != model->movesA.end(); ++it) {
			// reset x
			loc.X = 50;

			moves = "(";
			COORD currentCoord = *it;
			moves += model->coordinates[currentCoord.X / 2 - 2];
			moves += ", ";
			moves += model->coordinates[27 - currentCoord.Y];
			moves += ")";

			model->outer.WriteConsoleWindowMsg(moves.c_str(), (DWORD)moves.size(), loc);

			// paint move background
			for (size_t i = 0; i < moves.size(); i++)
			{
				model->outer.PaintCellClicked(model->colorA, loc);
				loc.X++;
			}

			loc.Y += 1;
		}

		loc.X = 66;
		loc.Y = 7;

		it = model->movesB.begin();
		for (; it != model->movesB.end(); ++it) {
			// reset x
			loc.X = 66;

			moves = "(";
			COORD currentCoord = *it;
			moves += model->coordinates[currentCoord.X / 2 - 2];
			moves += ", ";
			moves += model->coordinates[27 - currentCoord.Y];
			moves += ")";

			model->outer.WriteConsoleWindowMsg(moves.c_str(), (DWORD)moves.size(), loc);

			// paint move background
			for (size_t i = 0; i < moves.size(); i++)
			{
				model->outer.PaintCellClicked(model->colorB, loc);
				loc.X++;
			}

			loc.Y += 1;
		}
	}
}

void TitleView::Update()
{
	if (model->gs == GomokuModel::TITLEVIEW)
	{
		std::string msg = "Have fun playing Gomoku!";
		COORD loc;
		loc.X = (40 - (SHORT)msg.size()) / 2;
		loc.Y = 2;
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

		loc.Y = 4;
		msg = "Ctrl + C to quit";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 5;
		msg = "press R to restart game";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.Y = 6;
		msg = "Right-Click mouse to undo";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

		// ************************************* 
		// player information
		// ************************************* 
		loc.X = 47;
		loc.Y = 2;

		// print out player names
		model->outer.PaintCellClicked(model->colorA, loc);
		loc.X += 2;
		model->outer.WriteConsoleWindowMsg(model->nameA.c_str(), (DWORD)model->nameA.size(), loc);
		loc.X += 15;
		model->outer.PaintCellClicked(model->colorB, loc);
		loc.X += 2;
		model->outer.WriteConsoleWindowMsg(model->nameB.c_str(), (DWORD)model->nameB.size(), loc);

		// draw moves section
		loc.X = 45;
		loc.Y = 3;
		for (size_t i = 0; i < 30; i++)
		{
			loc.X++;
			msg = "_";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		}
		loc.X = 45;
		loc.Y = 4;
		msg = "|";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 76;
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 45;
		loc.Y = 5;
		msg = "|";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 76;
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		loc.X = 45;
		loc.Y = 6;
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		for (size_t i = 0; i < 30; i++)
		{
			loc.X++;
			msg = "_";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
		}
		loc.X++;
		msg = "|";
		model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);

		loc.X = 45;
		loc.Y = 7;
		for (size_t i = 0; i < 22; i++)
		{
			msg = "|";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.Y++;
		}
		loc.X = 61;
		loc.Y = 7;
		for (size_t i = 0; i < 22; i++)
		{
			msg = "|";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.Y++;
		}
		loc.X = 76;
		loc.Y = 7;
		for (size_t i = 0; i < 22; i++)
		{
			msg = "|";
			model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			loc.Y++;
		}
		loc.X = 45;
		loc.Y = 28;
		for (size_t i = 0; i < 30; i++)
		{
			if (i != 15) {
				loc.X++;
				msg = "_";
				model->outer.WriteConsoleWindowMsg(msg.c_str(), (DWORD)msg.size(), loc);
			}
			else
				loc.X++;
		}
	}
}
