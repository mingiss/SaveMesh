/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#pragma once

#include <iostream> 
#include <sstream>
#include <fstream>

#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

#include "GmshDefines.h"
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

extern int ElemSizes[MSH_MAX_NUM + 1];

class MeshFile
{
public:
    string m_sMeshFName;
    ofstream m_fMeshFile;
};

class MeshSaver
{
private:
    Ptr<Application> m_app = nullptr;
    Ptr<UserInterface> m_ui = nullptr;
    SmLogger *m_plog = nullptr;
    string m_sAppName;

    MeshFile m_TriMeshFile;
    MeshFile m_PolyMeshFile;

public:
    // all return values: true in case of success, false -- error

    bool init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog, char* app_name);
    bool saveActiveMesh(void); // searches for active document and calls writeMeshBody() for each found mesh
    
    // opens the mesh file for writing
    // called from openMeshFiles()
    // out: msh_file -- reference to either m_TriMeshFile or m_PolyMeshFile
    bool openMeshFile(MeshFile& msh_file);

    // opens both mesh files for writing
    // called from saveActiveMesh()
    // calls openMeshFile()
    bool openMeshFiles(void);

    // closes the mesh file
    // called from closeMeshFiles()
    // out: msh_file -- reference to either m_TriMeshFile or m_PolyMeshFile
    bool closeMeshFile(MeshFile& msh_file);

    // closes both mesh files
    // called from saveActiveMesh()
    bool closeMeshFiles(void);

    bool writeMeshBody(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMesh() or writeTriangleMesh()
    bool writePolygonMesh(Ptr<PolygonMesh> poly_mesh); // called from writeMeshBody(); calls writePoints()
    bool writeTriangleMesh(Ptr<TriangleMesh> tri_mesh); // called from writeMeshBody(); calls writePoints()
    
    // called from writePolygonMesh() or writeTriangleMesh()
    // msh_file -- reference to either m_fTriMeshFile or m_fPolyMeshFile
    bool writePoints(vector<Ptr<Point3D>>& points, vector<int>& triangles, vector<int>& quads, vector<int>& polygons, MeshFile& msh_file);
};
