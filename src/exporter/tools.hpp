#pragma once
#include "pch.h"
#include "sstream"
#include <cassert>
#pragma warning( disable : 4996)

static double getLevelElevation(Renga::IModelObjectPtr pModelObject)
{
	Renga::ILevelPtr pLevel;

	pModelObject->QueryInterface(&pLevel);
	assert(pLevel != nullptr);

	return pLevel->GetElevation();
}

static bool compareLevelElevations(Renga::IModelObjectPtr pLeftModelObject, Renga::IModelObjectPtr pRightModelObject)
{
	return getLevelElevation(pLeftModelObject) < getLevelElevation(pRightModelObject);
}
static const char* get_type_as_str(GUID obj_type)
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
	else if (obj_type == Renga::ObjectTypes::TextObject) out_type = "TextObject";
	else if (obj_type == Renga::ObjectTypes::Hatch) out_type = "Hatch";
	else if (obj_type == Renga::ObjectTypes::AngularDimension) out_type = "DimenAngularDimensionsion";
    else if (obj_type == Renga::ObjectTypes::DiametralDimension) out_type = "DiametralDimension";
    else if (obj_type == Renga::ObjectTypes::LinearDimension) out_type = "LinearDimension";
    else if (obj_type == Renga::ObjectTypes::RadialDimension) out_type = "RadialDimension";
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
static std::vector <double> get_transformed_coords(double x, double y, double z, std::vector<double>  current_params)
{
	//std::vector<double> current_params = transform_parameters;
	double new_x = current_params.at(0) + x * cos(current_params.at(3)) - y * sin(current_params.at(3));
	double new_y = current_params.at(1) + x * sin(current_params.at(3)) + y * cos(current_params.at(3));
	double new_z = current_params.at(2) + z;
	return { new_x,new_y,new_z };
}
static bool Transliterate(char* str, char* newStr)
{
    if (str)
    {
        for (; *str != 0; str++)
        {
            switch (str[0])
            {
            case 'à': strcat(&newStr[0], "a"); break;
            case 'á': strcat(&newStr[0], "b"); break;
            case 'â': strcat(&newStr[0], "v"); break;
            case 'ã': strcat(&newStr[0], "g"); break;
            case 'ä': strcat(&newStr[0], "d"); break;
            case 'å': strcat(&newStr[0], "e"); break;
            case '¸': strcat(&newStr[0], "ye"); break;
            case 'æ': strcat(&newStr[0], "zh"); break;
            case 'ç': strcat(&newStr[0], "z"); break;
            case 'è': strcat(&newStr[0], "i"); break;
            case 'é': strcat(&newStr[0], "y"); break;
            case 'ê': strcat(&newStr[0], "k"); break;
            case 'ë': strcat(&newStr[0], "l"); break;
            case 'ì': strcat(&newStr[0], "m"); break;
            case 'í': strcat(&newStr[0], "n"); break;
            case 'î': strcat(&newStr[0], "o"); break;
            case 'ï': strcat(&newStr[0], "p"); break;
            case 'ð': strcat(&newStr[0], "r"); break;
            case 'ñ': strcat(&newStr[0], "s"); break;
            case 'ò': strcat(&newStr[0], "t"); break;
            case 'ó': strcat(&newStr[0], "u"); break;
            case 'ô': strcat(&newStr[0], "f"); break;
            case 'õ': strcat(&newStr[0], "ch"); break;
            case 'ö': strcat(&newStr[0], "z"); break;
            case '÷': strcat(&newStr[0], "ch"); break;
            case 'ø': strcat(&newStr[0], "sh"); break;
            case 'ù': strcat(&newStr[0], "ch"); break;
            case 'ú': strcat(&newStr[0], "''"); break;
            case 'û': strcat(&newStr[0], "y"); break;
            case 'ü': strcat(&newStr[0], "''"); break;
            case 'ý': strcat(&newStr[0], "e"); break;
            case 'þ': strcat(&newStr[0], "yu"); break;
            case 'ÿ': strcat(&newStr[0], "ya"); break;
            case 'À': strcat(&newStr[0], "A"); break;
            case 'Á': strcat(&newStr[0], "B"); break;
            case 'Â': strcat(&newStr[0], "V"); break;
            case 'Ã': strcat(&newStr[0], "G"); break;
            case 'Ä': strcat(&newStr[0], "D"); break;
            case 'Å': strcat(&newStr[0], "E"); break;
            case '¨': strcat(&newStr[0], "Ye"); break;
            case 'Æ': strcat(&newStr[0], "Zh"); break;
            case 'Ç': strcat(&newStr[0], "Z"); break;
            case 'È': strcat(&newStr[0], "I"); break;
            case 'É': strcat(&newStr[0], "Y"); break;
            case 'Ê': strcat(&newStr[0], "K"); break;
            case 'Ë': strcat(&newStr[0], "L"); break;
            case 'Ì': strcat(&newStr[0], "M"); break;
            case 'Í': strcat(&newStr[0], "N"); break;
            case 'Î': strcat(&newStr[0], "O"); break;
            case 'Ï': strcat(&newStr[0], "P"); break;
            case 'Ð': strcat(&newStr[0], "R"); break;
            case 'Ñ': strcat(&newStr[0], "S"); break;
            case 'Ò': strcat(&newStr[0], "T"); break;
            case 'Ó': strcat(&newStr[0], "U"); break;
            case 'Ô': strcat(&newStr[0], "F"); break;
            case 'Õ': strcat(&newStr[0], "Ch"); break;
            case 'Ö': strcat(&newStr[0], "Z"); break;
            case '×': strcat(&newStr[0], "Ch"); break;
            case 'Ø': strcat(&newStr[0], "Sh"); break;
            case 'Ù': strcat(&newStr[0], "Ch"); break;
            case 'Ú': strcat(&newStr[0], "''"); break;
            case 'Û': strcat(&newStr[0], "Y"); break;
            case 'Ü': strcat(&newStr[0], "''"); break;
            case 'Ý': strcat(&newStr[0], "E"); break;
            case 'Þ': strcat(&newStr[0], "Yu"); break;
            case 'ß': strcat(&newStr[0], "Ya"); break;
            default: {
                char Temp[2] = { str[0], 0 };
                strcat(&newStr[0], &Temp[0]);
            }
            }
        }
        return true;
    }
    else return false;
    
}