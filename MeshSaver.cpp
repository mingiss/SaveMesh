/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#include "MeshSaver.h"


int static ElemSizes[MSH_MAX_NUM + 1] =
{
0,
2,  // MSH_LIN_2              1  
3,  // MSH_TRI_3              2  
4,  // MSH_QUA_4              3  
4,  // MSH_TET_4              4  
8,  // MSH_HEX_8              5  
6,  // MSH_PRI_6              6  
5,  // MSH_PYR_5              7  
3,  // MSH_LIN_3              8  
6,  // MSH_TRI_6              9  
9,  // MSH_QUA_9              10 
10, // MSH_TET_10             11 
27, // MSH_HEX_27             12 
18, // MSH_PRI_18             13 
14, // MSH_PYR_14             14 
1,  // MSH_PNT                15 
8,  // MSH_QUA_8              16 
20, // MSH_HEX_20             17 
15, // MSH_PRI_15             18 
13, // MSH_PYR_13             19 
9,  // MSH_TRI_9              20 
10, // MSH_TRI_10             21 
12, // MSH_TRI_12             22 
15, // MSH_TRI_15             23 
15, // MSH_TRI_15I            24 
21, // MSH_TRI_21             25 
4,  // MSH_LIN_4              26 
5,  // MSH_LIN_5              27 
6,  // MSH_LIN_6              28 
20, // MSH_TET_20             29 
35, // MSH_TET_35             30 
56, // MSH_TET_56             31 
22, // MSH_TET_22             32 
28, // MSH_TET_28             33 
0,  // MSH_POLYG_             34 
0,  // MSH_POLYH_             35 
16, // MSH_QUA_16             36 
25, // MSH_QUA_25             37 
36, // MSH_QUA_36             38 
12, // MSH_QUA_12             39 
16, // MSH_QUA_16I            40 
20, // MSH_QUA_20             41 
28, // MSH_TRI_28             42 
36, // MSH_TRI_36             43 
45, // MSH_TRI_45             44 
55, // MSH_TRI_55             45 
66, // MSH_TRI_66             46 
49, // MSH_QUA_49             47 
64, // MSH_QUA_64             48 
81, // MSH_QUA_81             49 
100, // MSH_QUA_100           50 
121, // MSH_QUA_121           51 
18, // MSH_TRI_18             52 
21, // MSH_TRI_21I            53 
24, // MSH_TRI_24             54 
27, // MSH_TRI_27             55 
30, // MSH_TRI_30             56 
24, // MSH_QUA_24             57 
28, // MSH_QUA_28             58 
32, // MSH_QUA_32             59 
36, // MSH_QUA_36I            60 
40, // MSH_QUA_40             61 
7,  // MSH_LIN_7              62 
8,  // MSH_LIN_8              63 
9,  // MSH_LIN_9              64 
10, // MSH_LIN_10             65 
11, // MSH_LIN_11             66 
2,  // MSH_LIN_B              67 
2,  // MSH_TRI_B              68 
0,  // MSH_POLYG_B            69 
2,  // MSH_LIN_C              70 
// TETS COMPLETE (6->10)
84, // MSH_TET_84             71 
120, // MSH_TET_120           72 
165, // MSH_TET_165           73 
220, // MSH_TET_220           74 
286, // MSH_TET_286           75
0,
0,
0, 
// TETS INCOMPLETE (6->10)
34, // MSH_TET_34             79 
40, // MSH_TET_40             80 
46, // MSH_TET_46             81 
52, // MSH_TET_52             82 
58, // MSH_TET_58             83 
//                               
1,  // MSH_LIN_1              84 
1,  // MSH_TRI_1              85 
1,  // MSH_QUA_1              86 
1,  // MSH_TET_1              87 
1,  // MSH_HEX_1              88 
1,  // MSH_PRI_1              89 
40, // MSH_PRI_40             90 
75, // MSH_PRI_75             91 
// HEXES COMPLETE (3->9)
64, // MSH_HEX_64             92 
125, // MSH_HEX_125           93 
216, // MSH_HEX_216           94 
343, // MSH_HEX_343           95 
512, // MSH_HEX_512           96 
729, // MSH_HEX_729           97 
1000, // MSH_HEX_1000         98 
// HEXES INCOMPLETE (3->9)
32, // MSH_HEX_32             99 
44, // MSH_HEX_44             100
56, // MSH_HEX_56             101
68, // MSH_HEX_68             102
80, // MSH_HEX_80             103
92, // MSH_HEX_92             104
104, // MSH_HEX_104           105
// PRISMS COMPLETE (5->9)
126, // MSH_PRI_126           106
196, // MSH_PRI_196           107
288, // MSH_PRI_288           108
405, // MSH_PRI_405           109
550, // MSH_PRI_550           110
// PRISMS INCOMPLETE (3->9)
24, // MSH_PRI_24             111
33, // MSH_PRI_33             112
42, // MSH_PRI_42             113
51, // MSH_PRI_51             114
60, // MSH_PRI_60             115
69, // MSH_PRI_69             116
78, // MSH_PRI_78             117
// PYRAMIDS COMPLETE (3->9)
30, // MSH_PYR_30             118
55, // MSH_PYR_55             119
91, // MSH_PYR_91             120
140, // MSH_PYR_140           121
204, // MSH_PYR_204           122
285, // MSH_PYR_285           123
385, // MSH_PYR_385           124
// PYRAMIDS INCOMPLETE (3->9)
21, // MSH_PYR_21             125
29, // MSH_PYR_29             126
37, // MSH_PYR_37             127
45, // MSH_PYR_45             128
53, // MSH_PYR_53             129
61, // MSH_PYR_61             130
69, // MSH_PYR_69             131
// Additional types              
1,  // MSH_PYR_1              132
1,  // MSH_PNT_SUB            133
2,  // MSH_LIN_SUB            134
3,  // MSH_TRI_SUB            135
4,  // MSH_TET_SUB            136
16, // MSH_TET_16             137
3,  // MSH_TRI_MINI           138
4,  // MSH_TET_MINI           139
4,  // MSH_TRIH_4             140
};


bool MeshSaver::init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog, char* app_name)
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

    return retc;
}


bool MeshSaver::openMeshFile(MeshFile& msh_file)
{
    bool retc = true;

    if (retc)
    {
        msh_file.m_fMeshFile.open(msh_file.m_sMeshFName, ios::out);
        if (msh_file.m_fMeshFile.bad())
        {
            string msg = "Error to open the file " + msh_file.m_sMeshFName + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
    }

    return retc;
}


bool MeshSaver::openMeshFiles(void)
{
    bool retc = true;

    string spath = string() + getenv("APPDATA") + "/Autodesk/Autodesk Fusion 360/API/AddIns/" + m_sAppName + "/" + m_sAppName + "_";
    m_TriMeshFile.m_sMeshFName = spath + "triangle" + ".msh";
    m_PolyMeshFile.m_sMeshFName = spath + "polygon" + ".msh";

    if (retc) retc = openMeshFile(m_TriMeshFile);
    if (retc) retc = openMeshFile(m_PolyMeshFile);

    return retc;
}


bool MeshSaver::closeMeshFile(MeshFile& msh_file)
{
    bool retc = true;

    if (retc)
    {
        msh_file.m_fMeshFile.close();
        if (msh_file.m_fMeshFile.bad())
        {
            string msg = "Error closing the file " + msh_file.m_sMeshFName + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
        else
        {
            string msg = "Mesh saved to the file " + msh_file.m_sMeshFName + ".";
            m_plog->msg(__func__, msg.c_str());
        }
    }

    return retc;
}


bool MeshSaver::closeMeshFiles(void)
{
    bool retc = true;

    if (retc) retc = closeMeshFile(m_TriMeshFile);
    if (retc) retc = closeMeshFile(m_PolyMeshFile);

    m_TriMeshFile.m_sMeshFName.clear();
    m_PolyMeshFile.m_sMeshFName.clear();

    return retc;
}


bool MeshSaver::writePoints(vector<Ptr<Point3D>>& points, vector<int>& triangles, vector<int>& quads, vector<int>& polygons, MeshFile& msh_file)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc)
    {
        msh_file.m_fMeshFile << "$MeshFormat" << endl;
        msh_file.m_fMeshFile << MSH_VERS << " " << MshFormats::ascii << " " << sizeof(size_t) << endl;
        msh_file.m_fMeshFile << "$EndMeshFormat" << endl;

        msh_file.m_fMeshFile << "$Nodes" << endl;
        size_t num_of_points = points.size();
        // entities header
        msh_file.m_fMeshFile << 1 // num of entities
            << " " << num_of_points // num of nodes in all entities
            << " " << 1 << " " << num_of_points // first and last tag
            << endl;
        // entity header
        msh_file.m_fMeshFile << 2 // num of dimensions
            << " " << 1 // entity tag
            << " " << false // no parametric coordinates
            << " " << num_of_points // num of nodes in the entity
            << endl;
        // node tags
        for (size_t ii = 0; ii < num_of_points; ii++)
            msh_file.m_fMeshFile << ii + 1 << endl;

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
                    msh_file.m_fMeshFile << *id << " ";
                msh_file.m_fMeshFile << endl;
            }
        }

        msh_file.m_fMeshFile << "$EndNodes" << endl;

        msh_file.m_fMeshFile << "$Elements" << endl;
        size_t num_of_triangles = triangles.size() / ElemSizes[MSH_TRI_3];
        size_t num_of_quads = 0; // quads.size() / ElemSizes[MSH_QUA_4];
        size_t num_of_polygons = 0; // polygons.size() / ElemSizes[MSH_TET_4]; ???
        size_t num_of_elems = num_of_triangles + num_of_quads + num_of_polygons;
        int id = 0; // tag of the current element

        // entities header
        msh_file.m_fMeshFile << 1 // num of entities
            << " " << num_of_elems // num of elems in all entities
            << " " << 1 << " " << num_of_elems // first and last tag
            << endl;

        // triangles entity
        int elem_type = MSH_TRI_3;
        int elem_size = ElemSizes[elem_type];
        msh_file.m_fMeshFile << 2 // num of dimensions
            << " " << 1 // entity tag
            << " " << elem_type // triangles
            << " " << num_of_elems << endl;

        int ix = 0;
        for (vector<int>::iterator it = triangles.begin(); (it != triangles.end()) && retc; it++)
        {
            if (ix % elem_size == 0)
                msh_file.m_fMeshFile << ++id; // elem tag
            msh_file.m_fMeshFile << " " << *it + 1;
            if (ix % elem_size == elem_size - 1)
                msh_file.m_fMeshFile << endl;
            ix++;
        }

        msh_file.m_fMeshFile << "$EndElements" << endl;
    }

    if (retc && msh_file.m_fMeshFile.bad())
    {
        string msg = "Error writing to the file " + msh_file.m_sMeshFName + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    return retc;
}
    
bool MeshSaver::writeTriangleMesh(Ptr<TriangleMesh> tri_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !tri_mesh)
    {
        m_plog->msg(__func__, "No TriangleMesh ptr given!");
        retc = false;
    }

    vector<Ptr<Point3D>> points;
    if (retc)
    {
        points = tri_mesh->nodeCoordinates();
        if (points.size() == 0)
            m_plog->msg(__func__, "TriangleMesh has no points!");
    }

    vector<int> triangles;
    if (retc)
        triangles = tri_mesh->nodeIndices();

    vector<int> quads;
    vector<int> polygons;

    if (retc)
    {
        m_plog->msg(__func__, "TriangleMesh:");
        retc = writePoints(points, triangles, quads, polygons, m_TriMeshFile);
    }

    return retc;
}


bool MeshSaver::writePolygonMesh(Ptr<PolygonMesh> poly_mesh)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !poly_mesh)
    {
        m_plog->msg(__func__, "No PolygonMesh ptr given!");
        retc = false;
    }

    vector<Ptr<Point3D>> points;
    if (retc)
    {
        points = poly_mesh->nodeCoordinates();
        if (points.size() == 0)
            m_plog->msg(__func__, "PolygonMesh has no points!");
    }

    vector<int> triangles;
    if (retc)
    {
        triangles = poly_mesh->triangleNodeIndices();
        int num_of_triangles = poly_mesh->triangleCount();
        // if (num_of_triangles)
        {
            stringstream out_str;
            out_str << "triangles: " << num_of_triangles << " " << triangles.size();
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    vector<int> quads;
    if (retc)
    {
        quads = poly_mesh->quadNodeIndices();
        int num_of_quads = poly_mesh->quadCount();
        // if (num_of_quads)
        {
            stringstream out_str;
            out_str << "quads: " << num_of_quads << " " << quads.size();
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    vector<int> polygons;
    if (retc)
    {
        polygons = poly_mesh->polygonNodeIndices();
        int num_of_polygons = poly_mesh->polygonCount();
        // if (num_of_polygons)
        {
            stringstream out_str;
            out_str << "polygons: " << num_of_polygons << " " << polygons.size();
            m_plog->msg(__func__, out_str.str().c_str());
        }
    }

    if (retc)
    {
        m_plog->msg(__func__, "PolygonMesh:");
        retc = writePoints(points, triangles, quads, polygons, m_PolyMeshFile);
    }

    return retc;
}


bool MeshSaver::writeMeshBody(Ptr<MeshBody> mesh_body)
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
        retc = writePolygonMesh(poly_mesh);

    if (retc && tri_mesh)
        retc = writeTriangleMesh(tri_mesh);

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
            retc = openMeshFiles();

            if (retc)
            {
                m_plog->msg(__func__, "Writing meshes...");

                for (int ii = 0; (ii < meshes->count()) && retc; ii++)
                {
                    Ptr<MeshBody> mesh_body = meshes->item(ii);
                    if (mesh_body)
                    {
                        stringstream out_str;
                        out_str << "The mesh " << ii << ": " << mesh_body->name() << endl;
                        m_plog->msg(__func__, out_str.str().c_str());
                        retc = writeMeshBody(mesh_body);
                    }
                    else
                        retc = false;
                }
            }

            retc &= closeMeshFiles();
        }
    }

    if (retc) m_plog->msg(__func__, "The mesh is saved.");
    else m_plog->msg(__func__, "Error!");

    return retc;
}
