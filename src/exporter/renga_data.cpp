#include "pch.h"
#include "renga_data.hpp"
#include "comutil.h"
#include "Renga\ObjectTypes.h"
#include "Renga\GridTypes.h"
#include "general_data.hpp"
#include "tools.hpp"
#include <algorithm>

#include <chrono>
#include <fstream>
renga_data::renga_data(Renga::IApplicationPtr application, export_configs configs)
{
	renga_application = application;
	Renga::IProjectPtr project = application->Project;
	Renga::IDataExporterPtr data_exporter = project->DataExporter;
	Renga::IMaterialManagerPtr material_manager = project->MaterialManager;

	std::wstring current_project_path(project->FilePath, SysStringLen(project->FilePath));
	std::string current_project_path_str(current_project_path.begin(), current_project_path.end());

	std::string extension = "";
	switch (configs.export_formats)
	{
	case 0:
		extension = ".nwc";
		break;
	case 1:
		extension = ".fbx";
		break;
	}

	std::string renga_ext = ".rnp";
	current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), extension);
	std::wstring new_path(current_project_path_str.begin(), current_project_path_str.end());
	this->project_path = new_path;

	//Видимость
	Renga::IModelViewPtr view_3d;
	Renga::IViewPtr current_view = renga_application->ActiveView;
	current_view->QueryInterface(&view_3d);
	if (configs.use_hidded == true && view_3d) configs.use_hidded = true;
	else configs.use_hidded = false;

	std::vector<int> objects_ids;
	if (configs.geometry_mode == 0)
	{
		Renga::IGridWithMaterialCollectionPtr collection = data_exporter->GetGrids();
		for (int objects_counter = 0; objects_counter < collection->Count; objects_counter++)
		{
			Renga::IGridWithMaterialPtr grid2material = collection->Get(objects_counter);
			Renga::IMaterialPtr material = grid2material->Material;
			Renga::Color color_info = material->GetColor();
			Renga::IGridPtr grid = grid2material->Grid;
			this->info_triangles_count += grid->TriangleCount;

			//create objects_3d_info definition
			std::vector<bstr_t> material_names{ material->Name};
			std::vector<std::vector<unsigned short>> material_colors{ {color_info.Red, color_info.Green, color_info.Blue, color_info.Alpha} };
			std::vector<Renga::IGridPtr> geometry{ grid };

			object_3d_info object_data(material_names, material_colors, geometry);
			this->objects_3d_info.insert(std::pair<int, object_3d_info>{objects_counter, object_data});
		}
	}
	else 
	{
		Renga::IModelObjectCollectionPtr model_objects_collection = project->Model->GetObjects();
		Renga::IExportedObject3DCollectionPtr objects_3d_collection = data_exporter->GetObjects3D();

		for (int counter_objects = 0; counter_objects < objects_3d_collection->Count; counter_objects++) 
		{
			int triangles_all_count = 0;
			std::chrono::system_clock::time_point info_start_get_object = std::chrono::system_clock::now();
			//create objects_3d_info definition
			std::vector<bstr_t> material_names;
			std::vector<std::vector<unsigned short>> material_colors;
			std::vector<Renga::IGridPtr> geometry;

			Renga::IExportedObject3DPtr object_3d_geometry = objects_3d_collection->Get(counter_objects);
			Renga::IModelObjectPtr model_object = model_objects_collection->GetById(object_3d_geometry->GetModelObjectId());
			GUID obj_type = model_object->GetObjectType();
			bstr_t object_name = model_object->GetName();
			bstr_t object_internal_id = model_object->GetUniqueIdS();

			if ((configs.use_hidded && view_3d->IsObjectVisible(model_object->Id)) | !configs.use_hidded)
			{
				Renga::Color color_object;
				bstr_t material_name = "-";
				color_object.Red = 0;
				color_object.Green = 0;
				color_object.Blue = 0;
				color_object.Alpha = 255;

				if (obj_type == Renga::ObjectTypes::Route) this->get_style(model_object, &color_object);
				else if (obj_type == Renga::ObjectTypes::Room)
				{
					color_object.Red = 128;
					color_object.Green = 128;
					color_object.Blue = 128;
					color_object.Alpha = 120;
					bstr_t material_name = "Условный серый";
				}
				else this->get_material(model_object, &color_object, &material_name);

				for (int counter_meshes = 0; counter_meshes < object_3d_geometry->GetMeshCount(); counter_meshes++)
				{
					Renga::IMeshPtr object_mesh = object_3d_geometry->GetMesh(counter_meshes);
					Renga::Color color_mesh;
					if (obj_type == Renga::ObjectTypes::Wall | obj_type == Renga::ObjectTypes::Roof | obj_type == Renga::ObjectTypes::Floor)
					{
						this->get_layered_material(counter_meshes, model_object, &color_mesh, &material_name);
					}
					else color_mesh = color_object;

					for (int counter_grids = 0; counter_grids < object_mesh->GetGridCount(); counter_grids++)
					{
						Renga::Color color_grid;
						Renga::IGridPtr object_grid = object_mesh->GetGrid(counter_grids);
						triangles_all_count += object_grid->TriangleCount;

						if (obj_type == Renga::ObjectTypes::Window | obj_type == Renga::ObjectTypes::Door)
						{
							this->get_grids_color(obj_type, object_grid->GetGridType(), &color_grid);
						}
						else color_grid = color_mesh;

						material_names.push_back(material_name);
						material_colors.push_back({ color_grid.Red, color_grid.Green, color_grid.Blue, color_grid.Alpha });
						geometry.push_back(object_grid);
					}
				}
				object_3d_info object_data(material_names, material_colors, geometry);
				object_data.object_name = object_name;
				object_data.object_guid = object_internal_id;
				if ((configs.use_max_triangles && triangles_all_count <= configs.maximum_triangles_count) | !configs.use_max_triangles)
				{
					objects_ids.push_back(model_object->Id);
					this->info_triangles_count += triangles_all_count;
					this->objects_3d_info.insert(std::pair<int, object_3d_info>{object_3d_geometry->GetModelObjectId(), object_data});
				}
				
			}
		}
	}
	this->start_sort_by_level_and_type(&objects_ids);
}
void renga_data::get_material(Renga::IModelObjectPtr model_object, Renga::Color* color, bstr_t* material_name)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	Renga::IObjectWithMaterialPtr one_material;
	model_object->QueryInterface(&one_material);
	if (one_material)
	{
		if (one_material->HasMaterial())
		{
			Renga::IMaterialPtr material = renga_application->Project->MaterialManager->GetMaterial(one_material->MaterialId);
			if (material)
			{
				(*color) = material->GetColor();
				(*material_name) = material->GetName();
			}
		}

	}
}
void renga_data::get_layered_material(int sub_object_position, Renga::IModelObjectPtr model_object, Renga::Color* color, bstr_t* material_name)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	Renga::IObjectWithLayeredMaterialPtr some_materials;
	model_object->QueryInterface(&some_materials);
	if (some_materials)
	{
		if (some_materials->HasLayeredMaterial())
		{
			Renga::ILayerCollectionPtr materials = some_materials->GetLayers();
			Renga::ILayerPtr layer_material = materials->Get(sub_object_position);

			Renga::IMaterialPtr material = renga_application->Project->MaterialManager->GetMaterial(layer_material->MaterialId);
			if (material)
			{
				(*color) = material->GetColor();
				(*material_name) = material->GetName();
			}
			else 
			{
				material = renga_application->Project->MaterialManager->GetMaterial(materials->Get(0)->MaterialId);
				(*color) = material->GetColor();
				(*material_name) = material->GetName();
			}

		}
	}
}
void renga_data::get_style(Renga::IModelObjectPtr model_object, Renga::Color* color)
{
	(*color).Red = 0;
	(*color).Green = 0;
	(*color).Blue = 0;
	(*color).Alpha = 255;
	GUID type = model_object->GetObjectType();
	
	if (type == Renga::ObjectTypes::Route)
	{
		Renga::IRouteParamsPtr route_params;
		model_object->QueryInterface(&route_params);
		if (route_params)
		{
			Renga::ISystemStyleManagerPtr manager = renga_application->Project->SystemStyleManager;
			if (manager->Contains(route_params->GetSystemStyleId()))
			{
				Renga::ISystemStylePtr style = manager->GetSystemStyle(route_params->GetSystemStyleId());
				(*color) = style->GetColor();
			}

		}
	}
}
void renga_data::get_grids_color(GUID object_type, int grid_type, Renga::Color* color)
{
	(*color).Alpha = 255;
	if (object_type == Renga::ObjectTypes::Window)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Window::Frame:
			(*color).Red = 186;
			(*color).Green = 152;
			(*color).Blue = 70;
			//object_color = { 0.729,0.596,0.274 };
			break;
		case Renga::GridTypes::Window::Glass:
			(*color).Red = 194;
			(*color).Green = 246;
			(*color).Blue = 237;
			(*color).Alpha = 153;
			//object_color = { 0.760,0.964,0.929 };
			//transparency = 0.6;
			break;
		case Renga::GridTypes::Window::Sill:
			(*color).Red = 172;
			(*color).Green = 172;
			(*color).Blue = 172;
			//object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::OutwardSill:
			(*color).Red = 172;
			(*color).Green = 172;
			(*color).Blue = 172;
			//object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::Reveal:
			//transparency = 0.1;
			break;
		}
	}
	else if ((object_type) == Renga::ObjectTypes::Door)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Door::Glass:
			(*color).Red = 194;
			(*color).Green = 246;
			(*color).Blue = 237;
			(*color).Alpha = 153;
			//object_color = { 0.760,0.964,0.929 };
			//transparency = 0.6;
			break;
		case Renga::GridTypes::Door::Frame:
			(*color).Red = 153;
			(*color).Green = 153;
			(*color).Blue = 0;
			//object_color = { 0.4,0.2,1.0 };
			break;
		case Renga::GridTypes::Door::Solid:
			(*color).Red = 153;
			(*color).Green = 153;
			(*color).Blue = 0;
			//object_color = { 0.4,0.2,0.0 };
			break;
		case Renga::GridTypes::Door::DoorLining:
			(*color).Red = 102;
			(*color).Green = 0;
			(*color).Blue = 0;
			//object_color = { 0.4,0.0,0.0 };
			break;
		case Renga::GridTypes::Door::Threshold:
			(*color).Red = 102;
			(*color).Green = 0;
			(*color).Blue = 0;
			//object_color = { 0.4,0.0,0.0 };
			break;
		}
	}
}
void renga_data::start_sort_by_level_and_type(std::vector<int> *objects_ids)
{
	Renga::IProjectPtr project = renga_application->Project;
	Renga::IModelObjectCollectionPtr model_objects_collection = project->Model->GetObjects();
	Renga::IExportedObject3DCollectionPtr objects3d_collection = project->DataExporter->GetObjects3D();

	//std::map<Renga::ILevelPtr, std::map<GUID, std::vector<int>>> levels_objects;
	//std::map<GUID, std::vector<int>> non_levels_objects;

	//Видимость
	//Renga::IModelViewPtr view_3d;
	//Renga::IViewPtr current_view = renga_application->ActiveView;
	//current_view->QueryInterface(&view_3d);
	//if (use_hidded == true && view_3d) use_hidded = true;
	//else use_hidded = false;

	////Учитываем только объекты модели, у которых можно достать геометрию (кроме уровней*)
	//std::vector<int> only_geometry_objects;
	//for (int counter_object3d = 0; counter_object3d < objects3d_collection->Count; counter_object3d++)
	//{
	//	Renga::IExportedObject3DPtr obj = objects3d_collection->Get(counter_object3d);
	//	if (view_3d->IsObjectVisible(obj->GetModelObjectId()))only_geometry_objects.push_back(obj->GetModelObjectId());
	//	
	//}

	std::vector<Renga::IModelObjectPtr> row_levels;
	std::map<int, std::vector< Renga::IModelObjectPtr>>row_on_level_objects;
	std::map<int, std::vector< Renga::IModelObjectPtr>>::iterator check_level_objects;

	std::vector<Renga::IModelObjectPtr> row_non_level_objects;
	for (int obj_counter = 0; obj_counter < model_objects_collection->Count; obj_counter++)
	{
		Renga::IModelObjectPtr model_object = model_objects_collection->GetByIndex(obj_counter);
		GUID object_type = model_object->GetObjectType();
		if (object_type == Renga::ObjectTypes::Level)
		{
			Renga::ILevelPtr level;
			model_object->QueryInterface(&level);
			row_levels.push_back(model_object);
		}
	}
	for (int counter_objects : (*objects_ids))
	{
		Renga::IModelObjectPtr model_object = model_objects_collection->GetById(counter_objects);
		GUID object_type = model_object->GetObjectType();
		//bool is_object_have_geometry = false;

		//if (std::find(only_geometry_objects.begin(), only_geometry_objects.end(), model_object->Id) != only_geometry_objects.end()) {
		//	is_object_have_geometry = true;
		//}
		//else {
		//	is_object_have_geometry = false;
		//}

		
		Renga::ILevelObjectPtr object_on_level;
		model_object->QueryInterface(&object_on_level);
		if (object_on_level)
		{
			int level_id = object_on_level->GetLevelId();
			check_level_objects = row_on_level_objects.find(level_id);
			if (check_level_objects != row_on_level_objects.end())
			{
				check_level_objects->second.push_back(model_object);
			}
			else row_on_level_objects.insert(
				std::pair<int, std::vector<Renga::IModelObjectPtr>>{level_id, { model_object }});
		}
		else
		{
			row_non_level_objects.push_back(model_object);
		}
	}
	//Сортируем уровни
	std::vector<Renga::IModelObjectPtr> sorted_levels = compare_levels(row_levels);
	//Начинаем сортировку объектов по уровню

	for (Renga::IModelObjectPtr one_level : sorted_levels)
	{
		Renga::ILevelPtr level;
		one_level->QueryInterface(&level);

		std::map<const char*, std::vector<int>> type2objects;
		std::vector<Renga::IModelObjectPtr> level_objects = row_on_level_objects[one_level->GetId()];

		sort_objects(&level_objects, &type2objects);
		this->levels_objects.insert(std::pair<Renga::ILevelPtr, std::map<const char*, std::vector<int>>>{level, type2objects});
	}
	//СОртировка по типу среди внеуровневых
	sort_objects(&row_non_level_objects, &(this->non_levels_objects));
}