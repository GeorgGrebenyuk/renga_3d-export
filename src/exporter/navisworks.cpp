#include "pch.h"
#include "navisworks.hpp"


#define LI_NWC_NO_PROGRESS_CALLBACKS NULL
#define LI_NWC_NO_USER_DATA NULL
navisworks::navisworks(renga_data* data)
{
	this->project_data = data;
	LiNwcApiErrorInitialise();
	int status = 0;
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		this->start();
		break;
	case LI_NWC_API_NOT_LICENSED:
		printf("\n\nNot Licensed\n");
		status = 1;
		break;
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("\n\nInternal Error\n");
		status = 2;
		break;
	}
}
void navisworks::parse_level_objects(std::map<const char*, std::vector<int>> data, LcNwcGroup* to_record)
{
	//bool is_any_geometry_all = false;
	for (std::pair<const char*, std::vector<int>> type2objects : data)
	{
		const char* group_name = type2objects.first;
		std::vector<int> objects = type2objects.second;
		bstr_t group_name_str = group_name;
		LcNwcGroup object_type_group;
		//bool is_any_geometry = false;
		object_type_group.SetName(group_name_str);
		object_type_group.SetLayer(TRUE);

		for (int model_object_id : objects)
		{
			//std::map<int, object_3d_info>::iterator check_exist_geometry;
			//check_exist_geometry = this->project_data->objects_3d_info.find(model_object_id);
			//if (check_exist_geometry != this->project_data->objects_3d_info.end())
			//{

			//}

			//is_any_geometry = true;
			//is_any_geometry_all = true;
			object_3d_info info = this->project_data->objects_3d_info[model_object_id];
			LcNwcGroup object_nwc;
			bstr_t object_name_str = info.object_name;
			object_nwc.SetName(object_name_str);
			object_nwc.SetLayer(TRUE);
			//Geometry
			for (int counter_sub_geometry = 0; counter_sub_geometry < info.geometry.size(); counter_sub_geometry++)
			{
				Renga::IGridPtr sub_grid = info.geometry[counter_sub_geometry];
				std::vector<unsigned short> sub_color = info.material_colors[counter_sub_geometry];
				bstr_t sub_material_name = info.material_names[counter_sub_geometry];

				LcNwcGeometry grid_data;
				grid_data.SetName(L"GRID");

				LcNwcMaterial body_material;
				body_material.SetDiffuseColor(sub_color[0] / 255.0, sub_color[1] / 255.0, sub_color[2] / 255.0);
				body_material.SetAmbientColor(sub_color[0] / 255.0, sub_color[1] / 255.0, sub_color[2] / 255.0);
				body_material.SetName(sub_material_name);
				body_material.SetTransparency(1 - sub_color[3]/255.0);
				grid_data.AddAttribute(body_material);

				std::vector<Renga::FloatPoint3D> points;
				for (int counter_points = 0; counter_points < sub_grid->GetVertexCount(); counter_points++)
				{
					points.push_back(sub_grid->GetVertex(counter_points));
				}
				LcNwcGeometryStream stream_grid_record = grid_data.OpenStream();
				for (int counter_triangles = 0; counter_triangles < sub_grid->GetTriangleCount(); counter_triangles++)
				{
					Renga::Triangle tr = sub_grid->GetTriangle(counter_triangles);
					stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
					Renga::FloatPoint3D p1 = points[tr.V0];
					Renga::FloatPoint3D p2 = points[tr.V1];
					Renga::FloatPoint3D p3 = points[tr.V2];
					stream_grid_record.TriFanVertex(p1.X / 1000.0, p1.Y / 1000.0, p1.Z / 1000.0);
					stream_grid_record.TriFanVertex(p2.X / 1000.0, p2.Y / 1000.0, p2.Z / 1000.0);
					stream_grid_record.TriFanVertex(p3.X / 1000.0, p3.Y / 1000.0, p3.Z / 1000.0);
					stream_grid_record.End();
				}
				grid_data.CloseStream(stream_grid_record);
				object_nwc.AddNode(grid_data);
			}
			object_type_group.AddNode(object_nwc);

			
		}
		(*to_record).AddNode(object_type_group);
	}
	//return is_any_geometry_all;

}
void navisworks::start() {

	LcNwcScene scene;
	//bstr_t file_path = this->project_data->project_path;
	LtWideString wfilename = this->project_data->project_path.c_str();

	//Getting geometry, properties, materials ...
	for (auto level_info : this->project_data->levels_objects)
	{
		Renga::ILevelPtr level_object = level_info.first;

		LcNwcGroup levels_objects;
		levels_objects.SetName(level_object->LevelName);
		levels_objects.SetLayer(TRUE);
		parse_level_objects(level_info.second, &levels_objects);
		scene.AddNode(levels_objects);
	}
	LcNwcGroup non_levels_objects;
	non_levels_objects.SetName(L"OtherObjects");
	non_levels_objects.SetLayer(TRUE);
	parse_level_objects((this->project_data->non_levels_objects), &non_levels_objects);
	scene.AddNode(non_levels_objects);
	
	int ii = 0;
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);

}
