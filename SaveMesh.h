/* ------------------------------
 * SaveMesh plug-in entry
 */

#pragma once

#include <string>

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

using namespace std;

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

class SaveMesh
{
private:
	string m_sLogFname;

public:
	Ptr<Application> m_app = nullptr;
	Ptr<UserInterface> m_ui = nullptr;

	bool init();
	void logMsg(const char* msg);
	bool saveActiveMesh();
};

extern SaveMesh theSaveMesh;
