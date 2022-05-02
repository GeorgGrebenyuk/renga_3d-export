#include "actions.h"
void tools::level2ids(Renga::IProjectPtr link_project,std::list<level_objects>* level2object, 
	std::list<int>* non_level_objects, std::list<Renga::IModelObjectPtr>* m_levels)
{
	Renga::IDataExporterPtr pDataExporter = link_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = link_project->GetModel()->GetObjects();

	for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++)
	{
		Renga::IExportedObject3DPtr internal_object = objects_collection3d->Get(counter_object);
		//int internal_object_id = internal_object->GetModelObjectId();
		Renga::IModelObjectPtr pModelObject = objects_collection->GetById(internal_object->GetModelObjectId());
		Renga::ILevelObjectPtr pLevelObject;
		pModelObject->QueryInterface(&pLevelObject);

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