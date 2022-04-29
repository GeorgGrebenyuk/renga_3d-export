#pragma once
#include <windows.h>
#include "string"
#include "Renga_import.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "renga_enums.h"
#include "nwcreate/LiNwcAll.h"

template <typename prop_name_type, typename Prop_type>
class prop_internal {
private:
	prop_name_type name;
	Prop_type value;
public:
	prop_internal(prop_name_type name_input, Prop_type value_input) : value(value_input), name(name_input)
	{ }
	Prop_type get_value() {
		return value;
	}
	prop_name_type get_value() {
		return name;
	}
	
};
class renga_data {
public:
	renga_data(const char* project_filepath, int type_conversion);
	std::wstring file_export_path;

	int type;
private:
	Renga::IProjectPtr r_project;
	Renga::IApplicationPtr r_app;
	void navis_init();
	void navis_export();
	std::map<int, std::vector<unsigned short>> grid2color;
	std::vector<double> object_color (Renga::IModelObjectPtr obj);
};
static class tools {
public:
	//static Renga::IProjectPtr renga_project;
	static bool compare_levels(Renga::IModelObjectPtr l1, Renga::IModelObjectPtr l2) {
		
		Renga::ILevelPtr pLevel1;
		l1->QueryInterface(&pLevel1);
		Renga::ILevelPtr pLevel2;
		l2->QueryInterface(&pLevel2);

		assert(pLevel1 != nullptr);
		assert(pLevel2 != nullptr);
		double l1_h = pLevel1->GetElevation();
		double l2_h = pLevel2->GetElevation();
		return l1_h < l2_h;
	}
	//static std::vector<double> init_internal_offset(Renga::IBuildingInfo building_info) {
	//	std::vector<double> output_paramaters{ 0.0,0.0,0.0,0.0 };
	//	
	//}
	static LcNwcPropertyAttribute get_props_by_object(Renga::IModelObjectPtr obj) {
		Renga::IPropertyContainerPtr props_usual = obj->GetProperties();
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
		}
	

		return internal_props;
	}
	static void get_object_geometry(Renga::IExportedObject3DPtr internal_object, LcNwcGroup *link_object)
	{
		for (int counter_meshes = 0; counter_meshes < internal_object->GetMeshCount(); counter_meshes++)
		{
			Renga::IMeshPtr pMesh = internal_object->GetMesh(counter_meshes);
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
					stream_grid_record.TriFanVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					stream_grid_record.TriFanVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					stream_grid_record.TriFanVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);
					stream_grid_record.End();
				}
				grid_triangles_geometry.CloseStream(stream_grid_record);
				one_mesh.AddNode(grid_triangles_geometry);
			}
			(*link_object).AddNode(one_mesh);
		}
	}
	static void nwc_object (int *exported_id, LcNwcGroup *link_group,
		Renga::IProjectPtr project_link)
	{

		Renga::IExportedObject3DPtr internal_object = project_link->GetDataExporter()->GetObjects3D()->Get(*exported_id);
		int internal_object_id = internal_object->GetModelObjectId();
		Renga::IModelObjectPtr object_data = project_link->GetModel()->GetObjects()->GetById(internal_object_id);
		
		Renga::ILevelObjectPtr object_on_level;
		object_data->QueryInterface(&object_on_level);

		LcNwcGroup one_object_instance;
		
		one_object_instance.SetLayer(TRUE);
		one_object_instance.SetName(object_data->GetName());

		std::vector<double> color_of_object = object_color(object_data, project_link);
		LcNwcMaterial body_material;
		body_material.SetDiffuseColor(color_of_object[0], color_of_object[1], color_of_object[2]);
		body_material.SetAmbientColor(color_of_object[0], color_of_object[1], color_of_object[2]);
		one_object_instance.AddAttribute(body_material);

		tools::get_object_geometry(internal_object, &one_object_instance);
		LcNwcPropertyAttribute internal_props = tools::get_props_by_object(object_data);
		one_object_instance.AddAttribute(internal_props);

		(*link_group).AddNode(one_object_instance);
	}
	static std::map<Renga::ILevelObjectPtr, std::list<int>> level2ids;
	static void add_object_to_level_list(Renga::ILevelObjectPtr level, int obj_id)
	{
		Renga::IModelObjectPtr l_obj;
		level->QueryInterface(&l_obj);
		if (!(level2ids).empty())
		{
			bool is_level_in_map = false;
			for (std::pair<Renga::ILevelObjectPtr, std::list<int>> one_pair : (level2ids))
			{
				Renga::IModelObjectPtr m_obj;
				one_pair.first->QueryInterface(&m_obj);
				if (m_obj->GetId() == l_obj->GetId())
				{
					is_level_in_map = true;

					one_pair.second.push_back(obj_id);
					break;
				}	
			}
			if (!is_level_in_map)
			{
				(level2ids).insert(std::pair<Renga::ILevelObjectPtr, std::list<int>>(level, std::list<int>{obj_id}));
			}
			
		}
		else (level2ids).insert(std::pair<Renga::ILevelObjectPtr, std::list<int>>(level, std::list<int>{obj_id}));
	}
	static std::vector<double> object_color(Renga::IModelObjectPtr obj, Renga::IProjectPtr project_link) {

		std::vector<double> object_color = { 1.0,1.0,1.0 };

		Renga::IObjectWithMaterialPtr pObjectWithMaterial;
		obj->QueryInterface(&pObjectWithMaterial);
		if (pObjectWithMaterial)
		{
			if (Renga::IMaterialPtr obj_material = project_link->GetMaterialManager()->GetMaterial(pObjectWithMaterial->GetMaterialId())) {
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
			if (Renga::ILayeredMaterialPtr pLayeredMaterial = project_link->GetLayeredMaterialManager()
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
						std::cout << "error " << obj->GetName() << std::endl;
					}

				}

			}
		}

		return object_color;
	}
};
