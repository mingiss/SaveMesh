/* ------------------------------
 * SaveMesh plug-in entry
 */

#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include "SmLogger.h"
#include "MeshSaver.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

#define APP_NAME "SaveMesh"

#define POLLING_TIMEOUT 3000

class SaveMesh
{
private:
	HANDLE m_hPollThread = NULL;
	DWORD m_iPollThread = 0;

public:
	MeshSaver m_saver;
	SmLogger m_log;

	volatile int m_iPollCnt = 0;
	volatile bool m_bPollThreadSemaphore = true; // false -- the thread should end

	Ptr<Application> m_app = nullptr;
	Ptr<UserInterface> m_ui = nullptr;

	bool init(void);
	bool stop(void);
};

extern SaveMesh theSaveMesh;
