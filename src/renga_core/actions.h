#pragma once
#include <windows.h>
#include "string"
#include "Renga_import.h"
#include <iostream>
#include <vector>
#include <list>

class renga_data {
public:
	Renga::IProjectPtr r_project;
	Renga::IApplicationPtr r_app;

	renga_data(const char* project_filepath, int type_conversion);
	std::wstring file_export_path;
	//std::vector<renga_object> objects;
	int type;
};
class renga_object {
public:
	GUID obj_type;
	//geometry
	std::list<Renga::FloatPoint3D> points_list; //all points at object
	std::list<std::list<unsigned int>> triangles_definition;//all triangles at object
	std::list<unsigned int> grid_definition; //indexes in triangles_definition
	std::list<unsigned int> mesh_definition; //indexes in grid_definition
	std::list<unsigned int> meshes_definition; //meshes at object
	//properties ...
	//material ...
	// 
	//class constructor
	renga_object(Renga::IExportedObject3DPtr internal_object);
};