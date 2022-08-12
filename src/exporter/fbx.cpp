#include "pch.h"
#include "fbx.hpp"
#include "fbx_common.hpp"
#include <codecvt>
#include "tools.hpp"
#define FBXSDK_VERSION_STRING 2019
#define FBXSDK_VERSION_MINOR 0
fbx::fbx(renga_data* data)
{
	this->lSdkManager = NULL;
	FbxScene* lScene = NULL;
	char* lSampleFileName = NULL;
	bool lResult;

	// Prepare the FBX SDK.
	InitializeSdkObjects(lSdkManager, lScene);
	
	//FbxSystemUnit SceneSystemUnit = lScene->GetGlobalSettings().GetSystemUnit();
	//SceneSystemUnit.m.ConvertScene(lScene);
	//FbxSystemUnit::m.ConvertScene(lScene);

	//lResult = CreateScene(lScene, lSampleFileName);
	
	
	FbxNode* lRootNode = lScene->GetRootNode();
	for (auto obj_info : data->objects_3d_info)
	{
		object_3d_info info = obj_info.second;
		FbxNode* fbx_object = FbxNode::Create(lSdkManager, "object_data");
		fbx_object->SetVisibility(true);
		char* name = info.object_name;
		char new_name[200] = { 0 };
		Transliterate(name, &new_name[0]);
		//bstr_t new_name = new_name;
		fbx_object->SetName(new_name);

		/*
		Создаем mesh'и по одинаковым материалам объекта
		*/
		std::vector<std::vector<unsigned short>> material_colors_was;
		for (int counter_color = 0; counter_color< info.material_colors.size(); counter_color ++)
		{
			std::vector<unsigned short> material_color_that = info.material_colors[counter_color];
			bool that_color_exist = false;
			for (auto color : material_colors_was)
			{
				if (color == material_color_that)
				{
					that_color_exist = true;
					break;
				}
			}
			if (!that_color_exist)
			{
				//продолжаем
				FbxNode* fbx_sub_object = FbxNode::Create(lSdkManager, "sub_object_data");
				FbxMesh* fbx_object_geometry = FbxMesh::Create(lSdkManager, "object_geometry");
				this->materials_work(&info, counter_color, fbx_sub_object);
				int contr_points_count = 0;
				for (int counter_grids = 0; counter_grids < info.geometry.size(); counter_grids++)
				{
					Renga::IGridPtr renga_Grid = info.geometry[counter_grids];
					if (info.material_colors[counter_grids] == material_color_that) contr_points_count += renga_Grid->GetVertexCount();
					
				}
				fbx_object_geometry->InitControlPoints(contr_points_count);
				FbxVector4* lControlPoints = fbx_object_geometry->GetControlPoints();
				contr_points_count = 0;

				for (int counter_grids = 0; counter_grids < info.geometry.size(); counter_grids++)
				{
					Renga::IGridPtr renga_Grid = info.geometry[counter_grids];
					if (info.material_colors[counter_grids] == material_color_that)
					{
						//FbxMesh* fbx_grid_geometry = FbxMesh::Create(lSdkManager, "grid_geometry");
					//fbx_grid_geometry->InitControlPoints(renga_Grid->GetVertexCount());
						for (int counter_points = 0; counter_points < renga_Grid->GetVertexCount(); counter_points++)
						{
							Renga::FloatPoint3D p3d = renga_Grid->GetVertex(counter_points);
							FbxVector4 p3d_fbx(p3d.Y / 1000.0, p3d.Z / 1000.0, p3d.X / 1000.0);
							lControlPoints[counter_points + contr_points_count] = p3d_fbx;
						}
						for (int counter_triangle = 0; counter_triangle < renga_Grid->TriangleCount; counter_triangle++)
						{
							//FbxVector4* pCtrlPoint = fbx_grid_geometry->GetControlPoints();
							Renga::Triangle tr = renga_Grid->GetTriangle(counter_triangle);
							fbx_object_geometry->BeginPolygon(-1, -1, -1, false);
							fbx_object_geometry->AddPolygon(tr.V0 + contr_points_count);
							fbx_object_geometry->AddPolygon(tr.V1 + contr_points_count);
							fbx_object_geometry->AddPolygon(tr.V2 + contr_points_count);
							fbx_object_geometry->EndPolygon();
						}

						contr_points_count += renga_Grid->GetVertexCount();
						//fbx_object->AddChild(fbx_grid_object);
					}
				}

				fbx_sub_object->SetNodeAttribute(fbx_object_geometry);
				fbx_object->AddChild(fbx_sub_object);
				material_colors_was.push_back(material_color_that);
			}
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
void fbx::materials_work(object_3d_info* info, int counter, FbxNode* to_assign)
{
	char* material_name = (*info).material_names[counter];
	char new_material_name[200] = { 0 };
	Transliterate(material_name, &new_material_name[0]);
	FbxString lMaterialName = new_material_name;

	std::vector<unsigned short> material_color = (*info).material_colors[counter];
	check_objects = color2materials.find((*info).material_colors[counter]);
	if (check_objects == color2materials.end())
	{
		FbxSurfacePhong* lMaterial = FbxSurfacePhong::Create(lSdkManager, lMaterialName.Buffer());
		FbxDouble3 color_data(material_color[0] / 255.0, material_color[1] / 255.0, material_color[2] / 255.0);
		lMaterial->Diffuse.Set(color_data);
		lMaterial->DiffuseFactor.Set(1.);

		color2materials.insert(std::pair<std::vector<unsigned short>, FbxSurfacePhong*>{material_color, lMaterial});
		to_assign->AddMaterial(lMaterial);
	}
	else
	{
		to_assign->AddMaterial(check_objects->second);
	}
}