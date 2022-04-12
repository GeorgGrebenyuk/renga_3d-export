#pragma once
#include <iostream>
#include <windows.h>
#include<string>
#include "nwcreate/LiNwcAll.h"
#include"Renga_import.h"
class actions
{
public:
	actions(_bstr_t project_file_path);
	int start();
	void test_process();
private:
	std::string current_project_file_path;
	Renga::IProjectPtr* renga_project;
};

