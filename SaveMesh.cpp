/* ------------------------------
 * SaveMesh plug-in entry
 */

#include "SaveMesh.h"
// #include "SaveMeshDialog.h"
#include "SmToolBox.h"

using namespace std;

bool SaveMesh::init()
{
	bool retc = true;

	if (retc)
	{
		m_app = Application::get();
		if (!m_app)
			retc = false;
	}

	if (retc)
	{
		m_ui = m_app->userInterface();
		if (!m_ui)
			retc = false;
	}

	if (retc) retc = m_log.init(m_ui, "SaveMesh");

	if (retc) retc = m_saver.init(m_app, m_ui, &m_log);

//	if (retc) retc = theSmToolBoxHandler.init(theSaveMesh.m_ui);

	if (retc) retc = m_saver.saveActiveMesh();

	return retc;
}

bool SaveMesh::stop(void)
{
	if (theSaveMesh.m_ui)
	{
		theSaveMesh.m_ui->messageBox("SaveMesh stopped");
		theSaveMesh.m_ui = nullptr;
	}

	return true;
}


SaveMesh theSaveMesh;


extern "C" XI_EXPORT bool run(const char* context)
{
	bool retc = true;

	if (retc) retc = theSaveMesh.init();
	
	return retc;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	bool retc = true;

	retc &= theSaveMesh.stop();

	return retc;
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
