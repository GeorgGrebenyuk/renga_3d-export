#include "plugin_start.h"
#include "actions.h"

class ButtonPress : public Renga::ActionEventHandler {
public:
	ButtonPress(Renga::IActionPtr pAction, const std::wstring& text) :
		Renga::ActionEventHandler(pAction),
		m_text(text)
	{
	}
	void OnTriggered() override
	{
		//::renga2nwc::user_selection();
		::MessageBox(nullptr, (LPCWSTR)m_text.c_str(), (LPCWSTR)L"Plugin message", MB_ICONINFORMATION | MB_OK);
	}

	void OnToggled(bool checked) override {}

private:
	std::wstring m_text;
};
renga2nwc::renga2nwc()
{
	::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

renga2nwc::~renga2nwc()
{
	::CoUninitialize();
}

void renga2nwc::addHandler(Renga::ActionEventHandler* pHandler)
{
	m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}
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
		if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
		{
			Renga::IImagePtr pImage = pUI->CreateImage();
			pImage->LoadFromFile(full_path.c_str());

			Renga::IActionPtr pAction = pUI->CreateAction();
			pAction->PutDisplayName(L"nwc_export");
			pAction->PutToolTip(L"Click to start export procedure");
			pAction->PutIcon(pImage);
			addHandler(new ButtonPress(pAction, L"nwc_export"));
			pUIPanelExtension->AddToolButton(pAction);

			pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
			pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
		}
	}
	return true;
}
//smth action to identify User's selection 
void renga2nwc::user_selection() {
	//actions::actions();
	
}

void renga2nwc::stop (){
	m_handlerContainer.clear();
	r_app = nullptr;
	r_project = nullptr;
}

