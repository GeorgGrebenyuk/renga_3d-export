#pragma once
#import <RengaCOMAPI.tlb>
#include "Renga/PluginsSystemGlobal.h"
#include "Renga/IPlugin.h"
#include "Renga/EntityTypes.h"
#include "Renga/WiringAccessoryCategoryIds.h"
#include "Renga/WindowCategoryIds.h"
#include "Renga/SystemTypeIds.h"
#include "Renga/StyleTypeIds.h"
#include "Renga/PlumbingFixtureCategoryIds.h"
#include "Renga/PipeFittingCategoryIds.h"
#include "Renga/PipeAccessoryCategoryIds.h"
#include "Renga/ObjectTypes.h"
#include "Renga/MechanicalEquipmentCategoryIds.h"
#include "Renga/LightFixtureCategoryIds.h"
#include "Renga/EquipmentCategoryIds.h"
#include "Renga/ElectricDistributionBoardCategoryIds.h"
#include "Renga/DuctFittingCategoryIds.h"
#include "Renga/DuctAccessoryCategoryIds.h"
#include "Renga/DoorCategoryIds.h"
#include "Renga/QuantityIds.h"
#include "Renga/ProfileParameterIds.h"
#include "Renga/ParameterIds.h"
#include "Renga/SelectionEventHandler.hpp"
#include "Renga/MeshTypes.h"
#include "Renga/GridTypes.h"
#include "Renga/EventHandler.hpp"
#include "Renga/CreateApplication.hpp"
#include "Renga/ApplicationEventHandler.hpp"
#include "Renga/ActionEventHandler.hpp"


#include "nwcreate/LiNwcAll.h"
#include <iostream>
#include <windows.h>
#include <list>

class renga2nwc : public plugins::IPlugin {
public:
	bool initialize(const wchar_t* pluginPath);
	void stop();
	int start();
	void GettingGeometry();
private:
	Renga::IApplicationPtr r_app;
	Renga::IProjectPtr r_project;

};
class ButtonBaseHandler : public Renga::ActionEventHandler
{
public:
	ButtonBaseHandler(Renga::IActionPtr pAction, ButtonBase* pButtonBase);

	virtual void OnTriggered() override;
	void OnToggled(bool checked) override {}

private:
	ButtonBase* m_pButtonBase;
};
class ButtonBase
{
public:
	ButtonBase(Renga::IUIPtr pUI, const std::wstring& toolTip);

	Renga::IActionPtr getAction();

	virtual void OnTriggered() = 0;

protected:
	std::unique_ptr<ButtonBaseHandler> m_pHandler;
	Renga::IActionPtr m_pAction;
};
EXPORT_PLUGIN(renga2nwc)