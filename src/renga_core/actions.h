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
class renga_data {
public:
	renga_data(const char* project_filepath, int type_conversion);
	std::wstring file_export_path;
	int type;
	//geometry
	void get_data();
	std::vector<std::vector<std::vector<grid_description>>>grids_collection_in_meshes;

	//level depend data
	void sort_layers();
	std::list<level_objects> level2objects;
	//Containers for objects on level and as other group -- objects without level's reference
	std::list<int> non_level_objects;
	std::map<int, Renga::IModelObjectPtr> id2level;

	//properties
	void get_material_grid(GUID object_type, int grid_type, Renga::IGridPtr link_grid, std::vector<double>* link_color, double* link_transparency);
	void get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color, double* link_transparency);
	//other info
	std::vector<double> projects_offset;
private:
	void get_transform_parameters();
	Renga::IProjectPtr r_project;
	Renga::IApplicationPtr r_app;
	void navis_init();
	void navis_export();
	
	
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
	static void level2ids (Renga::IProjectPtr link_project, std::list<level_objects>* level2object,
		std::list<int> *non_level_objects, std::list<Renga::IModelObjectPtr>* m_levels);
	static void log_status_work(int all_objects, int current_object, std::string pre_name, bstr_t obj_type) 
	{
		std::cout << "Progress for " << pre_name << " = " << current_object * 100.0 / all_objects << "% for " << obj_type << std::endl;
	}
	
//private:
	//static std::vector<double> transform_parameters;
};
class navis_object {
public:
	navis_object(Renga::IProjectPtr project_input, std::vector<std::vector<grid_description>>* object_info, LcNwcGroup* parent_element, Renga::IModelObjectPtr obj);
private:
	//int object_id_in_exporting;
	std::vector<double> internal_offset_parameters;
	Renga::IProjectPtr project;
	Renga::IModelObjectPtr current_model_object;
	Renga::IExportedObject3DPtr current_model_object_geometry;
	void getting_properties(LcNwcGroup* object_defenition);
};
