#pragma once
#include "pch.h"
#include <cmath>
static bool compare_levels(Renga::IModelObjectPtr l1, Renga::IModelObjectPtr l2) {

	Renga::ILevelPtr pLevel1;
	l1->QueryInterface(&pLevel1);
	Renga::ILevelPtr pLevel2;
	l2->QueryInterface(&pLevel2);

	assert(pLevel1 != nullptr);
	assert(pLevel2 != nullptr);
	double l1_h = round(pLevel1->GetElevation() * 1000) / 1000;
	double l2_h = round(pLevel2->GetElevation() * 1000) / 1000;
	bstr_t l1_name = pLevel1->LevelName;
	bstr_t l2_name = pLevel2->LevelName;

	if (l1_h == l2_h) return true;
	else return l1_h <= l2_h;
}
const char* get_type_as_str(GUID obj_type)
{
	const char* out_type;
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
static void sort_objects(std::vector<Renga::IModelObjectPtr>*level_objects, std::map<const char*, std::vector<int>>* type2objects)
{
	std::map<const char*, std::vector<int>>::iterator check_objects;
	for (Renga::IModelObjectPtr one_object : *level_objects)
	{
		//GUID object_type = one_object->GetObjectType();
		const char* object_type = get_type_as_str(one_object->GetObjectType());
		if ((*type2objects).empty())  (*type2objects).insert(std::pair< const char*, std::vector<int>>{object_type, { one_object->Id }});
		else
		{
			check_objects = (*type2objects).find(object_type);
			if (check_objects != (*type2objects).end())
			{
				check_objects->second.push_back(one_object->Id);
			}
			else (*type2objects).insert(std::pair< const char*, std::vector<int>>{object_type, { one_object->Id }});
		}
	}
}
