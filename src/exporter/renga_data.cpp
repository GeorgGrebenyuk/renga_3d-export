#include "pch.h"
#include "renga_data.hpp"
#include "comutil.h"
#include "Renga\ObjectTypes.h"
#include "Renga\GridTypes.h"
#include "general_data.hpp"
#include "tools.hpp"


renga_data::renga_data(Renga::IApplicationPtr application, int mode)
{
	renga_application = application;
	Renga::IProjectPtr project = application->Project;
	Renga::IDataExporterPtr data_exporter = project->DataExporter;
	Renga::IMaterialManagerPtr material_manager = project->MaterialManager;

	bstr_t file_path = project->FilePath;
	this->project_path = file_path;
	this->start_sort_by_level_and_type();
	if (mode == 1)
	{
		Renga::IGridWithMaterialCollectionPtr collection = data_exporter->GetGrids();
		for (int objects_counter = 0; objects_counter < collection->Count; objects_counter++)
		{
			Renga::IGridWithMaterialPtr grid2material = collection->Get(objects_counter);
			Renga::IMaterialPtr material = grid2material->Material;
			Renga::Color color_info = material->GetColor();

			//create objects_3d_info definition
			std::vector<const char*> material_names{ material->Name};
			std::vector<std::vector<unsigned short>> material_colors{ {color_info.Red, color_info.Green, color_info.Blue, color_info.Alpha} };
			std::vector<Renga::IGridPtr> geometry{ grid2material->Grid };

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
			//create objects_3d_info definition
			std::vector<const char*> material_names;
			std::vector<std::vector<unsigned short>> material_colors;
			std::vector<Renga::IGridPtr> geometry;

			Renga::IExportedObject3DPtr object_3d_geometry = objects_3d_collection->Get(counter_objects);
			Renga::IModelObjectPtr model_object = model_objects_collection->GetById(object_3d_geometry->GetModelObjectId());
			GUID obj_type = model_object->GetObjectType();
			const char* object_name = model_object->GetName();
			const char* object_internal_id((const char*)model_object->GetUniqueIdS());

			Renga::Color color;
			const char* material_name = "-";
			color.Red = 0;
			color.Green = 0;
			color.Blue = 0;
			
			if (obj_type == Renga::ObjectTypes::Route) this->get_style(model_object, &color);
			else this->get_material(model_object, &color, &material_name);

			for (int counter_meshes = 0; counter_meshes < object_3d_geometry->GetMeshCount(); counter_meshes++)
			{
				Renga::IMeshPtr object_mesh = object_3d_geometry->GetMesh(counter_meshes);
				if (obj_type == Renga::ObjectTypes::Wall | obj_type == Renga::ObjectTypes::Roof | obj_type == Renga::ObjectTypes::Floor)
				{
					this->get_layered_material(counter_meshes, model_object, &color, &material_name);
				}

				for (int counter_grids = 0; counter_grids < object_mesh->GetGridCount(); counter_grids++)
				{
					Renga::IGridPtr object_grid = object_mesh->GetGrid(counter_grids);
					if (obj_type == Renga::ObjectTypes::Window | obj_type == Renga::ObjectTypes::Door)
					{
						this->get_grids_color(obj_type, object_grid->GetGridType(), &color);
					}

					material_names.push_back(material_name);
					material_colors.push_back({ color.Red, color.Green, color.Blue, color.Alpha });
					geometry.push_back(object_grid);
				}
			}
			object_3d_info object_data(material_names, material_colors, geometry);
			object_data.object_name = object_name;
			object_data.object_guid = object_internal_id;
			this->objects_3d_info.insert(std::pair<int, object_3d_info>{object_3d_geometry->GetModelObjectId(), object_data});
		}
	}
}
void renga_data::get_material(Renga::IModelObjectPtr model_object, Renga::Color* color, const char** material_name)
{
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
void renga_data::get_layered_material(int sub_object_position, Renga::IModelObjectPtr model_object, Renga::Color* color, const char** material_name)
{
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
		}
	}
}
void renga_data::get_style(Renga::IModelObjectPtr model_object, Renga::Color* color)
{
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
			//object_color = { 0.760,0.964,0.929 };
			//transparency = 0.6;
			break;
		case Renga::GridTypes::Door::Frame:
			(*color).Red = 102;
			(*color).Green = 51;
			(*color).Blue = 255;
			//object_color = { 0.4,0.2,1.0 };
			break;
		case Renga::GridTypes::Door::Solid:
			(*color).Red = 102;
			(*color).Green = 51;
			(*color).Blue = 255;
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
void renga_data::start_sort_by_level_and_type()
{
	Renga::IProjectPtr project = renga_application->Project;
	Renga::IModelObjectCollectionPtr model_objects_collection = project->Model->GetObjects();

	//std::map<Renga::ILevelPtr, std::map<GUID, std::vector<int>>> levels_objects;
	//std::map<GUID, std::vector<int>> non_levels_objects;

	std::list<Renga::IModelObjectPtr> row_levels;
	std::map<int, std::vector< Renga::IModelObjectPtr>>row_on_level_objects;
	std::vector<Renga::IModelObjectPtr> row_non_level_objects;

	for (int counter_objects = 0; counter_objects < model_objects_collection->Count; counter_objects++)
	{
		Renga::IModelObjectPtr model_object = model_objects_collection->GetByIndex(counter_objects);
		GUID object_type = model_object->GetObjectType();

		if (object_type == Renga::ObjectTypes::Level)
		{
			Renga::ILevelPtr level;
			model_object->QueryInterface(&level);
			row_levels.push_back(model_object);
		}
		else 
		{
			Renga::ILevelObjectPtr object_on_level;
			model_object->QueryInterface(&object_on_level);
			if (object_on_level)
			{
				int level_id = object_on_level->GetLevelId();
				bool find_existed = false;
				for (auto list_data : row_on_level_objects)
				{
					if (list_data.first == level_id) 
					{
						list_data.second.push_back(model_object);
						find_existed = true;
						break;
					}
				}
				if (!find_existed) row_on_level_objects.insert(
					std::pair<int, std::vector<Renga::IModelObjectPtr>>{level_id, { model_object }});
			}
			else 
			{
				row_non_level_objects.push_back(model_object);
			}
		}
	}
	//Сортируем уровни
	row_levels.sort(compare_levels);
	//Начинаем сортировку объектов по уровню
	std::map<int, std::vector< Renga::IModelObjectPtr>>row2_on_level_objects;
	for (Renga::IModelObjectPtr one_level : row_levels)
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