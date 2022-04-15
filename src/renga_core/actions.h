#pragma once
#include <windows.h>
#include "string"
#include "Renga_import.h"
#include <iostream>
#include <list>
#include <memory>

class renga_data {
public:
	Renga::IProject *r_project;
	Renga::IApplication* r_app;

	renga_data(const char* project_filepath);
	std::string file_path;
	void Quit();
};