#pragma once
#include "Renga_import.h"
#include<string>
#include <iostream>
#include <windows.h>
#include <list>
#include <memory>


class renga2nwc : public plugins::IPlugin {
public:
	renga2nwc();
	~renga2nwc();
	bool initialize(const wchar_t* pluginPath)  override;
	void stop()  override;
	void user_selection(); //operation to select button and start other procedures
	Renga::IApplicationPtr r_app;
	Renga::IProjectPtr r_project;
private:
	void addHandler(Renga::ActionEventHandler* pHandler);
private:
	typedef std::unique_ptr<Renga::ActionEventHandler> HandlerPtr;
private:
	std::list<HandlerPtr> m_handlerContainer;
};

EXPORT_PLUGIN(renga2nwc)