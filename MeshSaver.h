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

#define MSH_VERS 4.1

enum MshFormats
{
    ascii,  // 0
    bin     // 1
};

class MeshSaver
{
private:
    Ptr<Application> m_app = nullptr;
    Ptr<UserInterface> m_ui = nullptr;
    SmLogger *m_plog = nullptr;
    string m_sAppName;

public:
    bool init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog, char* app_name);
    bool saveActiveMesh(void); // searches for active document and calls writeMeshBody() for each found mesh
    bool writeMeshBody(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMesh() or writeTriangleMesh()
    bool writePolygonMesh(Ptr<PolygonMesh> poly_mesh); // called from writeMeshBody(); calls writePoints()
    bool writeTriangleMesh(Ptr<TriangleMesh> tri_mesh); // called from writeMeshBody(); calls writePoints()
    bool writePoints(vector<Ptr<Point3D>>* points, char* fname); // called from writePolygonMesh() or writeTriangleMesh(); fname -- .msh file name without a path nor extension
};
