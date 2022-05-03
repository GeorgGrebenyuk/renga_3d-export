//#pragma once
#include <list>
#include <memory>
#include "Renga_import.h"
#include <string>


class export_data_plugin : public plugins::IPlugin
{
public:
	export_data_plugin();
	~export_data_plugin();

	bool initialize(const wchar_t* pluginPath);
	void stop();

private:
	void addHandler(Renga::ActionEventHandler* pHandler);
	Renga::IImagePtr CreateIcon(Renga::IUIPtr pUI, std::wstring local_image_path);
	Renga::IActionPtr CreateAction(Renga::IUIPtr pUI, 
		const std::wstring& displayName, const std::wstring& icon_local_path, 
		int type, Renga::IProjectPtr project);
	typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;
	std::wstring plugin_path;
	std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(export_data_plugin);
