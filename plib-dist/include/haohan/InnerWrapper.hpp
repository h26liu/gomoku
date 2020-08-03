/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		InnerWrapper.hpp
*/

#pragma once
#include <Windows.h>
#undef min
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdint>
#include <algorithm>

struct OriginalConsoleState {
	// System Data
	//HANDLE hConsoleInput, hConsoleOutput;
	CONSOLE_SCREEN_BUFFER_INFO	originalCSBI;
	CONSOLE_CURSOR_INFO			originalCCI;
	std::vector<CHAR_INFO>		originalBuffer;
	COORD						originalBufferCoord;
	DWORD						originalConsoleMode;
	WORD						currentConsoleWidth = 0;

	std::string					_originalTitle;
	SMALL_RECT					_bufferRect{ 0 };
};

/* ErrorDescription */
std::string ErrorDescription(DWORD dwMessageID);

/* Console error exception class. */
class XError {
public:
	using id_type = decltype(GetLastError());
	using file_type = char const *;
	using string_type = std::string;
private:
	id_type code_;
	int	line_;
	file_type file_;
public:
	XError(int line, file_type file) : code_(GetLastError()), line_(line), file_(file) {}
	auto code() const -> id_type { return code_; }
	auto line() const -> int { return line_; }
	auto file() const -> file_type { return file_; }

	string_type msg() const {
		std::ostringstream oss;
		oss << "Error: " << code() << "\n";
		oss << ErrorDescription(code()) << "\n";
		oss << "In: " << file() << "\n";
		oss << "Line: " << line() << "\n";
		return oss.str();
	}
};
/* Console error exception throw helper macro. */
#define THROW_IF_CONSOLE_ERROR(res) if(!res) throw XError(__LINE__,__FILE__)
#define THROW_CONSOLE_ERROR() throw XError(__LINE__,__FILE__)

class InnerWrapper {
private:
	HANDLE hConsoleInput, hConsoleOutput;
public:
	InnerWrapper();
	~InnerWrapper();

	// Return ConsoleInput & ConsoleOutput
	HANDLE GetConsoleInput();
	HANDLE GetConsoleOutput();

	LPSTR GetConsoleTitleAWrapper(_In_ DWORD nSize);
	CONSOLE_SCREEN_BUFFER_INFO GetConsoleScreenBufferInfoWrapper();
	BOOL GetConsoleCursorInfoWrapper(_Out_ PCONSOLE_CURSOR_INFO lpConsoleCursorInfo);
	BOOL ReadConsoleOutputAWrapper(
		_Out_writes_(dwBufferSize.X * dwBufferSize.Y) PCHAR_INFO lpBuffer,
		_In_ COORD dwBufferSize,
		_In_ COORD dwBufferCoord,
		_Inout_ PSMALL_RECT lpReadRegion
	);
	BOOL GetConsoleModeWrapper(_Out_ LPDWORD lpMode);
	BOOL SetConsoleModeWrapper(_In_ DWORD dwMode);
	BOOL SetConsoleWindowInfoWrapper(
		_In_ BOOL bAbsolute,
		_In_ CONST SMALL_RECT* lpConsoleWindow
	);
	BOOL SetConsoleScreenBufferSizeWrapper(_In_ COORD dwSize);
	BOOL SetConsoleCursorInfoWrapper(_In_ CONST CONSOLE_CURSOR_INFO* lpConsoleCursorInfo);

	BOOL GetConsoleScreenBufferInfoWrapper(_Out_ PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo);

	// paint screen
	BOOL FillConsoleOutputCharacterAWrapper(
		_In_ CHAR cCharacter,
		_In_ DWORD nLength,
		_In_ COORD dwWriteCoord,
		_Out_ LPDWORD lpNumberOfCharsWritten
	);
	BOOL FillConsoleOutputAttributeWrapper(
		_In_ WORD wAttribute,
		_In_ DWORD nLength,
		_In_ COORD dwWriteCoord,
		_Out_ LPDWORD lpNumberOfAttrsWritten
	);

	// 
	BOOL WriteConsoleOutputAWrapper(
		_In_reads_(dwBufferSize.X * dwBufferSize.Y) CONST CHAR_INFO* lpBuffer,
		_In_ COORD dwBufferSize,
		_In_ COORD dwBufferCoord,
		_Inout_ PSMALL_RECT lpWriteRegion
	);
	BOOL WriteConsoleOutputCharacterAWrapper(
		_In_reads_(nLength) LPCSTR lpCharacter,
		_In_ DWORD nLength,
		_In_ COORD dwWriteCoord,
		_Out_ LPDWORD lpNumberOfCharsWritten
	);

	// restore cursor loc
	BOOL SetConsoleCursorPositionWrapper(_In_ COORD dwCursorPosition);

	void SetConsoleTitleAWrapper(_In_ LPCSTR lpConsoleTitle);
	void SetConsoleCtrlHandlerWrapper(_In_ PHANDLER_ROUTINE HandlerRoutine,_In_ BOOL Add);

};