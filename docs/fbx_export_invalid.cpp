#include "pch.h"
#include "fbx_export.hpp"
#include "Renga\GridTypes.h"
#include "Renga\ObjectTypes.h"
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (pManager) pManager->Destroy();
	if (pExitStatus) FBXSDK_printf("Program Success!\n");
}
void InitializeSdkObjects(FbxManager*& pManager)
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	pManager = FbxManager::Create();
	if (!pManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);
}
// Export document, the format is ascii by default
bool SaveDocument(FbxManager* pManager, FbxDocument* pDocument, const char* pFilename, int pFileFormat = -1, bool pEmbedMedia = false)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(pManager, "");

	if (pFileFormat < 0 || pFileFormat >= pManager->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// Write in fall back format if pEmbedMedia is true
		pFileFormat = pManager->GetIOPluginRegistry()->GetNativeWriterFormat();

		if (!pEmbedMedia)
		{
			//Try to export in ASCII if possible
			int lFormatIndex, lFormatCount = pManager->GetIOPluginRegistry()->GetWriterFormatCount();

			for (lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
			{
				if (pManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
				{
					FbxString lDesc = pManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
					const char* lASCII = "ascii";//ascii
					if (lDesc.Find(lASCII) >= 0)
					{
						pFileFormat = lFormatIndex;
						break;
					}
				}
			}
		}
	}

	// Set the export states. By default, the export states are always set to 
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
	// shows how to change these states.
	IOS_REF.SetBoolProp(EXP_FBX_MATERIAL, true);
	IOS_REF.SetBoolProp(EXP_FBX_TEXTURE, true);
	IOS_REF.SetBoolProp(EXP_FBX_EMBEDDED, pEmbedMedia);
	IOS_REF.SetBoolProp(EXP_FBX_ANIMATION, true);
	IOS_REF.SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Initialize the exporter by providing a filename.
	if (lExporter->Initialize(pFilename, pFileFormat, pManager->GetIOSettings()) == false)
	{
		FBXSDK_printf("Call to FbxExporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}

	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	FBXSDK_printf("FBX version number for this version of the FBX SDK is %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// Export the scene.
	lStatus = lExporter->Export(pDocument);

	// Destroy the exporter.
	lExporter->Destroy();
	return lStatus;
}
void fbx_export::get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color, double* link_transparency)
{
	std::vector<double> object_color = { 1.0,1.0,1.0 };
	double transparency = 0.0;

	Renga::IObjectWithMaterialPtr pObjectWithMaterial;
	obj->QueryInterface(&pObjectWithMaterial);
	if (pObjectWithMaterial)
	{
		if (Renga::IMaterialPtr obj_material = this->r_project->GetMaterialManager()->GetMaterial(pObjectWithMaterial->GetMaterialId())) {
			Renga::Color mat_color = obj_material->GetColor();

			double color_red(mat_color.Red / 255.0);
			double color_green(mat_color.Green / 255.0);
			double color_blue(mat_color.Blue / 255.0);
			object_color = { color_red ,color_green,color_blue };
			//return object_color;
		}

	}
	Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
	obj->QueryInterface(&pObjectWithLayeredMaterial);
	if (pObjectWithLayeredMaterial) {
		if (Renga::ILayeredMaterialPtr pLayeredMaterial = this->r_project->GetLayeredMaterialManager()
			->GetLayeredMaterialByIdGroupPair(pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair())) {
			Renga::IMaterialLayerCollectionPtr pLayers = pLayeredMaterial->GetLayers();
			if (pLayers->GetCount() > 0) {
				Renga::IMaterialLayerPtr pLayer = pLayers->Get(0);
				Renga::IMaterialPtr obj_material = pLayer->GetMaterial();
				if (obj_material != NULL)
				{
					Renga::Color mat_color = obj_material->GetColor();
					double color_red(mat_color.Red / 255.0);
					double color_green(mat_color.Green / 255.0);
					double color_blue(mat_color.Blue / 255.0);
					object_color = { color_red ,color_green,color_blue };
				}
				else
				{
					//std::cout << "error " << this->current_model_object->GetName() << std::endl;
				}
			}
		}
	}

	(*link_color) = object_color;
	(*link_transparency) = transparency;
}
void fbx_export::get_material_grid(GUID object_type, int grid_type, Renga::IGridPtr link_grid, 
	std::vector<double>* link_color, double* link_transparency)
{
	std::vector<double> object_color = { 1.0,1.0,1.0 };
	double transparency = 0.0;
	if ((object_type) == Renga::ObjectTypes::Window)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Window::Frame:
			object_color = { 0.729,0.596,0.274 };
			break;
		case Renga::GridTypes::Window::Glass:
			object_color = { 0.760,0.964,0.929 };
			transparency = 0.6;
			break;
		case Renga::GridTypes::Window::Sill:
			object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::OutwardSill:
			object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::Reveal:
			transparency = 0.1;
			break;
		}
	}
	else if ((object_type) == Renga::ObjectTypes::Door)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Door::Glass:
			object_color = { 0.760,0.964,0.929 };
			transparency = 0.6;
			break;
		case Renga::GridTypes::Door::Frame:
			object_color = { 0.4,0.2,1.0 };
			break;
		case Renga::GridTypes::Door::Solid:
			object_color = { 0.4,0.2,0.0 };
			break;
		case Renga::GridTypes::Door::DoorLining:
			object_color = { 0.4,0.0,0.0 };
			break;
		case Renga::GridTypes::Door::Threshold:
			object_color = { 0.4,0.0,0.0 };
			break;
		}
	}
	(*link_color) = object_color;
	(*link_transparency) = transparency;
}
fbx_export::fbx_export(Renga::IApplicationPtr app) {
	
	auto data_exporter = app->Project->GetDataExporter();
	if (data_exporter)
	{
		this->r_project = app->Project;
		FbxManager* lSdkManager = NULL;
		FbxDocument* lDocument = NULL;
		bool lResult = false;
		// Prepare the FBX SDK.
		InitializeSdkObjects(lSdkManager);

		// create the main document
		lDocument = FbxDocument::Create(lSdkManager, "RootDoc");

		FbxDocumentInfo* lDocInfo = FbxDocumentInfo::Create(lSdkManager, "DocInfo");
		lDocInfo->mTitle = "Test Renga export";
		lDocInfo->mSubject = "Renga exported objects by Renga::IGridWithMaterialCollectionPtr";
		lDocInfo->mAuthor = "GeorgGrebenyuk";
		lDocInfo->mRevision = "rev. 1.0";
		lDocInfo->mKeywords = "Fbx document";
		lDocInfo->mComment = "-";

		// add the documentInfo
		lDocument->SetDocumentInfo(lDocInfo);

		//Add somethind root object -- point in Zero
		FbxNode* zero_object = FbxNode::Create(lSdkManager, "renga_project");

		Renga::IExportedObject3DCollectionPtr objects3d_collection = data_exporter->GetObjects3D();
		Renga::IModelObjectCollectionPtr objects_collection = this->r_project->GetModel()->GetObjects();
		for (int counter_objects = 0; counter_objects < objects3d_collection->GetCount(); counter_objects++)
		{
			Renga::IExportedObject3DPtr current_model_object_geometry = objects3d_collection->Get(counter_objects);
			Renga::IModelObjectPtr current_model_object = objects_collection->GetById(current_model_object_geometry->GetModelObjectId());
			GUID obj_type = current_model_object->GetObjectType();
			//color
			std::vector<double> color_object;
			double transparency_object;
			this->get_material(current_model_object, &color_object, &transparency_object);

			FbxNode* fbx_obj = FbxNode::Create(lSdkManager, "obj_data");
			for (int counter_meshes = 0; counter_meshes < current_model_object_geometry->GetMeshCount(); counter_meshes++)
			{
				Renga::IMeshPtr pMesh = current_model_object_geometry->GetMesh(counter_meshes);
				FbxNode* fbx_obj_mesh = FbxNode::Create(lSdkManager, "mesh_data");
				for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
				{
					Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
					FbxNode* lNode = FbxNode::Create(lSdkManager, "grid_data");
					std::vector<double> color;
					double transparency;
					if (obj_type == Renga::ObjectTypes::Door | obj_type == Renga::ObjectTypes::Window)
					{
						int grid_type = pGrid->GetGridType();
						this->get_material_grid(current_model_object->GetObjectType(), grid_type, pGrid, &color, &transparency);
					}
					else
					{
						transparency = transparency_object;
						color = color_object;
					}

					FbxMesh* pMesh = FbxMesh::Create(lSdkManager, "grid_faceted");
					pMesh->InitControlPoints(pGrid->GetVertexCount());

					FbxVector4* lControlPoints = pMesh->GetControlPoints();
					for (int counter_points = 0; counter_points < pGrid->GetVertexCount(); counter_points++)
					{
						Renga::FloatPoint3D p3d = pGrid->GetVertex(counter_points);
						FbxVector4 p3d_fbx(p3d.X/1000.0, p3d.Y / 1000.0, p3d.Z / 1000.0);
						lControlPoints[counter_points] = p3d_fbx;
					}

					for (int counter_triangle = 0; counter_triangle < pGrid->TriangleCount; counter_triangle++)
					{
						FbxVector4* pCtrlPoint = pMesh->GetControlPoints();
						Renga::Triangle tr = pGrid->GetTriangle(counter_triangle);
						pMesh->BeginPolygon(-1, -1, -1, false);
						pMesh->AddPolygon(tr.V0);
						pMesh->AddPolygon(tr.V1);
						pMesh->AddPolygon(tr.V2);
						pMesh->EndPolygon();
					}
					lNode->SetNodeAttribute(pMesh);

					FbxString lMaterialName = "material_for_grid";
					FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(lSdkManager, lMaterialName.Buffer());
					FbxDouble3 color_data(color[0], color[1], color[2]);
					lMaterial->Diffuse.Set(color_data);
					lMaterial->DiffuseFactor.Set(1.);
					lNode->AddMaterial(lMaterial);
					fbx_obj_mesh->AddChild(lNode);

				}
				fbx_obj->AddChild(fbx_obj_mesh);
			}
			zero_object->AddChild(fbx_obj);
		}

		lDocument->AddRootMember(zero_object);
		SaveDocument(lSdkManager, lDocument, "E:\\Temp\\test_1.fbx");
		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(lSdkManager, lResult);
		app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Уведомление", "Модель была успешно создана!");
	}
}

