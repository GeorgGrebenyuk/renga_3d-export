#pragma once
#include "pch.h"

class object_3d_info {
public:
	object_3d_info(std::vector<const char*> material_names_data,
		std::vector<double> material_colors_data, std::vector<Renga::IGridPtr> geometry_data);
	std::vector<const char*> material_names;
	std::vector<double> material_colors;
	std::vector<Renga::IGridPtr> geometry;
	//properties
	/*
	* if export for Grid2Material -- they are empty
	*/
private:
	void get_material();
	void get_layered_material(int sub_object_position);
	void get_properties();
	void get_style();
};
/// <summary>
/// Class for getting info from model about objects
/// </summary>
class renga_data
{
public: 
	renga_data(Renga::IApplicationPtr application, int mode);
	std::map<int, object_3d_info> objects_3d_info;
};


