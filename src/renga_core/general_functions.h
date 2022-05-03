#pragma once
#include "Renga_import.h"
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include "string"
#include "renga_enums.h"
#include <math.h>

struct level_objects {
public:
	std::list<int> objects;
	int level_model_id;
	level_objects(int level_model_id_input, int obj)
	{
		this->objects.push_back(obj);
		this->level_model_id = level_model_id_input;
	}
	void add_item(int new_object)
	{
		this->objects.push_back(new_object);
	}
};
class grid_description {
public:
	std::vector<std::vector<std::vector<double>>> geometry;
	std::vector<double> color;
	double transparency;
	grid_description(std::vector<std::vector<std::vector<double>>>geom_input, std::vector<double> color_input, double transparency_input)
	{
		this->color = color_input;
		this->geometry = geom_input;
		this->transparency = transparency_input;
	}
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
		return l1_h <= l2_h;
	}
	static std::string get_type_str(GUID obj_type);
	static void get_quanity_prop(GUID quanity_prop, std::string* rus_name, std::string* eng_name, std::string* eng_code, std::string* rus_code);
	//static std::vector <double> get_transform_parameters(Renga::IProjectPtr link_project);
	static std::vector <double> get_transformed_coords(double x, double y, double z, std::vector<double>  current_params)
	{
		//std::vector<double> current_params = transform_parameters;
		double new_x = current_params.at(0) + x * cos(current_params.at(3)) - y * sin(current_params.at(3));
		double new_y = current_params.at(1) + x * sin(current_params.at(3)) + y * cos(current_params.at(3));
		double new_z = current_params.at(2) + z;
		return { new_x,new_y,new_z };
	}
	static void level2ids(Renga::IProjectPtr link_project, std::list<level_objects>* level2object,
		std::list<int>* non_level_objects, std::list<Renga::IModelObjectPtr>* m_levels);
	static void log_status_work(int all_objects, int current_object, std::string pre_name, bstr_t obj_type)
	{
		std::cout << "Progress for " << pre_name << " = " << current_object * 100.0 / all_objects << "% for " << obj_type << std::endl;
	}
	static void get_new_save_path(Renga::IProjectPtr project, int type, bool inc_guid, std::wstring* file_export_path)
	{
		std::wstring current_project_path(project->FilePath, SysStringLen(project->FilePath));
		std::string current_project_path_str(current_project_path.begin(), current_project_path.end());

		std::string extension = "";
		if (type == 0) extension = ".nwc";

		std::string uuid_and_ext = "";
		if (inc_guid)
		{
			GUID uniq_id;
			::CoCreateGuid(&uniq_id);
			std::string  uniq_id_str = std::to_string(uniq_id.Data1);
			uuid_and_ext = "_" + uniq_id_str + extension;
		}
		else uuid_and_ext = extension;
		std::string renga_ext = ".rnp";

		current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), uuid_and_ext);
		std::wstring new_path(current_project_path_str.begin(), current_project_path_str.end());
		*file_export_path = new_path;
	}

	//private:
		//static std::vector<double> transform_parameters;
};
class renga_data {
public:
	//renga_data(const char* project_filepath, int type_conversion);
	renga_data(Renga::IProjectPtr project, int type_input);
	std::wstring file_export_path;
	int type;
	//geometry
	void get_data();
	std::vector<std::vector<std::vector<grid_description>>>grids_collection_in_meshes;

	//level depend data
	
	std::list<level_objects> level2objects;
	//Containers for objects on level and as other group -- objects without level's reference
	std::list<int> non_level_objects;
	std::map<int, Renga::IModelObjectPtr> id2level;
	void sort_layers();
	//properties
	void get_material_grid(GUID object_type, int grid_type, Renga::IGridPtr link_grid, std::vector<double>* link_color, double* link_transparency);
	void get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color, double* link_transparency);
	//other info
	std::vector<double> projects_offset;
	Renga::IProjectPtr r_project;
private:
	void get_transform_parameters();
	
	Renga::IApplicationPtr r_app;

};
