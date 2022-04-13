/*  Sample: ComplexToolButtons
 *
 *  This sample illustrates how to add custom complex tool buttons
 *  to Renga's main toolbar.
 *
 *  Copyright Renga Software LLC, 2018. All rights reserved.
 */

#pragma once
#import <RengaCOMAPI.tlb>
//#include "Renga_import.h"

#include <Renga/IPlugin.h>
#include <Renga/ActionEventHandler.hpp>


#include <memory>
#include <list>
#include <string>


class export_data_plugin : public plugins::IPlugin
{
public:
	export_data_plugin();
	~export_data_plugin();

	bool initialize(const wchar_t* pluginPath) override;
	void stop() override;

private:
	void addHandler(Renga::ActionEventHandler* pHandler);
	Renga::IImagePtr CreateIcon(Renga::IUIPtr pUI, std::wstring local_image_path);
	Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, 
		const std::wstring& displayName, const std::wstring& icon_local_path, int type);
	typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;
	std::wstring plugin_path;
	std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(export_data_plugin);
