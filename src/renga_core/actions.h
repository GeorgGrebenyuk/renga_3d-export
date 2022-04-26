#pragma once
#include <windows.h>
#include "string"
#include "Renga_import.h"
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include "renga_enums.h"



class renga_data {
public:
	renga_data(const char* project_filepath, int type_conversion);
	std::wstring file_export_path;

	int type;
private:
	Renga::IProjectPtr r_project;
	Renga::IApplicationPtr r_app;
	void navis_init();
	void navis_export();
	std::map<int, std::vector<unsigned short>> grid2color;
	std::vector<double> object_color (Renga::IModelObjectPtr obj);
};