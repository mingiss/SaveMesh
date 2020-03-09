
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

	ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");

	Ptr<Documents> documents = app->documents();
	if (!documents)
	{
		ui->messageBox("No documents opened!");
		return false;
	}

//	Ptr<Document> doc = documents->add(DocumentTypes::FusionDesignDocumentType);
//	if (!doc)
//	{
//		ui->messageBox("Error retrieving a document!");
//		return false;
//	}

	Ptr<Product> product = app->activeProduct();
	if (!product)
	{
		ui->messageBox("No active design!");
		return false;
	}

	Ptr<Design> design = product;
	if (!design)
	{
		ui->messageBox("Error retrieving active design!");
		return false;
	}

	// Get the root component of the active design
	Ptr<Component> rootComp = design->rootComponent();
	if (!rootComp)
	{
		ui->messageBox("No components in the active design!");
		return false;
	}

	Ptr<MeshBodies> meshes = rootComp->meshBodies();
	{
		ui->messageBox("No meshes in the active design!");
		return false;
	}

	stringstream log_fname;
	log_fname << getenv("TEMP") << "/SaveMesh.log";

	ofstream log_file;
	log_file.open(log_fname.str(), ios::app);

	log_file << "The mesh:" << endl;

	for (int ii = 0; ii < meshes->count(); ii++)
	{
		Ptr<MeshBody> mesh = meshes->item(ii);
		log_file << ii << ": " << mesh->name() << endl;
	}

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
