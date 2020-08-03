/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		GomokuModel.hpp
*/

#pragma once
#include <haohan/ObservablePublisher.hpp>
#include <haohan/OuterWrapper.hpp>

class GomokuModel : public ObservablePublisher
{
public:
	enum GameStatus
	{
		HOMEVIEW,
		TITLEVIEW,
		BOARDVIEW,
		SCOREVIEW,
		PLAYERVIEW,
		MOVELISTVIEW
	};

	GameStatus gs;
	OuterWrapper outer;

	bool gameOver = false;
	bool hasGameStarted = false;
	int clickCount = 0;

	// vector for player 1
	std::vector<std::vector<int>> aPieces;
	// vector for player 2
	std::vector<std::vector<int>> bPieces;

	//vector for coordinates
	std::vector<char> coordinates;

	//vector for player A moves
	std::vector<COORD> movesA;
	//vector for player B moves
	std::vector<COORD> movesB;

	// player names
	std::string nameA = "";
	std::string nameB = "";

	// player scores
	unsigned int scoreA = 0;
	unsigned int scoreB = 0;

	// cursor loc
	COORD inputLocA = { 21, 13 };
	COORD inputLocB = { 51, 13 };

	COORD inputLoc = inputLocA;

	// colors
	static WORD const RGB_R = BACKGROUND_RED;
	static WORD const RGB_G = BACKGROUND_GREEN;
	static WORD const RGB_B = BACKGROUND_BLUE;

	static WORD const RGB_Y = RGB_R | RGB_G;
	static WORD const RGB_M = RGB_R | RGB_B;
	static WORD const RGB_C = RGB_B | RGB_G;

	// player colors
	WORD colorA = RGB_R;
	WORD colorB = RGB_G;

	// functions
	void DrawGomokuGameBoard();
	void AddCoordinates();
	void FillBoardVector();
	void ResetBoardVector();

	void UpdatePlayerColors();
	void SetPlayerColorPalette(COORD loc);
	// this function will be called 
	// if the size of loc is greater than 5
	// and checking if there are 5 pieces in row
	// if so, TRUE will be returned
	bool FiveInRow(std::vector<std::vector<int>> v);
	
};