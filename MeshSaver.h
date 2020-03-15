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
    bool saveActiveMesh(void); // searches for active document and calls writeMeshBody() for each found mesh
    bool writeMeshBody(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMesh() or writeTriangleMesh()
    bool writePolygonMesh(Ptr<PolygonMesh> poly_mesh); // called from writeMeshBody(); calls writePoints()
    bool writeTriangleMesh(Ptr<TriangleMesh> tri_mesh); // called from writeMeshBody(); calls writePoints()
    bool writePoints(vector<Ptr<Point3D>>* points); // called from writePolygonMesh() or writeTriangleMesh()
};
