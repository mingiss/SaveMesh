/* --------------------------------------
 * MeshFile
 *      supporting .msh file format
 *      look: TODO: link to .msh file format
 *
 */

#pragma once

#include <iostream> 
#include <sstream>
#include <fstream>

#include "GmshDefines.h"
#include "SmLogger.h"

extern int ElemSizes[MSH_MAX_NUM + 1];

#define MSH_VERS 4.1

enum MshFormats
{
    ascii,  // 0
    bin     // 1
};

class ElemEntity
{
public:
    int m_iElemType = MSH_TRI_3;
    vector<int> m_Elems;
};

class MeshFile
{
private:
    SmLogger* m_plog = nullptr;

public:
    string m_sMeshFName;
    ofstream m_fMeshFile;

    int m_iNumOfPointEntities = 0; // Fusion 360: total number of point vectors of all TriangleMesh'es or PolygonMesh'es of all MeshBodies
    /* int */ size_t m_iNumOfPoints = 0; // Fusion 360: total number of points (nodes) in all TriangleMesh'es or PolygonMesh'es
    int m_iNumOfElemEntities = 0; // Fusion 360: total number of triangle vectors in all TriangleMesh'es or PolygonMesh'es of all MeshBodies
    int m_iNumOfElems = 0; // Fusion 360: total number of elements in all TriangleMesh'es or PolygonMesh'es

    bool init(SmLogger* plog);
    bool open(const char* fname); // opens the file for write
    bool close(void); // close the written file

    bool writeNodeHeader(void); // m_iNumOfPointEntities and m_iNumOfPoints should be calculated before calling the method
    bool writeElemsHeader(void); // m_iNumOfElemEntities and m_iNumOfElems should be calculated before calling the method

    // entity_tag -- reference to current entity (point vector being output) counter; Fusion 360 MeshSaver: either m_iTriEntityTag or m_iPolyEntityTag
    // point_tag -- reference to current node (point) counter; Fusion 360 MeshSaver: either m_iTriPointTag or m_iPolyPointTag
    bool writePoints(const vector<Ptr<Point3D>>& points, int& entity_tag, int& point_tag);

    // entity_tag -- reference to current entity (element vector being output) counter; Fusion 360 MeshSaver: either m_iTriEntityTag or m_iPolyEntityTag
    // elem_tag -- reference to current element (polygon) counter; Fusion 360 MeshSaver: either m_iTriElemTag or m_iPolyElemTag
    // first_point_tag -- reference to the tag of the first node (point), corresponding to the current element entity; Fusion 360 MeshSaver: either m_iTriFirstPointTag or m_iPolyFirstPointTag
    bool writeElems(const vector<ElemEntity>& elems, int& entity_tag, int& elem_tag, const /* int& */ size_t& first_point_tag);
};
