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

	// m_ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");
	
	// if (retc)
	// {
	// 	documents = m_app->documents();
	// 	if (!documents)
	// 	{
	// 		m_ui->messageBox("No documents opened!");
	// 		retc = false;
	// 	}
	// }

	// if (retc)
	// {
	//	doc = documents->add(DocumentTypes::FusionDesignDocumentType);
	//	if (!doc)
	//	{
	//		m_ui->messageBox("Error retrieving a document!");
	//		retc = false;
	//	}
	// }

	if (retc)
	{
		product = m_app->activeProduct();
		if (!product)
		{
			m_ui->messageBox("No active design!");
			retc = false;
		}
	}

	if (retc)
	{
		design = product;
		if (!design)
		{
			m_ui->messageBox("Error retrieving active design!");
			retc = false;
		}
	}

	// Get the root component of the active design
	if (retc)
	{
		rootComp = design->rootComponent();
		if (!rootComp)
		{
			m_ui->messageBox("No components in the active design!");
			retc = false;
		}
	}

	if (retc)
	{
		meshes = rootComp->meshBodies();
		if (!meshes)
		{
			m_ui->messageBox("No meshes in the active design!");
			retc = false;
		}
	}

	if (retc)
	{
		m_plog->msg("The mesh:");

		for (int ii = 0; (ii < meshes->count()) && retc; ii++)
		{
			Ptr<MeshBody> mesh = meshes->item(ii);
			if (mesh)
			{
				stringstream out_str;
				out_str << ii << ": " << mesh->name() << endl;
				m_plog->msg(out_str.str().c_str());
			}
			else
				retc = false;
		}
	}

	m_ui->messageBox("The mesh is saved");

	return true;
}
