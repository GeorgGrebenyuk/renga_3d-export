#include "actions.h"
#include <chrono>
navis_object::navis_object(Renga::IProjectPtr project_input, std::vector<double>* offset_parameters, 
	LcNwcGroup* parent_element, Renga::IExportedObject3DPtr obj ) //int object_id_in_exporting
{
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	

	this->project = project_input;
	this->internal_offset_parameters = *offset_parameters;
	//this->object_id_in_exporting = object_id_in_exporting;

	//this->current_model_object_geometry = this->project->GetDataExporter()->GetObjects3D()->Get(object_id_in_exporting);
	this->current_model_object_geometry = obj;
	std::chrono::steady_clock::time_point end0 = std::chrono::steady_clock::now();
	long long time_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end0 - begin).count();
	//std::cout << "gettind internal object = " << time_1 << std::endl;

	int internal_object_id = this->current_model_object_geometry->GetModelObjectId();
	this->current_model_object = this->project->GetModel()->GetObjects()->GetById(internal_object_id);

	std::chrono::steady_clock::time_point end1 = std::chrono::steady_clock::now();
	long long time_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - end0).count();
	//std::cout << "gettind model object = " << time_2 << std::endl;


	LcNwcGroup one_object_instance;

	one_object_instance.SetLayer(TRUE);
	one_object_instance.SetName(this->current_model_object->GetName());

	GUID obj_type = this->current_model_object->GetObjectType();
	if (obj_type != Renga::ObjectTypes::Window && obj_type != Renga::ObjectTypes::Door)
	{
		this->getting_color(&one_object_instance);
	}
	//else - init color in geometry block (by Grid type)
	
	std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
	long long time_3 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - end1).count();
	//std::cout << "gettind colors = " << time_3 << std::endl;

	this->create_geometry(&one_object_instance);
	std::chrono::steady_clock::time_point end3 = std::chrono::steady_clock::now();
	long long time_4 = std::chrono::duration_cast<std::chrono::milliseconds>(end3 - end2).count();
	//std::cout << "gettind geometry = " <<time_4<< std::endl;

	this->getting_properties(&one_object_instance);
	std::chrono::steady_clock::time_point end4 = std::chrono::steady_clock::now();
	long long time_5 = std::chrono::duration_cast<std::chrono::milliseconds>(end4 - end3).count();
	//std::cout << "gettind props = " <<time_5 << std::endl;

	std::string str_type = tools::get_type_str(this->current_model_object->GetObjectType());
	std::cout << str_type << "\tgeometry getting time = " << time_4 << "ms" << std::endl;// +";" << time_1 << ";" << time_2 << ";" << time_3 << ";" << time_4 << ";" << time_5 << std::endl;

	(*parent_element).AddNode(one_object_instance);
}
void navis_object::getting_color(LcNwcGroup* object_defenition)
{
	std::vector<double> object_color = { 1.0,1.0,1.0 };
	Renga::IObjectWithMaterialPtr pObjectWithMaterial;
	this->current_model_object->QueryInterface(&pObjectWithMaterial);
	if (pObjectWithMaterial)
	{
		if (Renga::IMaterialPtr obj_material = this->project->GetMaterialManager()->GetMaterial(pObjectWithMaterial->GetMaterialId())) {
			Renga::Color mat_color = obj_material->GetColor();

			double color_red(mat_color.Red / 255.0);
			double color_green(mat_color.Green / 255.0);
			double color_blue(mat_color.Blue / 255.0);
			object_color = { color_red ,color_green,color_blue };
			//return object_color;
		}

	}
	Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
	this->current_model_object->QueryInterface(&pObjectWithLayeredMaterial);
	if (pObjectWithLayeredMaterial) {
		if (Renga::ILayeredMaterialPtr pLayeredMaterial = this->project->GetLayeredMaterialManager()
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
	LcNwcMaterial body_material;
	body_material.SetDiffuseColor(object_color[0], object_color[1], object_color[2]);
	body_material.SetAmbientColor(object_color[0], object_color[1], object_color[2]);
	(*object_defenition).AddAttribute(body_material);
}
void navis_object::getting_color_grid(LcNwcGeometry* geometry_definition, GUID* object_type, int *grid_type)
{
	std::vector<double> object_color = { 1.0,1.0,1.0 };
	LcNwcMaterial body_material;
	if ((*object_type) == Renga::ObjectTypes::Window)
	{
		switch (*grid_type)
		{
		case Renga::GridTypes::Window::Frame:
			object_color = {0.729,0.596,0.274};
			break;
		case Renga::GridTypes::Window::Glass:
			object_color = { 0.760,0.964,0.929};
			body_material.SetTransparency(0.6);
			break;
		case Renga::GridTypes::Window::Sill:
			object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::OutwardSill:
			object_color = { 0.674,0.674,0.674 };
			break;
		case Renga::GridTypes::Window::Reveal:
			body_material.SetTransparency(0.1);
			break;
		}
	}
	else if ((*object_type) == Renga::ObjectTypes::Door)
	{
		switch (*grid_type)
		{
		case Renga::GridTypes::Door::Glass:
			object_color = { 0.760,0.964,0.929 };
			body_material.SetTransparency(0.6);
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
	
	body_material.SetDiffuseColor(object_color[0], object_color[1], object_color[2]);
	body_material.SetAmbientColor(object_color[0], object_color[1], object_color[2]);
	(*geometry_definition).AddAttribute(body_material);
}
void navis_object::create_geometry(LcNwcGroup* object_defenition)
{
	auto obj_type = this->current_model_object->GetObjectType();
	for (int counter_meshes = 0; counter_meshes < this->current_model_object_geometry->GetMeshCount(); counter_meshes++)
	{
		Renga::IMeshPtr pMesh = this->current_model_object_geometry->GetMesh(counter_meshes);
		LcNwcGroup one_mesh;
		one_mesh.SetLayer(FALSE);
		one_mesh.SetName(L"one_internal_mesh");

		for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
		{
			Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
			LcNwcGeometry grid_triangles_geometry;
			int grid_type = pGrid->GetGridType();
			if (obj_type == Renga::ObjectTypes::Window | obj_type == Renga::ObjectTypes::Door)
			{
				this->getting_color_grid(&grid_triangles_geometry, &obj_type, &grid_type);
			}

			grid_triangles_geometry.SetName(L"one_internal_grid");
			LcNwcGeometryStream stream_grid_record = grid_triangles_geometry.OpenStream();
			for (int counter_triangles = 0; counter_triangles < pGrid->GetTriangleCount(); counter_triangles++)
			{
				
				Renga::Triangle triangle_definition = pGrid->GetTriangle(counter_triangles);

				Renga::FloatPoint3D p1 = pGrid->GetVertex(triangle_definition.V0);
				Renga::FloatPoint3D p2 = pGrid->GetVertex(triangle_definition.V1);
				Renga::FloatPoint3D p3 = pGrid->GetVertex(triangle_definition.V2);

				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				//std::vector<double> finded_parameters = params;

				std::vector<double> transformed_1 = tools::get_transformed_coords(p1.X / 1000, p1.Y / 1000, p1.Z / 1000, this->internal_offset_parameters);
				std::vector<double> transformed_2 = tools::get_transformed_coords(p2.X / 1000, p2.Y / 1000, p2.Z / 1000, this->internal_offset_parameters);
				std::vector<double> transformed_3 = tools::get_transformed_coords(p3.X / 1000, p3.Y / 1000, p3.Z / 1000, this->internal_offset_parameters);

				stream_grid_record.TriFanVertex(transformed_1.at(0), transformed_1.at(1), transformed_1.at(2));
				stream_grid_record.TriFanVertex(transformed_2.at(0), transformed_2.at(1), transformed_2.at(2));
				stream_grid_record.TriFanVertex(transformed_3.at(0), transformed_3.at(1), transformed_3.at(2));

				stream_grid_record.End();
			}
			grid_triangles_geometry.CloseStream(stream_grid_record);
			one_mesh.AddNode(grid_triangles_geometry);
		}
		(*object_defenition).AddNode(one_mesh);
	}
}
void navis_object::getting_properties(LcNwcGroup* object_defenition)
{
	Renga::IPropertyContainerPtr props_usual = this->current_model_object->GetProperties();

	Renga::IGuidCollectionPtr props_ids = props_usual->GetIds();

	//Internal properies
	LcNwcPropertyAttribute internal_props;
	internal_props.SetClassName(L"Renga_Properies", "Внутренние свойства");
	//std::vector <Renga::IPropertyPtr> props_as_props;
	for (int counter_property = 0; counter_property < props_ids->GetCount(); counter_property++)
	{
		GUID id = props_ids->Get(counter_property);
		Renga::IPropertyPtr one_prop = props_usual->Get(id);

		LcNwcData one_property;
		const char* internal_name;
		switch (one_prop->Type) {
		case Renga::PropertyType::PropertyType_Double:
			one_property.SetFloat(one_prop->GetDoubleValue());
			break;
		case Renga::PropertyType::PropertyType_Area:
			one_property.SetAreaFloat(one_prop->GetDoubleValue());
			break;
		case Renga::PropertyType::PropertyType_Boolean:
			one_property.SetBoolean(one_prop->GetBooleanValue());
			break;
		case Renga::PropertyType::PropertyType_Logical:
			one_property.SetBoolean(one_prop->GetBooleanValue());
			break;
		case Renga::PropertyType::PropertyType_Integer:
			one_property.SetInt32(one_prop->GetIntegerValue());
			break;
		case Renga::PropertyType::PropertyType_Length:
			one_property.SetLinearFloat(one_prop->GetLengthValue(Renga::LengthUnit::LengthUnit_Millimeters));
		case Renga::PropertyType::PropertyType_Mass:
			one_property.SetFloat(one_prop->GetMassValue(Renga::MassUnit::MassUnit_Kilograms));
			internal_name = one_prop->Name + "kg";
			break;
		case Renga::PropertyType::PropertyType_Angle:
			one_property.SetAngularFloat(one_prop->GetAngleValue(Renga::AngleUnit::AngleUnit_Degrees));
			internal_name = one_prop->Name + "degree";
			break;
		case Renga::PropertyType::PropertyType_String:
			one_property.SetWideString(one_prop->GetStringValue());
			break;
		case Renga::PropertyType::PropertyType_Volume:
			one_property.SetVolumeFloat(one_prop->GetVolumeValue(Renga::VolumeUnit::VolumeUnit_Meters3));
			internal_name = one_prop->Name + "meters3";
		}
		internal_props.AddProperty(one_prop->Name, one_prop->Name, one_property);
	}
	(*object_defenition).AddAttribute(internal_props);
}