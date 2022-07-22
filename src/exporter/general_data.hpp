//variables
#include "Renga/ObjectTypes.h"
static int max_triangles = 0;
static bool use_recalc = false;
static bool use_hidded = true;

const std::list<GUID> c_levelTreeTypes =
{
  Renga::ObjectTypes::Wall,
  Renga::ObjectTypes::Column,
  Renga::ObjectTypes::Floor,
  Renga::ObjectTypes::Opening,
  Renga::ObjectTypes::Roof,
  Renga::ObjectTypes::Beam,
  Renga::ObjectTypes::Stair,
  Renga::ObjectTypes::Ramp,
  Renga::ObjectTypes::Window,
  Renga::ObjectTypes::Door,
  Renga::ObjectTypes::Railing,
  Renga::ObjectTypes::Room,
  Renga::ObjectTypes::IsolatedFoundation,
  Renga::ObjectTypes::WallFoundation,
  Renga::ObjectTypes::AssemblyInstance,
  Renga::ObjectTypes::Element,
  Renga::ObjectTypes::Plate,
  Renga::ObjectTypes::RoutePoint,
  Renga::ObjectTypes::Equipment,
  Renga::ObjectTypes::PlumbingFixture,
  Renga::ObjectTypes::MechanicalEquipment,
  Renga::ObjectTypes::Line3D,
  Renga::ObjectTypes::Hatch,
  Renga::ObjectTypes::TextShape,
  Renga::ObjectTypes::Rebar
};

const std::list<GUID> c_nonLevelTreeTypes
{
  Renga::ObjectTypes::PipeAccessory,
  Renga::ObjectTypes::PipeFitting,
  Renga::ObjectTypes::Pipe,
  Renga::ObjectTypes::Duct,
  Renga::ObjectTypes::DuctAccessory,
  Renga::ObjectTypes::DuctFitting,
  Renga::ObjectTypes::LineElectricalCircuit,
  Renga::ObjectTypes::LightFixture,
  Renga::ObjectTypes::ElectricDistributionBoard,
  Renga::ObjectTypes::WiringAccessory,
  Renga::ObjectTypes::Route,
  Renga::ObjectTypes::Axis,
  Renga::ObjectTypes::Elevation,
  Renga::ObjectTypes::Section,
  Renga::ObjectTypes::Dimension,
};

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