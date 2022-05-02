#include "actions.h"
#include "renga_core.h"
#include <regex>


void default_start(const char* path_to_project, int type_conversion) {
	renga_data test_class(path_to_project, type_conversion);

	//std::cout << test_class.r_project->FilePath << std::endl;

}
renga_data::renga_data(const char* project_filepath, int type_conversion)
{
	this->type = type_conversion;
	::CoInitialize(nullptr);
	this->r_app = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
	this->r_app->PutVisible(VARIANT_TRUE);
	this->r_app->OpenProject(bstr_t(project_filepath));
	this->r_project = this->r_app->GetProject();

	this->get_transform_parameters();
	//this->projects_offset = tools::get_transform_parameters(this->r_project);

	//tools::renga_project = this->r_project;
	//Get file path and set new file name with target extension
	std::wstring current_project_path(this->r_project->FilePath, SysStringLen(this->r_project->FilePath));
	std::string current_project_path_str(current_project_path.begin(), current_project_path.end());

	GUID uniq_id;
	CoCreateGuid(&uniq_id);
	std::string  uniq_id_str = std::to_string(uniq_id.Data1);
	std::string extension = "";
	if (this->type == 0) extension = ".nwc";
	std::string uuid_and_ext ="_" + uniq_id_str + extension;
	std::string renga_ext = ".rnp";

	current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), uuid_and_ext);
	std::wstring new_path(current_project_path_str.begin(), current_project_path_str.end());
	this->file_export_path = new_path;

	switch (this->type) {

	case 0:
		this->navis_init();
		break;
	}
	this->r_app->Quit();
}
void renga_data::get_transform_parameters()
{
	Renga::IBuildingInfoPtr building_object = this->r_project->GetBuildingInfo();
	
	Renga::IPropertyContainerPtr props_usual = building_object->GetProperties();
	
	bstr_t offset_x = "IfcLocationX";
	bool is_x_correct = false;
	double offset_x_value = 0.0;
	bstr_t offset_y = "IfcLocationY";
	bool is_y_correct = false;
	double offset_y_value = 0.0;
	bstr_t offset_z = "IfcLocationZ";
	bool is_z_correct = false;
	double offset_z_value = 0.0;
	bstr_t angle = "IfcDirectionPrecession";
	bool angle_correct = false;
	double angle_value = 0.0;

	//Renga::IGuidCollectionPtr props_ids = props_usual->GetIds();

	/*for (int counter_property = 0; counter_property < props_ids->GetCount(); counter_property++)
	{
		GUID id = props_ids->Get(counter_property);
		Renga::IPropertyPtr one_prop = props_usual->Get(id);
		if (one_prop->Name == offset_x && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_x_value = one_prop->GetDoubleValue();
			is_x_correct = true;
		}
		else if (one_prop->Name == offset_y && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_y_value = one_prop->GetDoubleValue();
			is_y_correct = true;
		}
		else if (one_prop->Name == offset_z && one_prop->GetType() == Renga::PropertyType::PropertyType_Double) {
			offset_z_value = one_prop->GetDoubleValue();
			is_z_correct = true;
		}
		else if (one_prop->Name == angle && one_prop->GetType() == Renga::PropertyType::PropertyType_Angle) {
			angle_value = one_prop->GetAngleValue(Renga::AngleUnit::AngleUnit_Degrees);
			angle_correct = true;
		}
	}*/
	
	std::vector<double> find_transform_parameters{ offset_x_value,offset_y_value,offset_z_value,angle_value };
	this->projects_offset = find_transform_parameters;
}