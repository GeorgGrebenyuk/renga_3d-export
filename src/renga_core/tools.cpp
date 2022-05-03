#include "general_functions.h"
void tools::level2ids(Renga::IProjectPtr link_project,std::list<level_objects>* level2object, 
	std::list<int>* non_level_objects, std::list<Renga::IModelObjectPtr>* m_levels)
{
	Renga::IDataExporterPtr pDataExporter = link_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = link_project->GetModel()->GetObjects();

	int all_3d = objects_collection3d->GetCount();
	for (int counter_object = 0; counter_object < all_3d; counter_object++)
	{
		Renga::IExportedObject3DPtr internal_object = objects_collection3d->Get(counter_object);
		//int internal_object_id = internal_object->GetModelObjectId();
		Renga::IModelObjectPtr pModelObject = objects_collection->GetById(internal_object->GetModelObjectId());
		Renga::ILevelObjectPtr pLevelObject;
		pModelObject->QueryInterface(&pLevelObject);

		auto type = pModelObject->ObjectType;
		bool is_level = (std::find(c_levelTreeTypes.begin(), c_levelTreeTypes.end(), pModelObject->ObjectType) != c_levelTreeTypes.end());
		if (is_level) 
		{
			//Exception for non level id in object properties
			bool find_level = false;
			for (Renga::IModelObjectPtr level : (*m_levels))
			{
				if (level->Id == pLevelObject->LevelId)
				{
					find_level = true;
					break;
				}
			}
			if (!find_level) is_level = false;
		}

		//bool is_level_non = (std::find(c_nonLevelTreeTypes.begin(), c_nonLevelTreeTypes.end(), pModelObject->ObjectType) != c_nonLevelTreeTypes.end());

		//Leveld not even is equal valid level in model -- need check ...
		if (is_level)
		{
			//level_objects new_item;
			//new_item.ids.push_back(counter_object);
			//new_item.level_model_id = pLevelObject->LevelId;
			if (!(*level2object).empty())
			{
				bool level_find_status = false;
				for (level_objects &one_group : (*level2object))
				{
					if (one_group.level_model_id == pLevelObject->LevelId)
					{
						one_group.add_item(counter_object);
						//one_group.ids.push_back(counter_object);
						level_find_status = true;
						break;
					}
				}
				if (!level_find_status)
				{
					(*level2object).push_back(level_objects(pLevelObject->LevelId, counter_object));
					//(*level2object).push_back(new_item);
				}
			}
			else 
			{
				(*level2object).push_back(level_objects(pLevelObject->LevelId, counter_object));
				//(*level2object).push_back(new_item);
			}
		}
		else
		{
			(*non_level_objects).push_back(counter_object);
		}
	}
}
std::string tools::get_type_str(GUID obj_type)
{
	std::string out_type;
	if (obj_type == Renga::ObjectTypes::Undefined) out_type = "Unknown";
	else if (obj_type == Renga::ObjectTypes::Level) out_type = "Level";
	else if (obj_type == Renga::ObjectTypes::Wall) out_type = "Wall";
	else if (obj_type == Renga::ObjectTypes::Column) out_type = "Column";
	else if (obj_type == Renga::ObjectTypes::Beam) out_type = "Beam";
	else if (obj_type == Renga::ObjectTypes::Floor) out_type = "Floor";
	else if (obj_type == Renga::ObjectTypes::Opening) out_type = "Opening";
	else if (obj_type == Renga::ObjectTypes::Roof) out_type = "Roof";
	else if (obj_type == Renga::ObjectTypes::Ramp) out_type = "Ramp";
	else if (obj_type == Renga::ObjectTypes::Stair) out_type = "Stair";
	else if (obj_type == Renga::ObjectTypes::Door) out_type = "Door";
	else if (obj_type == Renga::ObjectTypes::Window) out_type = "Window";
	else if (obj_type == Renga::ObjectTypes::Railing) out_type = "Railing";
	else if (obj_type == Renga::ObjectTypes::IsolatedFoundation) out_type = "IsolatedFoundation";
	else if (obj_type == Renga::ObjectTypes::WallFoundation) out_type = "WallFoundation";
	else if (obj_type == Renga::ObjectTypes::Room) out_type = "Room";
	else if (obj_type == Renga::ObjectTypes::AssemblyInstance) out_type = "AssemblyInstance";
	else if (obj_type == Renga::ObjectTypes::Element) out_type = "Element";
	else if (obj_type == Renga::ObjectTypes::RoutePoint) out_type = "RoutePoint";
	else if (obj_type == Renga::ObjectTypes::Route) out_type = "Route";
	else if (obj_type == Renga::ObjectTypes::PlumbingFixture) out_type = "PlumbingFixture";
	else if (obj_type == Renga::ObjectTypes::Equipment) out_type = "Equipment";
	else if (obj_type == Renga::ObjectTypes::MechanicalEquipment) out_type = "MechanicalEquipment";
	else if (obj_type == Renga::ObjectTypes::Pipe) out_type = "Pipe";
	else if (obj_type == Renga::ObjectTypes::PipeAccessory) out_type = "PipeAccessory";
	else if (obj_type == Renga::ObjectTypes::DuctAccessory) out_type = "DuctAccessory";
	else if (obj_type == Renga::ObjectTypes::PipeFitting) out_type = "PipeFitting";
	else if (obj_type == Renga::ObjectTypes::DuctFitting) out_type = "DuctFitting";
	else if (obj_type == Renga::ObjectTypes::IfcObject) out_type = "IfcObject";
	else if (obj_type == Renga::ObjectTypes::Rebar) out_type = "Rebar";
	else if (obj_type == Renga::ObjectTypes::Axis) out_type = "Axis";
	else if (obj_type == Renga::ObjectTypes::Section) out_type = "Section";
	else if (obj_type == Renga::ObjectTypes::Elevation) out_type = "Elevation";
	else if (obj_type == Renga::ObjectTypes::Plate) out_type = "Plate";
	else if (obj_type == Renga::ObjectTypes::Line3D) out_type = "Line3D";
	else if (obj_type == Renga::ObjectTypes::TextShape) out_type = "TextShape";
	else if (obj_type == Renga::ObjectTypes::Hatch) out_type = "Hatch";
	else if (obj_type == Renga::ObjectTypes::Dimension) out_type = "Dimension";
	else if (obj_type == Renga::ObjectTypes::Duct) out_type = "Duct";
	else if (obj_type == Renga::ObjectTypes::LineElectricalCircuit) out_type = "LineElectricalCircuit";
	else if (obj_type == Renga::ObjectTypes::LightFixture) out_type = "LightFixture";
	else if (obj_type == Renga::ObjectTypes::WiringAccessory) out_type = "WiringAccessory";
	else if (obj_type == Renga::ObjectTypes::ElectricDistributionBoard) out_type = "ElectricDistributionBoard";
	else out_type = "Unknown type";

	return out_type;
}
void tools::get_quanity_prop(GUID quanity_prop, std::string* rus_name, std::string* eng_name, std::string* eng_code, std::string* rus_code)
{
	if (quanity_prop == Renga::QuantityIds::Area)
	{
		(*eng_code) = "NominalThickness";
		(*rus_code) = "Номинальная толщина";
		(*eng_name) = "The nominal thickness of an object or a layer";
		(*rus_name) = "Номинальная толщина объекта или слоя";
	}
	else if (quanity_prop == Renga::QuantityIds::NominalWidth)
	{
		(*eng_code) = "NominalWidth";
		(*rus_code) = "Номинальная ширина";
		(*eng_name) = "The nominal width of an object";
		(*rus_name) = "Номинальная ширина объекта.";
	}
	else if (quanity_prop == Renga::QuantityIds::NominalHeight)
	{
		(*eng_code) = "NominalHeight";
		(*rus_code) = "Номинальная высота";
		(*eng_name) = "The nominal height of an object or a layer";
		(*rus_name) = "Номинальная высота объекта или слоя";
	}
	else if (quanity_prop == Renga::QuantityIds::Perimeter)
	{
		(*eng_code) = "Perimeter";
		(*rus_code) = "Периметр";
		(*eng_name) = "The perimeter of an object";
		(*rus_name) = "Периметр объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::OverallWidth)
	{
		(*eng_code) = "OverallWidth";
		(*rus_code) = "Общая ширина";
		(*eng_name) = "The overall width measure. It equals the width of the bounding box of an object.";
		(*rus_name) = "Ширина ограничивающей рамки объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::OverallHeight)
	{
		(*eng_code) = "OverallHeight";
		(*rus_code) = "Общая ширина";
		(*eng_name) = "The overall height measure. It equals the width of the bounding box of an object.";
		(*rus_name) = "Высота ограничивающей рамки объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::OverallLength)
	{
		(*eng_code) = "OverallLength";
		(*rus_code) = "Общая длина";
		(*eng_name) = "The overall length measure. It equals the width of the bounding box of an object.";
		(*rus_name) = "Длина ограничивающей рамки объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::OverallDepth)
	{
		(*eng_code) = "OverallDepth";
		(*rus_code) = "Общая глубина";
		(*eng_name) = "The overall depth measure. It equals the width of the bounding box of an object.";
		(*rus_name) = "Глубина ограничивающей рамки объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::Volume)
	{
		(*eng_code) = "Volume";
		(*rus_code) = "Объем";
		(*eng_name) = "Overall volume measure";
		(*rus_name) = "Общий объем";
	}
	else if (quanity_prop == Renga::QuantityIds::NetVolume)
	{
		(*eng_code) = "NetVolume";
		(*rus_code) = "Абсолютный (чистый) объем";
		(*eng_name) = "The total net volume of an object or a layer, taking into account possible processing features (cut-outs, etc.) or openings and recesses";
		(*rus_name) = "Общий чистый объем объекта или слоя с учетом возможных особенностей обработки (вырезы и т.д.) или отверстий и углублений";
	}
	else if (quanity_prop == Renga::QuantityIds::NetMass)
	{
		(*eng_code) = "NetMass";
		(*rus_code) = "Абсолютная (чистая) масса";
		(*eng_name) = "The total net mass of an object or a layer, taking into account possible processing features (cut-outs, etc.) or openings and recesses";
		(*rus_name) = "Общая чистая масса объекта или слоя с учетом возможных особенностей обработки (вырезы и т.д.) или отверстий и углублений";
	}
	else if (quanity_prop == Renga::QuantityIds::OuterSurfaceArea)
	{
		(*eng_code) = "OuterSurfaceArea";
		(*rus_code) = "Площадь выдавленных поверхностей";
		(*eng_name) = "The total area of extruded surfaces of an object (not taking into account the end cap areas), normally calculated as perimeter * length.";
		(*rus_name) = "Общая площадь выдавленных поверхностей объекта (без учета областей торцевых крышек), обычно рассчитываемая как периметр * длина";
	}
	else if (quanity_prop == Renga::QuantityIds::CrossSectionOverallWidth)
	{
		(*eng_code) = "CrossSectionOverallWidth";
		(*rus_code) = "Ширина поперечного сечения";
		(*eng_name) = "The overall cross section (or profile) width of an object";
		(*rus_name) = "Общая ширина поперечного сечения (или профиля) объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::CrossSectionOverallHeight)
	{
		(*eng_code) = "CrossSectionOverallHeight";
		(*rus_code) = "Высота поперечного сечения";
		(*eng_name) = "The overall cross section (or profile) height of an object";
		(*rus_name) = "Общая высота поперечного сечения (или профиля) объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::NetCrossSectionArea)
	{
		(*eng_code) = "NetCrossSectionArea";
		(*rus_code) = "Площадь поперечного сечения";
		(*eng_name) = "Area of the cross section, excluding the interior space";
		(*rus_name) = "Общая площадь поперечного сечения (или профиля) объекта";
	}
	else if (quanity_prop == Renga::QuantityIds::GrossCrossSectionArea)
	{
		(*eng_code) = "GrossCrossSectionArea";
		(*rus_code) = "Площадь общего попереченого сечения";
		(*eng_name) = "Area of the cross section, including interior space";
		(*rus_name) = "Площадь поперечного сечения, включая внутреннее пространство";
	}
	else if (quanity_prop == Renga::QuantityIds::GrossSideArea)
	{
		(*eng_code) = "GrossSideArea";
		(*rus_code) = "Площадь вдоль центральной линии слоя";
		(*eng_name) = "The gross area along the center line of an object or a layer, excluding any processing features (cut-outs, etc.) or openings and recesses.";
		(*rus_name) = "Общая площадь вдоль центральной линии объекта или слоя, исключая любые элементы обработки (вырезы и т.д.) или отверстия и углубления.";
	}
	else if (quanity_prop == Renga::QuantityIds::GrossSideAreaLeft)
	{
		(*eng_code) = "GrossSideAreaLeft";
		(*rus_code) = "Площадь вдоль левой линии слоя";
		(*eng_name) = "The gross area along the left line of an object or a layer, excluding any processing features (cut-outs, etc.) or openings and recesses.";
		(*rus_name) = "Общая площадь вдоль левой линии объекта или слоя, исключая любые элементы обработки (вырезы и т.д.) или отверстия и углубления.";
	}
	else if (quanity_prop == Renga::QuantityIds::GrossSideAreaRight)
	{
		(*eng_code) = "GrossSideAreaRight";
		(*rus_code) = "Площадь вдоль правой линии слоя";
		(*eng_name) = "The gross area along the right line of an object or a layer, excluding any processing features (cut-outs, etc.) or openings and recesses.";
		(*rus_name) = "Общая площадь вдоль правой линии объекта или слоя, исключая любые элементы обработки (вырезы и т.д.) или отверстия и углубления.";
	}
	else if (quanity_prop == Renga::QuantityIds::Area)
	{
		(*eng_code) = "Area";
		(*rus_code) = "Площадь";
		(*eng_name) = "The area of an object. For windows and doors it equals the area of the cross section along the center line.";
		(*rus_name) = "Площадь объекта. Для окон и дверей он равен площади поперечного сечения вдоль центральной линии..";
	}
	else if (quanity_prop == Renga::QuantityIds::NetArea)
	{
		(*eng_code) = "NetArea";
		(*rus_code) = "Площадь верхней поверхности";
		(*eng_name) = "The total top surface area of an object or a layer, excluding any processing features (openings, cut-outs, etc.) or recesses.";
		(*rus_name) = "Общая площадь верхней поверхности объекта или слоя, исключая любые элементы обработки (отверстия, вырезы и т.д.) или углубления.";
	}


}