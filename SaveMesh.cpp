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

bool SaveMesh::init()
{
	m_app = Application::get();
	if (!m_app)
		return false;

	m_ui = m_app->userInterface();
	if (!m_ui)
		return false;

	// init log file
	stringstream log_fname;
	log_fname << getenv("TEMP") << "/SaveMesh.log";
	m_sLogFname = log_fname.str();

	return true;
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
	if (!m_app)
		return false;

	if (!m_ui)
		return false;

	// m_ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");

	Ptr<Documents> documents = m_app->documents();
	if (!documents)
	{
		m_ui->messageBox("No documents opened!");
		return false;
	}

	//	Ptr<Document> doc = documents->add(DocumentTypes::FusionDesignDocumentType);
	//	if (!doc)
	//	{
	//		m_ui->messageBox("Error retrieving a document!");
	//		return false;
	//	}

	Ptr<Product> product = m_app->activeProduct();
	if (!product)
	{
		m_ui->messageBox("No active design!");
		return false;
	}

	Ptr<Design> design = product;
	if (!design)
	{
		m_ui->messageBox("Error retrieving active design!");
		return false;
	}

	// Get the root component of the active design
	Ptr<Component> rootComp = design->rootComponent();
	if (!rootComp)
	{
		m_ui->messageBox("No components in the active design!");
		return false;
	}

	Ptr<MeshBodies> meshes = rootComp->meshBodies();
	{
		m_ui->messageBox("No meshes in the active design!");
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

	m_ui->messageBox(m_sLogFname);
	m_ui->messageBox("The mesh is saved");

	return true;
}


SaveMesh theSaveMesh;


extern "C" XI_EXPORT bool run(const char* context)
{
	bool retc = true;

	if (retc) retc = theSaveMesh.init();

	if (retc) retc = theCmdCreatedHandler.init(theSaveMesh.m_ui);

	return retc;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (theSaveMesh.m_ui)
	{
		theSaveMesh.m_ui->messageBox("SaveMesh stopped");
		theSaveMesh.m_ui = nullptr;
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
