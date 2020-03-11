/* ------------------------------
 * SaveMesh plug-in entry
 */

#include <iostream> 
#include <sstream>
#include <fstream>

#include "SaveMesh.h"
// #include "SaveMeshDialog.h"
#include "SMToolBox.h"

using namespace std;

Ptr<Application> app;
Ptr<UserInterface> ui;

SaveMesh theSaveMesh;

void SaveMesh::init()
{
	// init log file
	stringstream log_fname;
	log_fname << getenv("TEMP") << "/SaveMesh.log";
	m_sLogFname = log_fname.str();
}

void SaveMesh::logMsg(const char* msg)
{
	ofstream log_file;
	log_file.open(m_sLogFname, ios::app);
	log_file << msg << endl;
	log_file.close();
}

bool SaveMesh::saveActiveMesh()
{
	// ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");

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

	logMsg("The mesh:");

	for (int ii = 0; ii < meshes->count(); ii++)
	{
		Ptr<MeshBody> mesh = meshes->item(ii);
		stringstream out_str;
		out_str << ii << ": " << mesh->name() << endl;
		logMsg(out_str.str().c_str());
	}

	ui->messageBox(m_sLogFname);
	ui->messageBox("The mesh is saved");

	return true;
}

extern "C" XI_EXPORT bool run(const char* context)
{
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	theSaveMesh.init();

	// Create the command definition.
	Ptr<CommandDefinitions> commandDefinitions = ui->commandDefinitions();
	if (!commandDefinitions)
		return nullptr;

	// Get the existing command definition or create it if it doesn't already exist.
	Ptr<CommandDefinition> cmdDef = commandDefinitions->itemById("cmdInputsSample");
	if (!cmdDef)
	{
		cmdDef = commandDefinitions->addButtonDefinition("cmdInputsSample",
			"Command Inputs Sample",
			"Sample to demonstrate various command inputs.");
	}

	// Connect to the command created event.
	Ptr<CommandCreatedEvent> commandCreatedEvent = cmdDef->commandCreated();
	if (!commandCreatedEvent)
		return false;
	commandCreatedEvent->add(&_cmdCreatedHandler);

	// Execute the command definition.
	cmdDef->execute();

	// Prevent this module from being terminated when the script returns, because we are waiting for event handlers to fire.
	adsk::autoTerminate(false);

	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (ui)
	{
		ui->messageBox("SaveMesh stopped");
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
