#pragma once
#include "general_functions.h"
#include "nwcreate/LiNwcAll.h"

class navis {
public:
	navis(renga_data* data);
	void navis_export();
private:
	renga_data* data;
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