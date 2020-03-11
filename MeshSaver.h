/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#pragma once

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include "SmLogger.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

class MeshSaver
{
private:
    Ptr<Application> m_app = nullptr;
    Ptr<UserInterface> m_ui = nullptr;
    SmLogger *m_plog = nullptr;

public:
    bool init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog);
    bool saveActiveMesh(void);
};
