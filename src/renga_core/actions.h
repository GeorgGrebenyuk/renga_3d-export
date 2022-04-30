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
#include <math.h>

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
struct level_objects {
public:
	std::list<int> ids;
	Renga::ILevelPtr level;
};
class renga_data {
public:
	renga_data(const char* project_filepath, int type_conversion);
	std::wstring file_export_path;
	int type;
private:
	void get_transform_parameters();
	Renga::IProjectPtr r_project;
	Renga::IApplicationPtr r_app;
	void navis_init();
	void navis_export();
	std::vector<double> projects_offset;
	
};
class tools {
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
	//static std::vector <double> get_transform_parameters(Renga::IProjectPtr link_project);
	static std::vector <double> get_transformed_coords(double x, double y, double z, std::vector<double>  current_params)
	{
		//std::vector<double> current_params = transform_parameters;
		double new_x = current_params.at(0) + x * cos(current_params.at(3)) - y * sin(current_params.at(3));
		double new_y = current_params.at(1) + x * sin(current_params.at(3)) + y * cos(current_params.at(3));
		double new_z = current_params.at(2) + z;
		return { new_x,new_y,new_z };
	}
	static std::list<level_objects> level2ids (Renga::IProjectPtr link_project); 
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
//private:
	//static std::vector<double> transform_parameters;
};
class navis_object {
public:
	navis_object(Renga::IProjectPtr project_input, std::vector<double> *offset_parameters, LcNwcGroup* parent_element, int object_id_in_exporting);
private:
	int object_id_in_exporting;
	std::vector<double> internal_offset_parameters;
	Renga::IProjectPtr project;
	Renga::IModelObjectPtr current_model_object;
	Renga::IExportedObject3DPtr current_model_object_geometry;
	void create_geometry(LcNwcGroup* object_defenition);
	void getting_properties(LcNwcGroup* object_defenition);
	void getting_color(LcNwcGroup* object_defenition);
};
