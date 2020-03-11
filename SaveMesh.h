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

class SaveMesh
{
private:
	SmLogger m_log;
	MeshSaver m_saver;

public:
	Ptr<Application> m_app = nullptr;
	Ptr<UserInterface> m_ui = nullptr;

	bool init(void);
	bool stop(void);
};

extern SaveMesh theSaveMesh;
