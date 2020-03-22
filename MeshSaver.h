/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#pragma once

#define TRIPPLE_TEST

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

    MeshFile m_TriMeshFile; // file for TriangleMesh'es
    MeshFile m_PolyMeshFile; // file for PolygonMesh'es

    size_t m_iMeshCount = 0; // number of MeshBody'es
    int m_iNumOfTriPointChunks = 0; // total number of point vectors of all TriangleMesh'es of all MeshBodies
    int m_iNumOfPolyPointChunks = 0; // total number of point vectors of all PolygonMesh'es of all MeshBodies
    /* int */ size_t m_iNumOfTriPoints = 0; // total number of points (nodes) in all TriangleMesh'es
    int m_iNumOfPolyPoints = 0; // total number of points (nodes) in all PolygonMesh'es
    int m_iNumOfTriElemChunks = 0; // total number of triangle vectors in all TriangleMesh'es of all MeshBodies
    int m_iNumOfPolyElemChunks = 0; // total number of triangle, quad and pollygon vectors in all PolygonMesh'es of all MeshBodies
    int m_iNumOfTriElems = 0; // total number of polygons (elements) in all TriangleMesh'es
    int m_iNumOfPolyElems = 0; // total number of polygons (elements) in all PolygonMesh'es

    // autoincrementing tags of chunks, both, point and elems
    int m_iTriEntityTag = 1; 
    int m_iPolyEntityTag = 1;
    
    // autoincrementing tags of points
    int m_iTriPointTag = 1; 
    int m_iPolyPointTag = 1;

    // autoincrementing tags of elems
    int m_iTriElemTag = 1;
    int m_iPolyElemTag = 1;

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

    bool writeMeshBodyPoints(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMeshPoints() or writeTriangleMeshPoints()
    bool writePolygonMeshPoints(Ptr<PolygonMesh> poly_mesh); // called from writeMeshBodyPoints(); calls writePoints()
    bool writeTriangleMeshPoints(Ptr<TriangleMesh> tri_mesh); // called from writeMeshBodyPoints(); calls writePoints()

    bool writeMeshBodyElems(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls writePolygonMeshElems() or writeTriangleMeshElems()
    bool writePolygonMeshElems(Ptr<PolygonMesh> poly_mesh); // called from writeMeshBodyElems(); calls writeElems()
    bool writeTriangleMeshElems(Ptr<TriangleMesh> tri_mesh); // called from writeMeshBodyElems(); calls writeElems()

    bool countMeshBody(Ptr<MeshBody> mesh); // called from saveActiveMesh(); calls countPolygonMesh() or countTriangleMesh()
    bool countPolygonMesh(Ptr<PolygonMesh> poly_mesh); // called from countMeshBody()
    bool countTriangleMesh(Ptr<TriangleMesh> tri_mesh); // called from countMeshBody()

    // called from saveActiveMesh(); 
    // num_of_entities -- either m_iNumOfTriPointChunks or m_iNumOfPolyPointChunks
    // num_of_points -- either m_iNumOfTriPoints or m_iNumOfPolyPoints
    // msh_file -- reference to either m_fTriMeshFile or m_fPolyMeshFile
    bool writeNodeHeader(int num_of_entities, /* int */ size_t num_of_points, MeshFile& msh_file);

    // called from saveActiveMesh(); 
    // num_of_entities -- total number of element chunks (entities), either m_iNumOfTriElemChunks or m_iNumOfPolyElemChunks
    // num_of_elems -- total number of polygons (elements) in all element chunks (entities), either m_iNumOfTriElems or m_iNumOfPolyElems
    // msh_file -- reference to either m_TriMeshFile or m_PolyMeshFile
    bool writeElemsHeader(int num_of_entities, int num_of_elems, MeshFile& msh_file);

    // called from writePolygonMeshPoints() or writeTriangleMeshPoints()
    // entity_tag -- reference to either m_iTriEntityTag or m_iPolyEntityTag
    // point_tag -- reference to either m_iTriPointTag or m_iPolyPointTag
    // msh_file -- reference to either m_fTriMeshFile or m_fPolyMeshFile
    bool writePoints(vector<Ptr<Point3D>>& points, int& entity_tag, int& point_tag, MeshFile& msh_file);

    // called from writePolygonMeshElems() or writeTriangleMeshElems()
    // entity_tag -- reference to either m_iTriEntityTag or m_iPolyEntityTag
    // elem_tag -- reference to either m_iTriElemTag or m_iPolyElemTag
    // msh_file -- reference to either m_fTriMeshFile or m_fPolyMeshFile
    bool writeElems(vector<int>& triangles, vector<int>& quads, vector<int>& polygons, int& entity_tag, int& elem_tag, MeshFile& msh_file);
};
