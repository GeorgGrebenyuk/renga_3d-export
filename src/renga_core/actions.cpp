#include "actions.h"
#include "renga_core.h"
#include <regex>

#include"navis.h"


void default_start(const char* path_to_project, int type_conversion) {
	renga_data test_class(path_to_project, type_conversion);

	//std::cout << test_class.r_project->FilePath << std::endl;

}
renga_data::renga_data(const char* project_filepath, int type_conversion)
{
	this->type = type_conversion;
	::CoInitialize(nullptr);
	Renga::IApplicationPtr renga_current_app = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
	renga_current_app->PutVisible(VARIANT_TRUE);
	int result = renga_current_app->OpenProject(bstr_t(project_filepath));
	Renga::IProjectPtr pProject = renga_current_app->GetProject();

	this->r_project = pProject;
	this->r_app = renga_current_app;

	//Get file path and set new file name with target extension
	std::wstring current_project_path(pProject->FilePath, SysStringLen(pProject->FilePath));
	std::string current_project_path_str(current_project_path.begin(), current_project_path.end());

	GUID uniq_id;
	CoCreateGuid(&uniq_id);
	std::string  uniq_id_str = std::to_string(uniq_id.Data1);
	std::string extension = "";
	if (this->type == 0) extension = ".nwc";
	std::string uuid_and_ext = uniq_id_str + extension;
	std::string renga_ext = ".rnp";

	current_project_path_str.replace(current_project_path_str.find(renga_ext), renga_ext.length(), uuid_and_ext);
	std::wstring new_path(current_project_path_str.begin(), current_project_path_str.end());
	this->file_export_path = new_path;

	//Getting geometry, properties, materials ...
	Renga::IDataExporterPtr pDataExporter = pProject->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	
	//Renga::IGridWithMaterialCollectionPtr objects_grids = pDataExporter->GetGrids();

	std::vector<renga_object> objects;
	for (int couter_object = 0; couter_object < objects_collection3d->GetCount(); couter_object++)
	{
		Renga::IExportedObject3DPtr pObject = objects_collection3d->Get(couter_object);
		objects.push_back(renga_object(pObject));
	}
	switch (this->type) {

	case 0:
		navis start_conv(objects, this->file_export_path);
		break;
	}
}
