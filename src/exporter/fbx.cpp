#include "pch.h"
#include "fbx.hpp"
#include "fbx_common.hpp"
#include <codecvt>
#define FBXSDK_VERSION_STRING 2019
#define FBXSDK_VERSION_MINOR 0
fbx::fbx(renga_data* data)
{
	FbxManager* lSdkManager = NULL;
	FbxScene* lScene = NULL;
	char* lSampleFileName = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);

	//lResult = CreateScene(lScene, lSampleFileName);
	FbxNode* lRootNode = lScene->GetRootNode();
	for (auto obj_info : data->objects_3d_info)
	{
		object_3d_info info = obj_info.second;
		FbxNode* fbx_object = FbxNode::Create(lSdkManager, "object_data");
		fbx_object->SetVisibility(true);
		//const char* name = info.object_name;
		//fbx_object->SetName(name);

		for (int counter_grids = 0; counter_grids < info.geometry.size(); counter_grids++)
		{
			Renga::IGridPtr renga_Grid = info.geometry[counter_grids];

			FbxNode* fbx_grid_object = FbxNode::Create(lSdkManager, "grid_data");
			fbx_grid_object->SetVisibility(true);

			FbxMesh* fbx_grid_geometry = FbxMesh::Create(lSdkManager, "grid_geometry");
			fbx_grid_geometry->InitControlPoints(renga_Grid->GetVertexCount());

			FbxVector4* lControlPoints = fbx_grid_geometry->GetControlPoints();
			for (int counter_points = 0; counter_points < renga_Grid->GetVertexCount(); counter_points++)
			{
				Renga::FloatPoint3D p3d = renga_Grid->GetVertex(counter_points);
				FbxVector4 p3d_fbx(p3d.X / 1000.0, p3d.Y / 1000.0, p3d.Z / 1000.0);
				lControlPoints[counter_points] = p3d_fbx;
			}

			for (int counter_triangle = 0; counter_triangle < renga_Grid->TriangleCount; counter_triangle++)
			{
				FbxVector4* pCtrlPoint = fbx_grid_geometry->GetControlPoints();
				Renga::Triangle tr = renga_Grid->GetTriangle(counter_triangle);
				fbx_grid_geometry->BeginPolygon(-1, -1, -1, false);
				fbx_grid_geometry->AddPolygon(tr.V0);
				fbx_grid_geometry->AddPolygon(tr.V1);
				fbx_grid_geometry->AddPolygon(tr.V2);
				fbx_grid_geometry->EndPolygon();
			}
			fbx_grid_object->SetNodeAttribute(fbx_grid_geometry);

			bstr_t material_name = info.material_names[counter_grids];
			const char * material_name_str = material_name;
			FbxString lMaterialName = material_name_str;

			std::vector<unsigned short> material_color = info.material_colors[counter_grids];
			FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(lSdkManager, lMaterialName.Buffer());
			FbxDouble3 color_data(material_color[0]/255.0, material_color[1] / 255.0, material_color[2] / 255.0);
			lMaterial->Diffuse.Set(color_data);
			lMaterial->DiffuseFactor.Set(1.);
			fbx_grid_object->AddMaterial(lMaterial);
			fbx_object->AddChild(fbx_grid_object);

		}
		lRootNode->AddChild(fbx_object);
	}

	std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
	std::string current_project_path_str = myconv.to_bytes(data->project_path);
	lResult = SaveScene(lSdkManager, lScene, current_project_path_str.c_str());
	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while saving the scene...\n");
		DestroySdkObjects(lSdkManager, lResult);
	}

}