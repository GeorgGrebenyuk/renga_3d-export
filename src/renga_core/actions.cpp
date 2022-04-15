#include "actions.h"
#include "renga_core.h"


void default_start(const char* path_to_project) {
	renga_data test_class(path_to_project);

	std::cout << test_class.r_project->FilePath << std::endl;

}
renga_data::renga_data(const char* project_filepath)
{
	::CoInitialize(nullptr);
	Renga::IApplicationPtr renga_current_app = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
	renga_current_app->PutVisible(VARIANT_TRUE);
	int result = renga_current_app->OpenProject(bstr_t(project_filepath));
	Renga::IProjectPtr pProject = renga_current_app->GetProject();

	this->r_project = pProject;
	this->r_app = renga_current_app;

	auto file_path = this->r_project->FilePath;
	//renga_current_app->Quit();
}
void renga_data::Quit() {

}
