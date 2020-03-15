/* --------------------------------------
 * SaveMesh plug-in mesh saving feature
 */

#include <iostream> 
#include <sstream>

#include "MeshSaver.h"


bool MeshSaver::init(Ptr<Application> app, Ptr<UserInterface> ui, SmLogger *plog)
{
	bool retc = true;

	if (retc)
	{
		m_app = app;
		if (!m_app)
			retc = false;
	}

	if (retc)
	{
		m_ui = ui;
		if (!m_ui)
			retc = false;
	}

	if (retc)
	{
		m_plog = plog;
		if (!m_plog)
			retc = false;
	}

	return retc;
}


bool MeshSaver::writePoints(vector<Ptr<Point3D>> *points)
{
	bool retc = true;

	if (retc) if (!m_plog) retc = false;

	if (retc && !points)
	{
		m_plog->msg(__func__, "No points vector ptr given!");
		retc = false;
	}

	if (retc)
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
				m_plog->msg(__func__, "Point:");
				for (vector<double>::iterator id = coords.begin(); (id != coords.end()) && retc; id++)
					m_plog->msg(__func__, to_string(*id).c_str());
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
		if (points.size() == 0)
		{
			m_plog->msg(__func__, "TriangleMesh is empty!");
			retc = false;
		}
	}

	if (retc) retc = writePoints(&points);

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
		{
			m_plog->msg(__func__, "PolygonMesh is empty!");
			retc = false;
		}
	}

	if (retc) retc = writePoints(&points);

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
		{
			m_plog->msg(__func__, "No PolygonMesh!");

			tri_mesh = mesh_body->displayMesh();
			if (!tri_mesh)
			{
				m_plog->msg(__func__, "No TriangleMesh!");
				retc = false;
			}
		}
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
	// Ptr<Documents> documents = nullptr;
	// Ptr<Document> doc = nullptr;
	Ptr<Product> product = nullptr;
	Ptr<Design> design = nullptr;
	Ptr<Component> rootComp = nullptr;
	Ptr<MeshBodies> meshes = nullptr;

	if (retc) if (!m_app) retc = false;
	if (retc) if (!m_ui) retc = false;
	if (retc) if (!m_plog) retc = false;

	if (retc) m_plog->msg(__func__, "saveActiveMesh()");

	// if (retc) m_ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");
	
	// if (retc)
	// {
	// 		documents = m_app->documents();
	// 		if (!documents)
	// 		{
	// 			m_plog->msg(__func__, "No documents opened!");
	// 			retc = false;
	// 		}
	// }

	// if (retc)
	// {
	//		doc = documents->add(DocumentTypes::FusionDesignDocumentType);
	//		if (!doc)
	//		{
	//			m_plog->msg(__func__, "Error retrieving a document!");
	//			retc = false;
	//		}
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

	// Get the root component of the active design
	if (retc)
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
	}

    if (retc)
	{
		m_plog->msg(__func__, "Writing meshes...");

		for (int ii = 0; (ii < meshes->count()) && retc; ii++)
		{
			Ptr<MeshBody> mesh = meshes->item(ii);
			if (mesh)
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
			else
				retc = false;
		}
	}

	if (retc) m_plog->msg(__func__, "The mesh is saved.");
	else m_plog->msg(__func__, "Error!");

	return true;
}
