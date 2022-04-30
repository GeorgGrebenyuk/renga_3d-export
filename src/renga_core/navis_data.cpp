#include "actions.h"
navis_object::navis_object(Renga::IProjectPtr project_input, std::vector<double>* offset_parameters, LcNwcGroup* parent_element, int object_id_in_exporting)
{
	this->project = project_input;
	this->internal_offset_parameters = *offset_parameters;
	this->object_id_in_exporting = object_id_in_exporting;

	this->current_model_object_geometry = this->project->GetDataExporter()->GetObjects3D()->Get(object_id_in_exporting);
	int internal_object_id = this->current_model_object_geometry->GetModelObjectId();
	this->current_model_object = this->project->GetModel()->GetObjects()->GetById(internal_object_id);
	
	Renga::ILevelObjectPtr object_on_level;
	this->current_model_object->QueryInterface(&object_on_level);

	LcNwcGroup one_object_instance;

	one_object_instance.SetLayer(TRUE);
	one_object_instance.SetName(this->current_model_object->GetName());

	this->getting_color(&one_object_instance);
	this->create_geometry(&one_object_instance);
	this->getting_properties(&one_object_instance);

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
					std::cout << "error " << this->current_model_object->GetName() << std::endl;
				}
			}
		}
	}
	LcNwcMaterial body_material;
	body_material.SetDiffuseColor(object_color[0], object_color[1], object_color[2]);
	body_material.SetAmbientColor(object_color[0], object_color[1], object_color[2]);
	(*object_defenition).AddAttribute(body_material);
}
void navis_object::create_geometry(LcNwcGroup* object_defenition)
{
	for (int counter_meshes = 0; counter_meshes < this->current_model_object_geometry->GetMeshCount(); counter_meshes++)
	{
		Renga::IMeshPtr pMesh = this->current_model_object_geometry->GetMesh(counter_meshes);
		LcNwcGroup one_mesh;
		one_mesh.SetLayer(TRUE);
		one_mesh.SetName(L"one_internal_mesh");

		for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
		{
			Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
			LcNwcGeometry grid_triangles_geometry;
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
				if (this->internal_offset_parameters.at(0) == 0.0 && this->internal_offset_parameters.at(1) == 0.0
					&& this->internal_offset_parameters.at(2) == 0.0 && this->internal_offset_parameters.at(3) == 0.0)
				{
					stream_grid_record.TriFanVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					stream_grid_record.TriFanVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					stream_grid_record.TriFanVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);
				}
				else
				{
					std::vector<double> transformed_1 = tools::get_transformed_coords(p1.X / 1000, p1.Z / 1000, p1.Y / 1000, this->internal_offset_parameters);
					std::vector<double> transformed_2 = tools::get_transformed_coords(p2.X / 1000, p2.Z / 1000, p2.Y / 1000, this->internal_offset_parameters);
					std::vector<double> transformed_3 = tools::get_transformed_coords(p3.X / 1000, p3.Z / 1000, p3.Y / 1000, this->internal_offset_parameters);

					stream_grid_record.TriFanVertex(transformed_1.at(0), transformed_1.at(2), transformed_1.at(1));
					stream_grid_record.TriFanVertex(transformed_2.at(0), transformed_2.at(2), transformed_2.at(1));
					stream_grid_record.TriFanVertex(transformed_3.at(0), transformed_3.at(2), transformed_3.at(1));
				}

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
	std::vector <Renga::IPropertyPtr> props_as_props;
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
		(*object_defenition).AddAttribute(internal_props);
	}
}