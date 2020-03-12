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

	if (retc) if (!m_app) retc = false;
	if (retc) if (!m_ui) retc = false;
	if (retc) if (!m_plog) retc = false;

	m_plog->msg("saveActiveMesh()");

	// m_ui->messageBox("Ready to save the mesh. Press 'OK' when the mesh is prepared.");
	
	// Ptr<Documents> documents = nullptr;
	// if (retc)
	// {
	// 	documents = m_app->documents();
	// 	if (!documents)
	// 	{
	// 		m_ui->messageBox("No documents opened!");
	// 		retc = false;
	// 	}
	// }

	// Ptr<Document> doc = nullptr;
	// if (retc)
	// {
	//	doc = documents->add(DocumentTypes::FusionDesignDocumentType);
	//	if (!doc)
	//	{
	//		m_ui->messageBox("Error retrieving a document!");
	//		retc = false;
	//	}
	// }

	m_plog->msg("Testing the product...");
	Ptr<Product> product = nullptr;
	if (retc)
	{
		product = m_app->activeProduct();
		if (!product)
		{
			m_plog->msg("No active design!");
			retc = false;
		}
	}

	m_plog->msg("Testing the design...");
	Ptr<Design> design = nullptr;
	if (retc)
	{
		design = product;
		if (!design)
		{
			m_plog->msg("Error retrieving the design!");

			m_plog->msg("Testing the cam...");
			Ptr<CAM> cam = product;
			if (!cam)
			{
				m_plog->msg("Error retrieving the cam!");
				retc = false;
			}
		}
	}

	if (design)
	{
		// Get the root component of the active design
		Ptr<Component> rootComp = nullptr;
		m_plog->msg("Testing the root component...");
		if (retc)
		{
			rootComp = design->rootComponent();
			if (!rootComp)
			{
				m_plog->msg("No components in the active design!");
				retc = false;
			}
		}

		m_plog->msg("Testing the mesh collection...");
		Ptr<MeshBodies> meshes = nullptr;
		if (retc)
		{
			meshes = rootComp->meshBodies();
			if (!meshes)
			{
				m_plog->msg("No meshes in the active design!");
				retc = false;
			}
		}

		m_plog->msg("Writing meshes...");
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
	}

	if (retc) m_plog->msg("The mesh is saved");
	else m_plog->msg("Error");

	return retc;
}
