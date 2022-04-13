#include "plugin_start.h"
#include <Renga/CreateApplication.hpp>

class TextHandler : public Renga::ActionEventHandler
{
public:
    TextHandler(Renga::IActionPtr pAction, std::wstring& buttom_link) :
        Renga::ActionEventHandler(pAction),
        type_start(buttom_link)
    {
    }

    void OnTriggered() override
    {
        ::MessageBox(nullptr, (LPCWSTR)type_start.c_str(), (LPCWSTR)L"Plugin message", MB_ICONINFORMATION | MB_OK);
        //switch (this->type_start) {
        //case 0:
        //    actions(actions(Renga::CreateApplication()));
        //    break;
        //}
        //actions(actions(type_start));
    }

    void OnToggled(bool checked) override {}

private:
    std::wstring type_start;
};

export_data_plugin::export_data_plugin()
{
    ::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
}

export_data_plugin::~export_data_plugin()
{
    ::CoUninitialize();
}
/// <summary>
/// Internal method to create IImagePtr by local file path
/// </summary>
Renga::IImagePtr export_data_plugin::CreateIcon(Renga::IUIPtr pUI, std::wstring local_image_path) {
    std::wstring image_absolute_path = this->plugin_path + local_image_path;
    Renga::IImagePtr pImage = pUI->CreateImage();
    pImage->LoadFromFile(image_absolute_path.c_str());
    return pImage;
}

/// <summary>
/// Internal method to create buttom and link it with User interface
/// </summary>
Renga::IActionPtr export_data_plugin::CreateAction(Renga::IUIPtr pUI, 
    const std::wstring& displayName, const std::wstring& icon_local_path) //, int type
{
    Renga::IActionPtr pAction = pUI->CreateAction();
    pAction->PutIcon(CreateIcon(pUI, icon_local_path));
    pAction->PutDisplayName(displayName.c_str());

    std::wstring operation_type(L"navis");
    addHandler(new TextHandler(pAction, operation_type));

    return pAction;
}

void export_data_plugin::addHandler(Renga::ActionEventHandler* pHandler)
{
    m_handlerContainer.emplace_back(HandlerPtr(pHandler));
}

bool export_data_plugin::initialize(const wchar_t* pluginPath)
{
    std::wstring string_path(pluginPath);
    this->plugin_path = string_path;

    auto pApplication = Renga::CreateApplication();
    if (!pApplication)
        return false;
    //r_app = pApplication;
    //if (pApplication->Project->FilePath.length() < 2)
    //    return false;

    if (auto pUI = pApplication->GetUI())
    {
        if (auto pUIPanelExtension = pUI->CreateUIPanelExtension())
        {          
            if (auto pDropDownButton = pUI->CreateDropDownButton())
            {
                std::string main_button_name = "Select export data formats";
                pDropDownButton->PutToolTip(main_button_name.c_str());
                pDropDownButton->PutIcon(CreateIcon(pUI, L"\\export_logo.png"));

                pDropDownButton->AddAction(
                    CreateAction(pUI, L"NWC export", L"\\navis_logo.png"));

                pUIPanelExtension->AddDropDownButton(pDropDownButton);
            }
            pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);
            pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);
        }
    }
    return true;
}

void export_data_plugin::stop()
{
    m_handlerContainer.clear();
}
