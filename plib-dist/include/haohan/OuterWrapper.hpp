/*!	\author		Haohan Liu
	\date		2019-03-04
*/
#pragma once
#include <haohan/InnerWrapper.hpp>

// Colours
WORD const FOREGROUND_BLACK = 0;
WORD const FOREGROUND_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
WORD const BACKGROUND_BLACK = 0;
WORD const BACKGROUND_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;

//struct OriginalConsoleState {
//	// System Data
//	//HANDLE hConsoleInput, hConsoleOutput;
//	CONSOLE_SCREEN_BUFFER_INFO	originalCSBI;
//	CONSOLE_CURSOR_INFO			originalCCI;
//	std::vector<CHAR_INFO>		originalBuffer;
//	COORD						originalBufferCoord;
//	DWORD						originalConsoleMode;
//	WORD						currentConsoleWidth = 0;
//
//	std::string					_originalTitle;
//	SMALL_RECT					_bufferRect{ 0 };
//};

class OuterWrapper {
	typedef bool(*CtrlHandler)(unsigned long);

private:
	InnerWrapper innerInstance;
	OriginalConsoleState oConsoleState;
public:
	OuterWrapper();
	~OuterWrapper();

	InnerWrapper GetInnerInstance();
	OriginalConsoleState GetConsoleState();

	CONSOLE_SCREEN_BUFFER_INFO GetConsoleWindowBufferInfo();
	void FillConsoleWindowWithChar(CHAR cCharacter, DWORD nLength, COORD dwWriteCoord);

	void SaveConsoleState();
	void RestoreConsoleState();

	void SetCursorLocation(COORD loc);

	void SetConsoleWindowTitle(LPCSTR lpConsoleTitle);
	void ResizeConsoleWindow(int WINDOW_WIDTH, int WINDOW_HEIGHT);
	void SetCtrlHandler(CtrlHandler handler, bool b);
	void HideConsoleWindowCursor();
	void PaintConsoleBackground(WORD colour);
	void PaintCellClicked(WORD colour, COORD bufferLoc);
	void WriteConsoleWindowMsg(LPCSTR msg, DWORD nLength, COORD loc);
	void RecoverCellClicked(WORD colour, COORD bufferLoc);

	HANDLE GetConsoleWindowInput();
};