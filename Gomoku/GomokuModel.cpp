/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuModel.cpp
*/

#include "GomokuModel.hpp"

void GomokuModel::DrawGomokuGameBoard()
{
	auto csbi = outer.GetConsoleWindowBufferInfo();

	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;

	SHORT x = 3;
	SHORT y = 10;
	bool done = false;

	for (SHORT i = 3; i < 42; i++)
	{
		COORD cursorHomeCoord{ i, 9 };

		outer.FillConsoleWindowWithChar('_', 1, cursorHomeCoord);
	}

	while (!done) {
		if (x == 42) {
			x = 3;
			y++;
		}

		if (y == csbi.dwSize.Y - 2) {
			done = true;
			break;
		}

		COORD cursorHomeCoord{ x, y };

		if (x % 2 == 0)
			outer.FillConsoleWindowWithChar('_', 1, cursorHomeCoord);
		else
			outer.FillConsoleWindowWithChar('|', 1, cursorHomeCoord);

		x++;
	}
}

void GomokuModel::AddCoordinates()
{
	coordinates = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z' };

	auto csbi = outer.GetConsoleWindowBufferInfo();

	SHORT x = 2;
	SHORT y = csbi.dwSize.Y - 3;
	// add for y axis
	for (size_t i = 0; i < csbi.dwSize.Y - 12; i++)
	{
		COORD cursorHomeCoord{ x, y };
		outer.FillConsoleWindowWithChar(coordinates[i], 1, cursorHomeCoord);
		y--;
	}
	// add for x axis
	y = csbi.dwSize.Y - 2;
	x = 4;
	for (size_t i = 0; i < 19; i++)
	{
		COORD cursorHomeCoord{ x, y };
		outer.FillConsoleWindowWithChar(coordinates[i], 1, cursorHomeCoord);
		x += 2;
	}
}

void GomokuModel::FillBoardVector()
{
	// fill vector with the size of game board ()
	std::vector<int> tempV;
	for (int i = 0; i <= 60; ++i) {
			tempV.push_back(0);
	}
	for (int i = 8; i <= 60; ++i) {
		aPieces.push_back(tempV);
	}
	for (int i = 8; i <= 60; ++i) {
		bPieces.push_back(tempV);
	}
	
}

void GomokuModel::ResetBoardVector()
{
	gameOver = false;

	aPieces.clear();
	bPieces.clear();

	movesA.clear();
	movesB.clear();

	scoreA = 0;
	scoreB = 0;

	gs = GomokuModel::SCOREVIEW;
	Notify();
	//UpdateUserScores();

	FillBoardVector();
}

void GomokuModel::UpdatePlayerColors()
{
	COORD loc;
	// player A
	loc.X = 25;
	loc.Y = 9;
	outer.PaintCellClicked(colorA, loc);
	// player B
	loc.X = 55;
	loc.Y = 9;
	outer.PaintCellClicked(colorB, loc);
}

void GomokuModel::SetPlayerColorPalette(COORD loc)
{
	{
		// THE FIRST ROW

		COORD loc_ = { loc.X, loc.Y };

		for (int i = loc.X; i < (int)loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_R, loc_);
		}

		loc.X += 5;
		for (int i = loc.X; i < (int)loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_G, loc_);
		}

		loc.X += 5;
		for (int i = loc.X; i < (int)loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_B, loc_);
		}

		// THE SECOND ROW

		loc.X -= 10;
		loc.Y += 2;
		loc_ = loc;

		for (int i = loc.X; i < loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_C, loc_);
		}

		loc.X += 5;
		for (int i = loc.X; i < loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_Y, loc_);
		}

		loc.X += 5;
		for (int i = loc.X; i < loc.X + 4; ++i) {
			loc_.X = i + 1;
			outer.PaintCellClicked(RGB_M, loc_);
		}
	}
}

bool GomokuModel::FiveInRow(std::vector<std::vector<int>> v)
{

	// check horizontal row
	for (int i = 0; i < 39; ++i) {
		for (int k = 0; k < 39; ++k) {
			if (v[i][k] != 0 &&
				v[i][k] == v[i][k + 2] &&
				v[i][k] == v[i][k + 4] &&
				v[i][k] == v[i][k + 6] &&
				v[i][k] == v[i][k + 8])
				return true;
		}
	}

	// check vertical row
	for (int i = 0; i < 39; ++i) {
		for (int k = 0; k < 39; ++k) {
			if (v[i][k] != 0 &&
				v[i][k] == v[i + 1][k] &&
				v[i][k] == v[i + 2][k] &&
				v[i][k] == v[i + 3][k] &&
				v[i][k] == v[i + 4][k])
				return true;
		}
	}

	// check others
	for (int i = 0; i < 42; ++i) {
		for (int k = 0; k < 42; ++k) {
			if (v[i][k] != 0 &&
				v[i][k] == v[i + 1][k + 2] &&
				v[i][k] == v[i + 2][k + 4] &&
				v[i][k] == v[i + 3][k + 6] &&
				v[i][k] == v[i + 4][k + 8])
				return true;
		}
	}
	for (int i = 0; i < 42; ++i) {
		for (int k = 0; k < 42; ++k) {
			if (k > 7 && v[i][k] != 0 &&
				v[i][k] == v[i + 1][k - 2] &&
				v[i][k] == v[i + 2][k - 4] &&
				v[i][k] == v[i + 3][k - 6] &&
				v[i][k] == v[i + 4][k - 8])
				return true;
		}
	}

	return false;
}

