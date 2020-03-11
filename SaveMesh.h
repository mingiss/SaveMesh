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

extern Ptr<Application> app;
extern Ptr<UserInterface> ui;

class SaveMesh
{
private:
	string m_sLogFname;

public:
	void init();
	void logMsg(const char* msg);
	bool saveActiveMesh();
};
