/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#include <iostream> 
#include <sstream>
#include <fstream>

#include "MeshSaver.h"


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


bool MeshSaver::writePoints(vector<Ptr<Point3D>> *points, char *fname)
{
    bool retc = true;

    if (retc) if (!m_plog) retc = false;

    if (retc && !points)
    {
        m_plog->msg(__func__, "No points vector ptr given!");
        retc = false;
    }

    if (retc && !fname)
    {
        m_plog->msg(__func__, "No .msh file name given!");
        retc = false;
    }

    ofstream msh_file;
    string sfname = string() + getenv("APPDATA") + "/Autodesk/Autodesk Fusion 360/API/AddIns/" + m_sAppName + "/" + fname + ".msh";
    if (retc)
    {
        msh_file.open(sfname, ios::out);
        if (msh_file.bad())
        {
            string msg = "Error to open the file " + sfname + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
    }

    if (retc)
    {
        msh_file << "$MeshFormat" << endl;
        msh_file << MSH_VERS << " " << MshFormats::ascii << " " << sizeof(size_t) << endl;
        msh_file << "$EndMeshFormat" << endl;

        msh_file << "$Nodes" << endl;
        int num_of_points = points->size();
        // entities header
        msh_file << 1 // num of entities
            << " " << num_of_points // num of nodes in all entities
            << " " << 1 << " " << num_of_points // first and last tag
            << endl;
        // entity header
        msh_file << 2 // num of dimensions
            << " " << 1 // entity tag
            << " " << false // no parametric coordinates
            << " " << num_of_points // num of nodes in the entity
            << endl;
        // node tags
        for (int ii = 0; ii < num_of_points; ii++)
            msh_file << ii + 1 << endl;

        for (vector<Ptr<Point3D>>::iterator it = points->begin(); (it != points->end()) && retc; it++)
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
                    msh_file << *id << " ";
                msh_file << endl;
            }
        }

        msh_file << "$EndNodes" << endl;
    }

    if (retc && msh_file.bad())
    {
        string msg = "Error writing to the file " + sfname + "!";
        m_plog->msg(__func__, msg.c_str());
        retc = false;
    }

    if (retc)
    {
        msh_file.close();
        if (msh_file.bad())
        {
            string msg = "Error closing the file " + sfname + "!";
            m_plog->msg(__func__, msg.c_str());
            retc = false;
        }
        else
        {
            string msg = "Mesh saved to the file " + sfname + ".";
            m_plog->msg(__func__, msg.c_str());
        }
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
        if (points.size() > 0)
        {
            m_plog->msg(__func__, "TriangleMesh:");
            retc = writePoints(&points, "SaveMesh_triangle");
        }
        else
            m_plog->msg(__func__, "TriangleMesh is empty!");
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
        if (points.size() > 0)
        {
            m_plog->msg(__func__, "PolygonMesh:");
            retc = writePoints(&points, "SaveMesh_polygon");
        }
        else
            m_plog->msg(__func__, "PolygonMesh is empty!");
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
    }

    if (retc) m_plog->msg(__func__, "The mesh is saved.");
    else m_plog->msg(__func__, "Error!");

    return retc;
}
