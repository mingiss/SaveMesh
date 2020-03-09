
#include <string>
#include <iostream> 
#include <sstream>
#include <fstream>

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

// #include "SaveMeshDialog.h"

using namespace std;

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

Ptr<Application> app;
Ptr<UserInterface> ui;

extern "C" XI_EXPORT bool run(const char* context)
{
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

//	SaveMeshDialog dlg();

	stringstream log_fname;
	log_fname << getenv("TEMP") << "/SaveMesh.log";

	ofstream log_file;
	log_file.open(log_fname.str(), ios::app);

	log_file << "The mesh:" << endl;


	log_file.close();

	ui->messageBox(log_fname.str());
	ui->messageBox("The mesh is saved");

	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (ui)
	{
		// ui->messageBox("SaveMesh stopped");
		ui = nullptr;
	}

	return true;
}


#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN
