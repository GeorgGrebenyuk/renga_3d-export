#pragma once
#include<string>
#include <iostream>
#include <windows.h>
#include "Renga_import.h"

class renga2nwc : public plugins::IPlugin {
public:
	bool initialize(const wchar_t* pluginPath);
	void stop();
	Renga::IApplicationPtr r_app;
	Renga::IProjectPtr r_project;
};

EXPORT_PLUGIN(renga2nwc)