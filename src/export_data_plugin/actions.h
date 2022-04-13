#pragma once
#include "Renga_import.h"
#include <iostream>
#include <windows.h>
#include <string>
#include "nwcreate/LiNwcAll.h"
class actions
{
public:
	actions(int type);
	int start();
	void test_process();
	void test2();
private:
	std::string current_project_file_path;
	Renga::IProjectPtr renga_project;
	Renga::IApplicationPtr app;
};

