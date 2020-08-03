/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		InnerWrapper.cpp
*/

#include <haohan/InnerWrapper.hpp>

// default ctor
InnerWrapper::InnerWrapper()
{
	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
}

// destructor
InnerWrapper::~InnerWrapper()
{
	/*hConsoleInput = nullptr;
	hConsoleOutput = nullptr;*/
}

HANDLE InnerWrapper::GetConsoleInput()
{
	return hConsoleInput;
}

HANDLE InnerWrapper::GetConsoleOutput()
{
	return hConsoleOutput;
}

LPSTR InnerWrapper::GetConsoleTitleAWrapper(DWORD nSize)
{
	LPSTR result = new CHAR[nSize];
	GetConsoleTitleA(result, nSize);

	return result;
}

CONSOLE_SCREEN_BUFFER_INFO InnerWrapper::GetConsoleScreenBufferInfoWrapper()
{
	CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo;
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hConsoleOutput, &lpConsoleScreenBufferInfo));

	return lpConsoleScreenBufferInfo;
}

BOOL InnerWrapper::GetConsoleCursorInfoWrapper(PCONSOLE_CURSOR_INFO lpConsoleCursorInfo)
{
	return GetConsoleCursorInfo(hConsoleOutput, lpConsoleCursorInfo);
}

BOOL InnerWrapper::ReadConsoleOutputAWrapper(PCHAR_INFO lpBuffer, COORD dwBufferSize, COORD dwBufferCoord, PSMALL_RECT lpReadRegion)
{
	return ReadConsoleOutputA(hConsoleOutput, lpBuffer, dwBufferSize, dwBufferCoord, lpReadRegion);
}

BOOL InnerWrapper::GetConsoleModeWrapper(LPDWORD lpMode)
{
	return GetConsoleMode(hConsoleInput, lpMode);
}

BOOL InnerWrapper::SetConsoleModeWrapper(DWORD dwMode)
{
	return SetConsoleMode(hConsoleInput, dwMode);
}

BOOL InnerWrapper::SetConsoleWindowInfoWrapper(BOOL bAbsolute, const SMALL_RECT * lpConsoleWindow)
{
	return SetConsoleWindowInfo(hConsoleOutput, bAbsolute, lpConsoleWindow);
}

BOOL InnerWrapper::SetConsoleScreenBufferSizeWrapper(COORD dwSize)
{
	return SetConsoleScreenBufferSize(hConsoleOutput, dwSize);
}

BOOL InnerWrapper::SetConsoleCursorInfoWrapper(const CONSOLE_CURSOR_INFO * lpConsoleCursorInfo)
{
	return SetConsoleCursorInfo(hConsoleOutput, lpConsoleCursorInfo);
}

BOOL InnerWrapper::GetConsoleScreenBufferInfoWrapper(PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo)
{
	return GetConsoleScreenBufferInfo(hConsoleOutput, lpConsoleScreenBufferInfo);
}

BOOL InnerWrapper::FillConsoleOutputCharacterAWrapper(CHAR cCharacter, DWORD nLength, COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten)
{
	return FillConsoleOutputCharacterA(hConsoleOutput, cCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
}

BOOL InnerWrapper::FillConsoleOutputAttributeWrapper(WORD wAttribute, DWORD nLength, COORD dwWriteCoord, LPDWORD lpNumberOfAttrsWritten)
{
	return FillConsoleOutputAttribute(hConsoleOutput, wAttribute, nLength, dwWriteCoord, lpNumberOfAttrsWritten);
}

BOOL InnerWrapper::WriteConsoleOutputAWrapper(const CHAR_INFO * lpBuffer, COORD dwBufferSize, COORD dwBufferCoord, PSMALL_RECT lpWriteRegion)
{
	return WriteConsoleOutputA(hConsoleOutput, lpBuffer, dwBufferSize, dwBufferCoord, lpWriteRegion);
}

BOOL InnerWrapper::WriteConsoleOutputCharacterAWrapper(LPCSTR lpCharacter, DWORD nLength, COORD dwWriteCoord, LPDWORD lpNumberOfCharsWritten)
{
	return WriteConsoleOutputCharacterA(hConsoleOutput, lpCharacter, nLength, dwWriteCoord, lpNumberOfCharsWritten);
}

BOOL InnerWrapper::SetConsoleCursorPositionWrapper(COORD dwCursorPosition)
{
	return SetConsoleCursorPosition(hConsoleOutput, dwCursorPosition);
}

void InnerWrapper::SetConsoleTitleAWrapper(LPCSTR lpConsoleTitle)
{
	SetConsoleTitleA(lpConsoleTitle);
}

void InnerWrapper::SetConsoleCtrlHandlerWrapper(PHANDLER_ROUTINE HandlerRoutine, BOOL Add)
{
	// Install a control handler to trap ^C
	if (SetConsoleCtrlHandler(HandlerRoutine, Add)) {
		// std::cout << "Terminate program with Ctrl-C\n";
	}
	else {
		std::cerr << "ERROR: failed to install control handler." << std::endl;
	}
}

std::string ErrorDescription(DWORD dwMessageID)
{
		char* msg;
		auto c = FormatMessageA(
			/* flags */			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK,
			/* source*/			NULL,
			/* message ID */	dwMessageID,
			/* language */		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			/* buffer */		(LPSTR)&msg,
			/* size */			0,
			/* args */			NULL
		);

		std::string strMsg = (c == 0)
			? "unknown"
			: msg;
		LocalFree(msg);
		return strMsg;
}
