/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		OuterWrapper.cpp
*/

#include <haohan/OuterWrapper.hpp>

using namespace std;

// 
//OriginalConsoleState oConsoleState;

OuterWrapper::OuterWrapper()
{
	
}

OuterWrapper::~OuterWrapper()
{
}

InnerWrapper OuterWrapper::GetInnerInstance()
{
	return innerInstance;
}

OriginalConsoleState OuterWrapper::GetConsoleState()
{
	return oConsoleState;
}

CONSOLE_SCREEN_BUFFER_INFO OuterWrapper::GetConsoleWindowBufferInfo()
{
	// Get the number of character cells in the current buffer
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	THROW_IF_CONSOLE_ERROR(innerInstance.GetConsoleScreenBufferInfoWrapper(&csbi));

	return csbi;
}

void OuterWrapper::FillConsoleWindowWithChar(CHAR cCharacter, DWORD nLength, COORD dwWriteCoord)
{
	DWORD charsWritten;
	// Fill the screen with Char
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputCharacterAWrapper(cCharacter, nLength, dwWriteCoord, &charsWritten));
}

//BOOL CtrlHandler(DWORD ctrlType) {
//	switch (ctrlType) {
//	case CTRL_C_EVENT:
//		oConsoleState.applicationQuitting = true;
//		return TRUE;
//	}
//
//	return FALSE;
//}

// 
void OuterWrapper::SaveConsoleState()
{
	// Save the original title
	//OriginalConsoleState oConsoleState;
	oConsoleState._originalTitle = innerInstance.GetConsoleTitleAWrapper(64 * 1024);

	// Get the old window/buffer size
	oConsoleState.originalCSBI = innerInstance.GetConsoleScreenBufferInfoWrapper();

	// Save the cursor
	THROW_IF_CONSOLE_ERROR(innerInstance.GetConsoleCursorInfoWrapper(&oConsoleState.originalCCI));

	// Save the desktop
	oConsoleState.originalBuffer.resize(size_t(oConsoleState.originalCSBI.dwSize.X)*oConsoleState.originalCSBI.dwSize.Y);
	oConsoleState.originalBufferCoord = COORD{ 0 };
	oConsoleState._bufferRect.Right = oConsoleState.originalCSBI.dwSize.X - 1;
	oConsoleState._bufferRect.Bottom = oConsoleState.originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(innerInstance.ReadConsoleOutputAWrapper(oConsoleState.originalBuffer.data(), oConsoleState.originalCSBI.dwSize, oConsoleState.originalBufferCoord, &oConsoleState._bufferRect));

	// Configure the console mode
	innerInstance.GetConsoleModeWrapper(&oConsoleState.originalConsoleMode);

	DWORD consoleMode = /*ENABLE_WINDOW_INPUT | */ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT;
	consoleMode |= ENABLE_EXTENDED_FLAGS;	// Stop windows from taking over the mouse.

	if (!innerInstance.SetConsoleModeWrapper(consoleMode)) {
		cerr << "\nERROR: could not set console mode.\n";
	}
}

void OuterWrapper::RestoreConsoleState()
{
	// Restore console mode
	if (!innerInstance.SetConsoleModeWrapper(oConsoleState.originalConsoleMode)) {
		cerr << "\nERROR: could not restore original console mode.\n";
	}

	// Restore the original settings/size
	SMALL_RECT sr{ 0 };
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleWindowInfoWrapper(TRUE, &sr));
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleScreenBufferSizeWrapper(oConsoleState.originalCSBI.dwSize));
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleWindowInfoWrapper(TRUE, &oConsoleState.originalCSBI.srWindow));

	// Restore the background colour and char filled
	oConsoleState._bufferRect = SMALL_RECT{ 0 };
	oConsoleState._bufferRect.Right = oConsoleState.originalCSBI.dwSize.X - 1;
	oConsoleState._bufferRect.Bottom = oConsoleState.originalCSBI.dwSize.Y - 1;
	THROW_IF_CONSOLE_ERROR(innerInstance.WriteConsoleOutputAWrapper(oConsoleState.originalBuffer.data(), oConsoleState.originalCSBI.dwSize, oConsoleState.originalBufferCoord, &oConsoleState._bufferRect));
	// Restore the title
	SetConsoleTitleA(oConsoleState._originalTitle.data());

	// Restore the cursor
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleCursorInfoWrapper(&oConsoleState.originalCCI));
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleCursorPositionWrapper(oConsoleState.originalCSBI.dwCursorPosition));
}

void OuterWrapper::SetCursorLocation(COORD loc)
{
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleCursorPositionWrapper(loc));
}

void OuterWrapper::SetConsoleWindowTitle(LPCSTR lpConsoleTitle)
{
	innerInstance.SetConsoleTitleAWrapper(lpConsoleTitle);
}

void OuterWrapper::ResizeConsoleWindow(int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	SMALL_RECT sr{ 0 };
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleWindowInfoWrapper(TRUE, &sr));

	COORD bufferSize;
	bufferSize.X = WINDOW_WIDTH;
	bufferSize.Y = WINDOW_HEIGHT;
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleScreenBufferSizeWrapper(bufferSize));

	CONSOLE_SCREEN_BUFFER_INFO sbi = innerInstance.GetConsoleScreenBufferInfoWrapper();

	sr.Top = sr.Left = 0;
	WINDOW_WIDTH = min((SHORT)WINDOW_WIDTH, sbi.dwMaximumWindowSize.X);
	WINDOW_HEIGHT = min((SHORT)WINDOW_HEIGHT, sbi.dwMaximumWindowSize.Y);
	sr.Right = WINDOW_WIDTH - 1;
	sr.Bottom = WINDOW_HEIGHT - 1;
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleWindowInfoWrapper(TRUE, &sr));
}

void OuterWrapper::SetCtrlHandler(CtrlHandler handler, bool b) {
	innerInstance.SetConsoleCtrlHandlerWrapper((PHANDLER_ROUTINE)handler, b);
}

void OuterWrapper::HideConsoleWindowCursor() {
	auto newCCI = oConsoleState.originalCCI;
	newCCI.bVisible = FALSE;
	THROW_IF_CONSOLE_ERROR(innerInstance.SetConsoleCursorInfoWrapper(&newCCI));
}

void OuterWrapper::PaintConsoleBackground(WORD colour)
{
	// Get the number of character cells in the current buffer
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	THROW_IF_CONSOLE_ERROR(innerInstance.GetConsoleScreenBufferInfoWrapper(&csbi));

	// Fill the entire screen area white
	DWORD charsWritten;
	DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD cursorHomeCoord{ 0, 0 };
	// Fill the screen with Char
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputCharacterAWrapper(' ', consoleSize, cursorHomeCoord, &charsWritten));
	// Change the screen colour
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputAttributeWrapper(colour, consoleSize, cursorHomeCoord, &charsWritten));
}

void OuterWrapper::PaintCellClicked(WORD colour, COORD bufferLoc)
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputAttributeWrapper(colour, 1, bufferLoc, &nCharsWritten));
}

void OuterWrapper::WriteConsoleWindowMsg(LPCSTR msg, DWORD nLength, COORD loc)
{
	DWORD charsWritten;
	innerInstance.WriteConsoleOutputCharacterAWrapper(msg, nLength, loc, &charsWritten);
}

void OuterWrapper::RecoverCellClicked(WORD colour, COORD bufferLoc)
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputCharacterAWrapper(' ', 1, bufferLoc, &nCharsWritten));
	THROW_IF_CONSOLE_ERROR(innerInstance.FillConsoleOutputAttributeWrapper(colour, 1, bufferLoc, &nCharsWritten));
}

HANDLE OuterWrapper::GetConsoleWindowInput()
{
	return innerInstance.GetConsoleInput();
}
