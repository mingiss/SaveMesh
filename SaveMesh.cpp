/* ------------------------------
 * SaveMesh plug-in entry
 */

#include <iostream> 
#include <sstream>
#include <Windows.h>

#include "SaveMesh.h"
// #include "SaveMeshDialog.h"
#include "SmToolBox.h"

using namespace std;

DWORD WINAPI PollingTheApp(_In_ LPVOID lpParameter)
{
	while (theSaveMesh.m_bPollThreadSemaphore)
	{
		// theSaveMesh.m_log.msg("polling");

		stringstream out_str;
		out_str << theSaveMesh.m_iPollCnt++;
		theSaveMesh.m_log.msg(out_str.str().c_str());

		theSaveMesh.m_saver.saveActiveMesh();

		Sleep(POLLING_TIMEOUT);
	}

	return(0);
}

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

//	if (retc) retc = m_saver.saveActiveMesh();

	if (retc)
	{
		m_hPollThread = CreateThread(NULL, 100000, PollingTheApp, NULL, 0, &m_iPollThread);
		if (!m_hPollThread)
		{
			theSaveMesh.m_ui->messageBox("Could not create polling thread!");
			retc = false;
		}
	}

	return retc;
}

bool SaveMesh::stop(void)
{
	m_bPollThreadSemaphore = false;

	if (theSaveMesh.m_ui)
	{
		theSaveMesh.m_ui->messageBox("SaveMesh stopped");
		theSaveMesh.m_ui = nullptr;
	}

	Sleep(2 * POLLING_TIMEOUT);

	return true;
}


SaveMesh theSaveMesh;

#if TRUE
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
#endif
