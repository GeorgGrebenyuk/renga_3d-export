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

	//tools::renga_project = this->r_project;
	//Get file path and set new file name with target extension
	std::wstring current_project_path(this->r_project->FilePath, SysStringLen(this->r_project->FilePath));
	std::string current_project_path_str(current_project_path.begin(), current_project_path.end());

	GUID uniq_id;
	CoCreateGuid(&uniq_id);
	std::string  uniq_id_str = std::to_string(uniq_id.Data1);
	std::string extension = "";
	if (this->type == 0) extension = ".nwc";
	std::string uuid_and_ext = uniq_id_str + "_" + extension;
	std::string renga_ext = ".rnp";

	current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), uuid_and_ext);
	std::wstring new_path(current_project_path_str.begin(), current_project_path_str.end());
	this->file_export_path = new_path;

	switch (this->type) {

	case 0:
		this->navis_init();
		break;
	}
}
