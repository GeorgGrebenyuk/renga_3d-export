#include "console_start.h"
#include "general_functions.h"

#include "navis.h"

void default_start(const char* path_to_project, int type_conversion)
{
	::CoInitialize(nullptr);
	Renga::IApplicationPtr r_app = Renga::CreateApplication(CLSCTX_LOCAL_SERVER);
	r_app->PutVisible(VARIANT_TRUE);
	r_app->OpenProject(bstr_t(path_to_project));
	Renga::IProjectPtr r_project = r_app->GetProject();

	renga_data model_data(r_project, type_conversion);
	switch (type_conversion) {
	case 0:
		navis::navis(&model_data);
		break;
	}
	r_app->Quit();
}