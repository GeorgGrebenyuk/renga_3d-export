#include "plugin_start.h"
#include "actions.h"

bool renga2nwc::initialize(const wchar_t* pluginPath) {
	//Convert path to image as string to wchar_t
	std::string image_local_path = "\\navis_logo.png";
	std::wstring image_path_w(std::begin(image_local_path), std::end(image_local_path));
	std::wstring string_path (pluginPath);
	std::wstring full_path = string_path + image_path_w;
	//Init application
	auto pApplication = Renga::CreateApplication();
	if (!pApplication)
		return false;
	r_app = pApplication;
	r_project = r_app->Project;
	auto project_path = r_project->FilePath;
	if (project_path.length() < 2)
		return false;

	if (auto pUI = pApplication->GetUI())
	{
		Renga::IImagePtr pImage = pUI->CreateImage();
		pImage->LoadFromFile(full_path.c_str());

		Renga::IActionPtr pAction = pUI->CreateAction();
		pAction->PutDisplayName(L"Navisworks_logo");
		pAction->PutToolTip(L"Click to start export procedure");
		pAction->PutIcon(pImage);
		
		//Doing smth to getting out button on tool-space
		Renga::IUIPanelExtensionPtr pUIPanelExtension = pUI->CreateUIPanelExtension();
		pUIPanelExtension->AddToolButton(pAction);
		pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
		pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
	}
}

//smth action to identify User's selection 
void renga2nwc::user_selection() {
	actions::actions(this->r_project->FilePath);
}

void renga2nwc::stop (){
	r_app = nullptr;
	r_project = nullptr;
}

