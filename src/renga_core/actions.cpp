#include "general_functions.h"
#include "console_start.h"
#include <regex>

#include "navis.h"

renga_data::renga_data(Renga::IProjectPtr project, int type_input)
{
	this->type = type_input;
	std::wstring export_path;
	tools::get_new_save_path(project, type_input, true, &export_path);
	this->file_export_path = export_path;
	this->r_project = project;
	this->get_transform_parameters();
	std::cout << "Start read internal data (geometry and properties" << std::endl;
	this->get_data();
	std::cout << "Start sort layers by level" << std::endl;
	this->sort_layers();
	std::cout << "End reading project!" << std::endl;
	//this->projects_offset = tools::get_transform_parameters(this->r_project);

	//tools::renga_project = this->r_project;
	//Get file path and set new file name with target extension
	//this->r_app->Quit();
}
void renga_data::get_transform_parameters()
{
	Renga::IBuildingInfoPtr building_object = this->r_project->GetBuildingInfo();
	
	Renga::IPropertyContainerPtr props_usual = building_object->GetProperties();
	
	bstr_t offset_x = "IfcLocationX";
	bool is_x_correct = false;
	double offset_x_value = 0.0;
	bstr_t offset_y = "IfcLocationY";
	bool is_y_correct = false;
	double offset_y_value = 0.0;
	bstr_t offset_z = "IfcLocationZ";
	bool is_z_correct = false;
	double offset_z_value = 0.0;
	bstr_t angle = "IfcDirectionPrecession";
	bool angle_correct = false;
	double angle_value = 0.0;

	//Renga::IGuidCollectionPtr props_ids = props_usual->GetIds();

	/*for (int counter_property = 0; counter_property < props_ids->GetCount(); counter_property++)
	{
		GUID id = props_ids->Get(counter_property);
		Renga::IPropertyPtr one_prop = props_usual->Get(id);
		if (one_prop->Name == offset_x && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_x_value = one_prop->GetDoubleValue();
			is_x_correct = true;
		}
		else if (one_prop->Name == offset_y && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_y_value = one_prop->GetDoubleValue();
			is_y_correct = true;
		}
		else if (one_prop->Name == offset_z && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_z_value = one_prop->GetDoubleValue();
			is_z_correct = true;
		}
		else if (one_prop->Name == angle && one_prop->GetType() == Renga::PropertyType::PropertyType_Angle) {
			angle_value = one_prop->GetAngleValue(Renga::AngleUnit::AngleUnit_Degrees);
			angle_correct = true;
		}
	}*/
	
	std::vector<double> find_transform_parameters{ offset_x_value,offset_y_value,offset_z_value,angle_value };
	this->projects_offset = find_transform_parameters;
}
void renga_data::get_data() {

	Renga::IDataExporterPtr pDataExporter = this->r_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = this->r_project->GetModel()->GetObjects();
	int all_objects = objects_collection3d->GetCount();

	for (int counter_objects = 0; counter_objects < objects_collection3d->GetCount(); counter_objects++)
	{


		std::vector<std::vector<grid_description>> object_geom;
		Renga::IExportedObject3DPtr current_model_object_geometry = objects_collection3d->Get(counter_objects);
		Renga::IModelObjectPtr current_model_object = objects_collection->GetById(current_model_object_geometry->GetModelObjectId());

		std::vector<double> color_object;
		double transparency_object;
		this->get_material(current_model_object, &color_object, &transparency_object);

		GUID obj_type = current_model_object->GetObjectType();
		tools::log_status_work(all_objects, counter_objects, "reading", current_model_object->GetName());
		for (int counter_meshes = 0; counter_meshes < current_model_object_geometry->GetMeshCount(); counter_meshes++)
		{
			std::vector<grid_description> mesh_description;
			Renga::IMeshPtr pMesh = current_model_object_geometry->GetMesh(counter_meshes);

			std::vector<std::vector<double>> grid_triangles;
			for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
			{
				std::vector<std::vector<std::vector<double>>> grid_geometry;
				
				Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
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
				for (int counter_triangles = 0; counter_triangles < pGrid->GetTriangleCount(); counter_triangles++)
				{
					std::vector<std::vector<double>> triangle_geometry;

					Renga::Triangle triangle_definition = pGrid->GetTriangle(counter_triangles);

					Renga::FloatPoint3D p1 = pGrid->GetVertex(triangle_definition.V0);
					Renga::FloatPoint3D p2 = pGrid->GetVertex(triangle_definition.V1);
					Renga::FloatPoint3D p3 = pGrid->GetVertex(triangle_definition.V2);

					std::vector<double> transformed_1 = tools::get_transformed_coords(p1.X / 1000, p1.Y / 1000, p1.Z / 1000, this->projects_offset);
					std::vector<double> transformed_2 = tools::get_transformed_coords(p2.X / 1000, p2.Y / 1000, p2.Z / 1000, this->projects_offset);
					std::vector<double> transformed_3 = tools::get_transformed_coords(p3.X / 1000, p3.Y / 1000, p3.Z / 1000, this->projects_offset);

					triangle_geometry.push_back({ transformed_1.at(0), transformed_1.at(1), transformed_1.at(2) });
					triangle_geometry.push_back({ transformed_2.at(0), transformed_2.at(1), transformed_2.at(2) });
					triangle_geometry.push_back({ transformed_3.at(0), transformed_3.at(1), transformed_3.at(2) });
					grid_geometry.push_back(triangle_geometry);
				}
				grid_description new_grid(grid_geometry, color, transparency);
				mesh_description.push_back(new_grid);
			}
			object_geom.push_back(mesh_description);
		}
		this->grids_collection_in_meshes.push_back(object_geom);
	}
	tools::log_status_work(all_objects, all_objects, "reading", "");
}
void renga_data::get_material_grid(GUID object_type, int grid_type, Renga::IGridPtr link_grid, std::vector<double>* link_color, double* link_transparency)
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
void renga_data::get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color, double* link_transparency)
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
void renga_data::sort_layers()
{
	Renga::IDataExporterPtr pDataExporter = this->r_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = this->r_project->GetModel()->GetObjects();

	//Getting levels
	std::list<Renga::IModelObjectPtr> m_levels;
	for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++)
	{
		Renga::IModelObjectPtr pModelObject = objects_collection->GetByIndex(counter_object);
		if (pModelObject->GetObjectType() == Renga::ObjectTypes::Level)
		{
			m_levels.push_back(pModelObject);
		}
	}
	//Sort levels
	//std::cout << "start sort layers by elevation\n";
	m_levels.sort(tools::compare_levels);
	
	for (Renga::IModelObjectPtr one_level : m_levels)
	{
		this->id2level.insert(std::pair<int, Renga::IModelObjectPtr>(one_level->Id, one_level));
	}
	//std::cout << "end level work, start getting objects to each level" << std::endl;
	tools::level2ids(this->r_project, &this->level2objects, &this->non_level_objects, &m_levels);
}