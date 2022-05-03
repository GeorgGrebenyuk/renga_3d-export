#include "navis.h"

navis_object::navis_object(Renga::IProjectPtr project_input, std::vector<std::vector<grid_description>>* object_info, LcNwcGroup* parent_element, Renga::IModelObjectPtr obj)
{
	this->project = project_input;
	//int internal_object_id = this->current_model_object_geometry->GetModelObjectId();
	this->current_model_object = obj;

	LcNwcGroup one_object_instance;
	one_object_instance.SetLayer(TRUE);
	one_object_instance.SetName(this->current_model_object->GetName());

	LcNwcGroup geometry_data;
	one_object_instance.SetLayer(TRUE);

	for (std::vector<grid_description> mesh_info : (*object_info))
	{
		LcNwcGroup mesh_data;
		mesh_data.SetLayer(TRUE);
		mesh_data.SetName(L"MESH");
		for (grid_description grid_info : mesh_info)
		{
			LcNwcGeometry grid_data;
			//LcNwcGroup grid_data;
			//grid_data.SetLayer(TRUE);
			grid_data.SetName(L"GRID");

			LcNwcMaterial body_material;
			body_material.SetDiffuseColor(grid_info.color[0], grid_info.color[1], grid_info.color[2]);
			body_material.SetAmbientColor(grid_info.color[0], grid_info.color[1], grid_info.color[2]);
			body_material.SetTransparency(grid_info.transparency);
			grid_data.AddAttribute(body_material);

			LcNwcGeometryStream stream_grid_record = grid_data.OpenStream();

			for (std::vector<std::vector<double>> triangle_info : grid_info.geometry)
			{
				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				for (std::vector<double> one_point : triangle_info)
				{
					stream_grid_record.TriFanVertex(one_point[0], one_point[1], one_point[2]);
				}
				stream_grid_record.End();
			}
			grid_data.CloseStream(stream_grid_record);
			//grid_data.AddNode(grid_data);
			mesh_data.AddNode(grid_data);
		}
		geometry_data.AddNode(mesh_data);
	}
	one_object_instance.AddNode(geometry_data);
	this->getting_properties(&one_object_instance);

	//std::cout << this->current_model_object->GetName();
	(*parent_element).AddNode(one_object_instance);

}
void navis_object::getting_properties(LcNwcGroup* object_defenition)
{
	//Internal properies
	Renga::IPropertyContainerPtr props_usual = this->current_model_object->GetProperties();
	Renga::IGuidCollectionPtr props_ids = props_usual->GetIds();
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

	//Quantity properties
	Renga::IQuantityContainerPtr  pQuantityContainer = this->current_model_object->GetQuantities();
	LcNwcPropertyAttribute quantity_props;
	quantity_props.SetClassName(L"Renga_Quantity_props", "Расчетные характеристики");


	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::Area))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"Area", "Площадь, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::Count))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetInt32(pQuantity->AsCount());
		quantity_props.AddProperty(L"Count", "Количество, шт.", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::CrossSectionOverallHeight))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsLength(Renga::LengthUnit_Millimeters));
		quantity_props.AddProperty(L"CrossSectionOverallHeight", "Общая высота поперечного сечения (или профиля) объекта, мм.", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::CrossSectionOverallWidth))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsLength(Renga::LengthUnit_Millimeters));
		quantity_props.AddProperty(L"CrossSectionOverallWidth", "Общая ширина поперечного сечения (или профиля) объекта, мм.", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GlazingArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GlazingArea", "Площадь остекления окна или двери, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossArea", "Общая площадь объекта или слоя, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossCeilingArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossCeilingArea", "Общая площадь потолка помещения, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossCrossSectionArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossCrossSectionArea", "Площадь поперечного сечения, включая внутреннее пространство, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossFloorArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossFloorArea", "Общая площадь потолка помещения, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossMass))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsMass(Renga::MassUnit_Kilograms));
		quantity_props.AddProperty(L"GrossMass", "Общая масса объекта, кг.", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossPerimeter))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsLength(Renga::LengthUnit_Millimeters));
		quantity_props.AddProperty(L"GrossPerimeter", "Общий периметр объекта, м.", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossSideArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossSideArea", "Общая площадь вдоль базовой линии без элементов обработки, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossSideAreaLeft))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossSideAreaLeft", "Общая площадь вдоль крайней левой линии без элементов обработки, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossSideAreaRight))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossSideAreaRight", "Общая площадь вдоль крайней правой линии без элементов обработки, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossVolume))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetVolumeFloat(pQuantity->AsVolume(Renga::VolumeUnit_Meters3));
		quantity_props.AddProperty(L"GrossVolume", "Общий внутренний объем объекта, м3", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::GrossWallArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"GrossWallArea", "Общая площадь стен комнаты, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::InnerSurfaceArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"InnerSurfaceArea", "Общая площадь внутренней поверхности, м2", one_quan_property);
	}
	if (auto pQuantity = pQuantityContainer->Get(Renga::QuantityIds::InnerSurfaceExternalArea))
	{
		LcNwcData one_quan_property;
		one_quan_property.SetAreaFloat(pQuantity->AsArea(Renga::AreaUnit_Meters2));
		quantity_props.AddProperty(L"InnerSurfaceExternalArea", "Общая площадь внутренней поверхности (внешней по отношению к раме), м2", one_quan_property);
	}

	(*object_defenition).AddAttribute(internal_props);
	(*object_defenition).AddAttribute(quantity_props);
}