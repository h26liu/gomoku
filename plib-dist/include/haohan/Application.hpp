/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		Application.hpp
*/

#pragma once

//#define _CRTDBG_MAP_ALLOC
#define _CRT_MAP_ALLOW
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <vector>

#ifdef _DEBUG
#define NEW_DBG new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define NEW_DBG new
#endif 

#if defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "plib-mt-gd.lib")
#elif defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "plib-mt-sgd.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "plib-mt.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "plib-mt-s.lib")
#endif

class Application {
protected:
	using Args = std::vector<std::string>;
private:
	static Application* thisApp_sm;
	friend int main(int argc, char* argv[]);
	int main(int argc, char* argv[]);
	Args args_m;
protected:
	Application();
	virtual int execute();
	Args const& get_args() { return args_m; }

	virtual void MouseEventProc(MOUSE_EVENT_RECORD const& mer);
	virtual void ProcessKeyEvent(KEY_EVENT_RECORD const& ker);
};