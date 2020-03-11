/* ------------------------------
 * SaveMesh plug-in messages
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

class SmLogger
{
private:
    Ptr<UserInterface> m_ui = nullptr;
    string m_sLogFname;

public:
    bool init(Ptr<UserInterface> ui, char *app_name);
    void msg(const char* str);
};
