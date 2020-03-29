/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 *
 */

#pragma once

// #define TRIPLE_TEST

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include "SmLogger.h"
#include "MeshFile.h"

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

class MeshSaver
{
private:
    Ptr<Application> m_app = nullptr;
    Ptr<UserInterface> m_ui = nullptr;
    SmLogger *m_plog = nullptr;
    string m_sAppName;

    MeshFile m_TriMeshFile; // file for TriangleMesh'es
    MeshFile m_PolyMeshFile; // file for PolygonMesh'es

    size_t m_iMeshCount = 0; // number of MeshBody'es

    // autoincrementing tags of entities, both, point and elements
    int m_iTriEntityTag = 1; 
    int m_iPolyEntityTag = 1;
    
    // autoincrementing tags of points
    int m_iTriPointTag = 1; 
    int m_iPolyPointTag = 1;

    // autoincrementing tags of elements
    int m_iTriElemTag = 1;
    int m_iPolyElemTag = 1;

    // first point tag in corresponding point entity to be used in tag shift of elements
    /* int */ size_t m_iTriFirstPointTag = 1;
    /* int */ size_t m_iPolyFirstPointTag = 1;

#ifdef TRIPLE_TEST
    double m_dCoordFact = 1.;
#endif

    // opens both mesh files for writing
    // called from saveActiveMesh()
    // calls openMeshFile()
    bool openMeshFiles(void);

    // closes both mesh files
    // called from saveActiveMesh()
    bool closeMeshFiles(void);

    bool writeMeshBodyPoints(const Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMeshPoints() or writeTriangleMeshPoints()
    bool writePolygonMeshPoints(const Ptr<PolygonMesh> poly_mesh); // called from writeMeshBodyPoints(); calls writePoints()
    bool writeTriangleMeshPoints(const Ptr<TriangleMesh> tri_mesh); // called from writeMeshBodyPoints(); calls writePoints()

    bool writeMeshBodyElems(const Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMeshElems() or writeTriangleMeshElems()
    bool writePolygonMeshElems(const Ptr<PolygonMesh> poly_mesh); // called from writeMeshBodyElems(); calls writeElems()
    bool writeTriangleMeshElems(const Ptr<TriangleMesh> tri_mesh); // called from writeMeshBodyElems(); calls writeElems()

    bool countMeshBody(const Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls countPolygonMesh() or countTriangleMesh()
    bool countPolygonMesh(const Ptr<PolygonMesh> poly_mesh); // called from countMeshBody()
    bool countTriangleMesh(const Ptr<TriangleMesh> tri_mesh); // called from countMeshBody()

#ifdef TRIPLE_TEST
    bool scalePoints(vector<Ptr<Point3D>>& points);
#endif

public:
    // all return values: true in case of success, false -- error

    bool init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger* plog, const char* app_name);
    bool saveActiveMesh(void); // searches for active document and calls writeMeshBody() for each found mesh
};
