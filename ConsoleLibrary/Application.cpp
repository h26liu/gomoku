/*!	\author		Haohan Liu
	\date		2019-04-15
	\file		Application.cpp
*/

#include <haohan/Application.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

Application* Application::thisApp_sm = nullptr;

int main(int argc, char* argv[]) try {
#ifdef _DEBUG
	// Enable CRT memory leak checking
	int dbgFlags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
	dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
	dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(dbgFlags);
#endif
	return Application::thisApp_sm->main(argc, argv);
}
catch (std::exception const& e) {
	cerr << "std::exception:" << e.what() << endl;
	return EXIT_FAILURE;
}
catch (...) {
	cerr << "Error: an exception has been caught...\n";
	return EXIT_FAILURE;
}

Application::Application() {
	if (thisApp_sm) {
		cerr << "Error: Console App already initialized!\nAborting...";
		exit(EXIT_FAILURE);
	}
	thisApp_sm = this;
}

int Application::main(int argc, char* argv[]) {
	args_m.assign(argv, argv + argc);
	return execute();
}

int Application::execute() {
	cout << "Console Paint/Gomoku Application framework.\n";
	return EXIT_SUCCESS;
}

void Application::MouseEventProc(MOUSE_EVENT_RECORD const & mer)
{
	cout << "MouseEventProc function, will be overriden in specific App" << endl;
}

void Application::ProcessKeyEvent(KEY_EVENT_RECORD const & ker)
{
	cout << "ProcessKeyEvent function, will be overriden in specific App" << endl;
}
