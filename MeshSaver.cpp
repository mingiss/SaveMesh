/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#include "MeshSaver.h"


bool MeshSaver::init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog, const char* app_name)
{
    bool retc = true;

    m_app = app;
    if (!m_app)
        retc = false;

    m_ui = ui;
    if (!m_ui)
        retc = false;

    m_plog = plog;
    if (!m_plog)
        retc = false;

    if (app_name)
        m_sAppName = app_name;
    else
        retc = false;

    retc &= m_TriMeshFile.init(m_plog);
    retc &= m_PolyMeshFile.init(m_plog);

    return retc;
}


bool MeshSaver::openMeshFiles(void)
{
    bool retc = true;

    string spath = string() + getenv("APPDATA") + "/Autodesk/Autodesk Fusion 360/API/AddIns/" + m_sAppName + "/" + m_sAppName + "_";
    string tri_mesh_fname = spath + "triangle" +
#ifdef TRIPLE_TEST
        "_3x" +
#endif
        ".msh";
    string poly_mesh_fname = spath + "polygon" +
#ifdef TRIPLE_TEST
        "_3x" +
#endif
        ".msh";

    if (retc) retc = m_TriMeshFile.open(tri_mesh_fname.c_str());
    if (retc) retc = m_PolyMeshFile.open(poly_mesh_fname.c_str());

    return retc;
}


bool MeshSaver::closeMeshFiles(void)
{
    bool retc = true;

    if (retc) retc = m_TriMeshFile.close();
    if (retc) retc = m_PolyMeshFile.close();

    m_TriMeshFile.m_sMeshFName.clear();
    m_PolyMeshFile.m_sMeshFName.clear();

    m_iMeshCount = 0;
    m_TriMeshFile.m_iNumOfPointEntities = 0;
    m_PolyMeshFile.m_iNumOfPointEntities = 0;
    m_TriMeshFile.m_iNumOfPoints = 0;
    m_PolyMeshFile.m_iNumOfPoints = 0;
    m_TriMeshFile.m_iNumOfElemEntities = 0;
    m_PolyMeshFile.m_iNumOfElemEntities = 0;
    m_TriMeshFile.m_iNumOfElems = 0;
    m_PolyMeshFile.m_iNumOfElems = 0;

    return retc;
}


#ifdef TRIPLE_TEST
bool MeshSaver::scalePoints(vector<Ptr<Point3D>>& points)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc)
    {
        for (vector<Ptr<Point3D>>::iterator it = points.begin(); (it != points.end()) && retc; it++)
        {
            vector<double> coords = (*it)->asArray();
            if (coords.size() == 0)
            {
                m_plog->msg(__func__, "Empty point coordinates!");
                retc = false;
            }
            if (retc)
            {
                for (vector<double>::iterator id = coords.begin(); (id != coords.end()) && retc; id++)
                    *id *= m_dCoordFact;

                retc = (*it)->setWithArray(coords);
                if (!retc)
                    m_plog->msg(__func__, "Unsuccessful rescaling of points!");
            }
        }
    }

    return retc;
}
#endif

bool MeshSaver::writeTriangleMeshPoints(Ptr<TriangleMesh> tri_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !tri_mesh)
    {
        m_plog->msg(__func__, "No TriangleMesh ptr given!");
        retc = false;
    }

    vector<Ptr<Point3D>> points;
    if (retc) points = tri_mesh->nodeCoordinates();

#ifdef TRIPLE_TEST
    if (retc) retc = scalePoints(points);
#endif

    if (retc)
    {
        m_plog->msg(__func__, "Writing TriangleMesh points...");
        retc = m_TriMeshFile.writePoints(points, m_iTriEntityTag, m_iTriPointTag);
    }

    return retc;
}


bool MeshSaver::writePolygonMeshPoints(Ptr<PolygonMesh> poly_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !poly_mesh)
    {
        m_plog->msg(__func__, "No PolygonMesh ptr given!");
        retc = false;
    }

    vector<Ptr<Point3D>> points;
    if (retc) points = poly_mesh->nodeCoordinates();

#ifdef TRIPLE_TEST
    if (retc) retc = scalePoints(points);
#endif

    if (retc)
    {
        m_plog->msg(__func__, "Writing PolygonMesh points...");
        retc = m_PolyMeshFile.writePoints(points, m_iPolyEntityTag, m_iPolyPointTag);
    }

    return retc;
}


bool MeshSaver::writeMeshBodyPoints(Ptr<MeshBody> mesh_body)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !mesh_body)
    {
        m_plog->msg(__func__, "No MeshBody ptr given!");
        retc = false;
    }

    Ptr<PolygonMesh> poly_mesh = nullptr;
    Ptr<TriangleMesh> tri_mesh = nullptr;
    if (retc)
    {
        m_plog->msg(__func__, "Looking for the PolygonMesh...");
        poly_mesh = mesh_body->mesh();
        if (!poly_mesh)
            m_plog->msg(__func__, "No PolygonMesh!");

        m_plog->msg(__func__, "Looking for the TriangleMesh...");
        tri_mesh = mesh_body->displayMesh();
        if (!tri_mesh)
            m_plog->msg(__func__, "No TriangleMesh!");
    }

    if (retc && poly_mesh)
        retc = writePolygonMeshPoints(poly_mesh);

    if (retc && tri_mesh)
        retc = writeTriangleMeshPoints(tri_mesh);

    return retc;
}


bool MeshSaver::writeTriangleMeshElems(Ptr<TriangleMesh> tri_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !tri_mesh)
    {
        m_plog->msg(__func__, "No TriangleMesh ptr given!");
        retc = false;
    }

    vector<ElemEntity> entities;

    if (retc)
    {
        ElemEntity triangles;
        triangles.m_iElemType = MSH_TRI_3;
        triangles.m_Elems = tri_mesh->nodeIndices();
        if (triangles.m_Elems.size() > 0)
            entities.push_back(triangles);
    }

    if (retc && (entities.size() == 0))
    {
        m_plog->msg(__func__, "No elems in TriangleMesh!");
        retc = false;
    }

    if (retc)
    {
        m_plog->msg(__func__, "Writing TriangleMesh elems...");
        retc = m_TriMeshFile.writeElems(entities, m_iTriEntityTag, m_iTriElemTag, m_iTriFirstPointTag);
    }

    if (retc)
        m_iTriFirstPointTag += tri_mesh->nodeCoordinates().size(); // tri_mesh->nodeCount();

    return retc;
}


bool MeshSaver::writePolygonMeshElems(Ptr<PolygonMesh> poly_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !poly_mesh)
    {
        m_plog->msg(__func__, "No PolygonMesh ptr given!");
        retc = false;
    }

    vector<ElemEntity> entities;

    if (retc)
    {
        ElemEntity triangles;
        triangles.m_iElemType = MSH_TRI_3;
        triangles.m_Elems = poly_mesh->triangleNodeIndices();
        if (triangles.m_Elems.size() > 0)
            entities.push_back(triangles);
    }

    if (retc)
    {
        ElemEntity quads;
        quads.m_iElemType = MSH_QUA_4;
        quads.m_Elems = poly_mesh->quadNodeIndices();
        if (quads.m_Elems.size() > 0)
            entities.push_back(quads);
    }

    if (retc)
    {
        ElemEntity polygons;
        polygons.m_iElemType = MSH_TET_4; // TODO: ???
        polygons.m_Elems = poly_mesh->polygonNodeIndices();
        if (polygons.m_Elems.size() > 0)
            entities.push_back(polygons);
    }

    if (retc && (entities.size() == 0))
    {
        m_plog->msg(__func__, "No elems in PolygonMesh!");
        retc = false;
    }

    if (retc)
    {
        m_plog->msg(__func__, "Write PolygonMesh elems...");
        retc = m_PolyMeshFile.writeElems(entities, m_iPolyEntityTag, m_iPolyElemTag, m_iPolyFirstPointTag);
    }

    if (retc)
        m_iPolyFirstPointTag += poly_mesh->nodeCount();

    return retc;
}


bool MeshSaver::writeMeshBodyElems(Ptr<MeshBody> mesh_body)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !mesh_body)
    {
        m_plog->msg(__func__, "No MeshBody ptr given!");
        retc = false;
    }

    Ptr<PolygonMesh> poly_mesh = nullptr;
    Ptr<TriangleMesh> tri_mesh = nullptr;
    if (retc)
    {
        m_plog->msg(__func__, "Looking for the PolygonMesh...");
        poly_mesh = mesh_body->mesh();
        if (!poly_mesh)
            m_plog->msg(__func__, "No PolygonMesh!");

        m_plog->msg(__func__, "Looking for the TriangleMesh...");
        tri_mesh = mesh_body->displayMesh();
        if (!tri_mesh)
            m_plog->msg(__func__, "No TriangleMesh!");
    }

    if (retc && poly_mesh)
        retc = writePolygonMeshElems(poly_mesh);

    if (retc && tri_mesh)
        retc = writeTriangleMeshElems(tri_mesh);

    return retc;
}


bool MeshSaver::countTriangleMesh(Ptr<TriangleMesh> tri_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !tri_mesh)
    {
        m_plog->msg(__func__, "No TriangleMesh ptr given!");
        retc = false;
    }

    if (retc)
    {
        vector<Ptr<Point3D>> points = tri_mesh->nodeCoordinates();
        size_t num_of_coords = points.size();

        if (num_of_coords > 0)
            m_TriMeshFile.m_iNumOfPointEntities++;
        else
            m_plog->msg(__func__, "TriangleMesh has no points!");

        int num_of_points = tri_mesh->nodeCount(); // erroneously returns count of triangle vertices
        m_TriMeshFile.m_iNumOfPoints += num_of_coords; //  num_of_points;
        stringstream out_str;
        out_str << "TriangleMesh points: " << num_of_points;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_coords != num_of_points)
        {
            stringstream out_str;
            out_str << "TriangleMesh points count conflict: " << num_of_coords;
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    if (retc)
    {
        int num_of_triangles = tri_mesh->triangleCount();
        m_TriMeshFile.m_iNumOfElems += num_of_triangles;

        if (num_of_triangles > 0)
            m_TriMeshFile.m_iNumOfElemEntities++;
        else
            m_plog->msg(__func__, "TriangleMesh has no elements!");

        vector<int> triangles = tri_mesh->nodeIndices();
        size_t num_of_vertices = triangles.size();

        stringstream out_str;
        out_str << "TriangleMesh triangles: " << num_of_triangles << "  vertices: " << num_of_vertices;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_vertices != num_of_triangles * ElemSizes[MSH_TRI_3])
            m_plog->msg(__func__, "TriangleMesh triangles vertex count conflict!");
    }

    return retc;
}


bool MeshSaver::countPolygonMesh(Ptr<PolygonMesh> poly_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !poly_mesh)
    {
        m_plog->msg(__func__, "No PolygonMesh ptr given!");
        retc = false;
    }

    if (retc)
    {
        vector<Ptr<Point3D>> points = poly_mesh->nodeCoordinates();
        size_t num_of_coords = points.size();

        if (num_of_coords > 0)
            m_PolyMeshFile.m_iNumOfPointEntities++;
        else
            m_plog->msg(__func__, "PolygonMesh has no points!");

        int num_of_points = poly_mesh->nodeCount();
        m_PolyMeshFile.m_iNumOfPoints += num_of_points;
        stringstream out_str;
        out_str << "PolygonMesh points: " << num_of_points;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_coords != num_of_points)
        {
            stringstream out_str;
            out_str << "PolygonMesh points count conflict: " << num_of_coords;
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    if (retc)
    {
        int num_of_triangles = poly_mesh->triangleCount();
        m_PolyMeshFile.m_iNumOfElems += num_of_triangles;

        if (num_of_triangles > 0)
            m_PolyMeshFile.m_iNumOfElemEntities++;

        vector<int> triangles = poly_mesh->triangleNodeIndices();
        size_t num_of_vertices = triangles.size();

        stringstream out_str;
        out_str << "PolygonMesh triangles: " << num_of_triangles << "  vertices: " << num_of_vertices;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_vertices != num_of_triangles * ElemSizes[MSH_TRI_3])
            m_plog->msg(__func__, "TriangleMesh triangles vertex count conflict!");
    }

    if (retc)
    {
        int num_of_quads = poly_mesh->quadCount();
        m_PolyMeshFile.m_iNumOfElems += num_of_quads;

        if (num_of_quads > 0)
            m_PolyMeshFile.m_iNumOfElemEntities++;

        vector<int> quads = poly_mesh->quadNodeIndices();
        size_t num_of_vertices = quads.size();

        stringstream out_str;
        out_str << "PolygonMesh quads: " << num_of_quads << "  vertices: " << num_of_vertices;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_vertices != num_of_quads * ElemSizes[MSH_QUA_4])
            m_plog->msg(__func__, "TriangleMesh quadss vertex count conflict!");
    }

    if (retc)
    {
        int num_of_polygons = poly_mesh->polygonCount();
        m_PolyMeshFile.m_iNumOfElems += num_of_polygons;

        if (num_of_polygons > 0)
            m_PolyMeshFile.m_iNumOfElemEntities++;

        vector<int> polygons = poly_mesh->polygonNodeIndices();
        size_t num_of_vertices = polygons.size();

        stringstream out_str;
        out_str << "PolygonMesh polygons: " << num_of_polygons << "  vertices: " << num_of_vertices;
        m_plog->msg(__func__, out_str.str().c_str());

        if (num_of_polygons > 0)
        {
            stringstream out_str;
            out_str << "PolygonMesh polygons vertice factor: " << (double)num_of_vertices / num_of_polygons << "  ElemSizes[MSH_TET_4]: " << ElemSizes[MSH_TET_4];
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    return retc;
}


bool MeshSaver::countMeshBody(Ptr<MeshBody> mesh_body)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !mesh_body)
    {
        m_plog->msg(__func__, "No MeshBody ptr given!");
        retc = false;
    }

    Ptr<PolygonMesh> poly_mesh = nullptr;
    Ptr<TriangleMesh> tri_mesh = nullptr;
    if (retc)
    {
        m_plog->msg(__func__, "Looking for the PolygonMesh...");
        poly_mesh = mesh_body->mesh();
        if (!poly_mesh)
            m_plog->msg(__func__, "No PolygonMesh!");

        m_plog->msg(__func__, "Looking for the TriangleMesh...");
        tri_mesh = mesh_body->displayMesh();
        if (!tri_mesh)
            m_plog->msg(__func__, "No TriangleMesh!");
    }

    if (retc && poly_mesh)
        retc = countPolygonMesh(poly_mesh);

    if (retc && tri_mesh)
        retc = countTriangleMesh(tri_mesh);

    return retc;
}


bool MeshSaver::saveActiveMesh(void)
{
    bool retc = true;

    if (retc) if (!m_app) retc = false;
    if (retc) if (!m_ui) retc = false;
    if (retc) if (!m_plog) retc = false;

    if (retc) m_plog->msg(__func__, "saveActiveMesh()");

    // if (retc) m_ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");
    
    // Ptr<Documents> documents = nullptr;
    // if (retc)
    // {
    //         documents = m_app->documents();
    //         if (!documents)
    //         {
    //             m_plog->msg(__func__, "No documents opened!");
    //             retc = false;
    //         }
    // }

    // Ptr<Document> doc = nullptr;
    // if (retc)
    // {
    //        doc = documents->add(DocumentTypes::FusionDesignDocumentType);
    //        if (!doc)
    //        {
    //            m_plog->msg(__func__, "Error retrieving a document!");
    //            retc = false;
    //        }
    // }

    Ptr<Product> product = nullptr;
    if (retc)
    {
        m_plog->msg(__func__, "Testing the product...");
        product = m_app->activeProduct();
        if (!product)
        {
            m_plog->msg(__func__, "No active design!");
            retc = false;
        }
    }

    Ptr<Design> design = nullptr;
    if (retc)
    {
        m_plog->msg(__func__, "Testing the design...");
        design = product;
        if (!design)
        {
            m_plog->msg(__func__, "Error retrieving the design!");

            m_plog->msg(__func__, "Testing the cam...");
            Ptr<CAM> cam = product;
            if (!cam)
            {
                m_plog->msg(__func__, "Error retrieving the cam!");
                retc = false;
            }
        }
    }

    if (design)
    {
        // Get the root component of the active design
        Ptr<Component> rootComp = nullptr;
        if (retc)
        {
            m_plog->msg(__func__, "Testing the root component...");
            rootComp = design->rootComponent();
            if (!rootComp)
            {
                m_plog->msg(__func__, "No components in the active design!");
                retc = false;
            }
        }

        Ptr<MeshBodies> meshes = nullptr;
        if (retc)
        {
            m_plog->msg(__func__, "Testing the mesh collection...");
            meshes = rootComp->meshBodies();
            if (!meshes)
            {
                m_plog->msg(__func__, "No meshes in the active design!");
                retc = false;
            }
        }

        if (retc)
        {
            m_plog->msg(__func__, "Calculating entities count...");
            m_iMeshCount = meshes->count();

#ifdef TRIPLE_TEST
            for (int jj = 0; (jj < 3) && retc; jj++)
#endif
            {
                for (int ii = 0; (ii < m_iMeshCount) && retc; ii++)
                {
                    Ptr<MeshBody> mesh_body = meshes->item(ii);
                    if (mesh_body)
                    {
                        stringstream out_str;
                        out_str << "The mesh " << ii << " (" << mesh_body->name() << ") counts: " << endl;
                        m_plog->msg(__func__, out_str.str().c_str());
                        retc = countMeshBody(mesh_body);
                    }
                    else
                    {
                        stringstream out_str;
                        out_str << "No mesh body " << ii << "!" << endl;
                        m_plog->msg(__func__, out_str.str().c_str());
                        retc = false;
                    }
                }

            }
        }

        if ((m_TriMeshFile.m_iNumOfPointEntities == 0) && (m_PolyMeshFile.m_iNumOfPointEntities == 0) &&
            (m_TriMeshFile.m_iNumOfPoints == 0) && (m_PolyMeshFile.m_iNumOfPoints == 0) &&
            (m_TriMeshFile.m_iNumOfElemEntities == 0) && (m_PolyMeshFile.m_iNumOfElemEntities == 0) &&
            (m_TriMeshFile.m_iNumOfElems == 0) && (m_PolyMeshFile.m_iNumOfElems == 0))
        {
            m_plog->msg(__func__, "The mesh is empty!"); // could happen, when Fusion 360 is already being closed
            retc = false;
        }

        if (retc)
        {
            retc = openMeshFiles();

            if (retc) retc = m_TriMeshFile.writeNodeHeader();

            if (retc) retc = m_PolyMeshFile.writeNodeHeader();

            m_iTriPointTag = 1;
            m_iPolyPointTag = 1;

            m_iTriElemTag = 1;
            m_iPolyElemTag = 1;

            if (retc)
            {
                m_plog->msg(__func__, "Writing points of meshes...");

                m_iTriEntityTag = 1;
                m_iPolyEntityTag = 1;

#ifdef TRIPLE_TEST
                m_dCoordFact = 2.;
                for (int jj = 0; (jj < 3) && retc; jj++)
                {
                    m_dCoordFact /= 2.;
#else
                {
#endif
                    for (int ii = 0; (ii < m_iMeshCount) && retc; ii++)
                    {
                        Ptr<MeshBody> mesh_body = meshes->item(ii);
                        if (mesh_body)
                        {
                            stringstream out_str;
                            out_str << "The mesh " << ii << ": " << mesh_body->name() << endl;
                            m_plog->msg(__func__, out_str.str().c_str());
                            retc = writeMeshBodyPoints(mesh_body);
                        }
                        else
                            retc = false;
                    }
                }
            }

            if (retc) retc = m_TriMeshFile.writeElemsHeader();

            if (retc) retc = m_PolyMeshFile.writeElemsHeader();

            if (retc)
            {
                m_plog->msg(__func__, "Writing elements of meshes...");

                m_iTriEntityTag = 1;
                m_iPolyEntityTag = 1;
                m_iTriFirstPointTag = 1;
                m_iPolyFirstPointTag = 1;

#ifdef TRIPLE_TEST
                for (int jj = 0; (jj < 3) && retc; jj++)
#endif
                {
                    for (int ii = 0; (ii < m_iMeshCount) && retc; ii++)
                    {
                        Ptr<MeshBody> mesh_body = meshes->item(ii);
                        if (mesh_body)
                        {
                            stringstream out_str;
                            out_str << "The mesh " << ii << ": " << mesh_body->name() << endl;
                            m_plog->msg(__func__, out_str.str().c_str());
                            retc = writeMeshBodyElems(mesh_body);
                        }
                        else
                            retc = false;
                    }
                }
            }

            retc &= closeMeshFiles();
        }
    }

    if (retc) m_plog->msg(__func__, "The mesh is saved.");
    else m_plog->msg(__func__, "Error!");

    return retc;
}
