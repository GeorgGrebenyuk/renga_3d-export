#pragma once
#include "renga_data.hpp"
#include "general_data.hpp"
//#pragma comment(linker, "/manifestdependency:\"name='nwcreate' version='1.0.0.0' type='win32'\"")
#include "nwcreate\LiNwcAll.h"
class navisworks
{
public: 
	navisworks(renga_data* data);
private:
	void parse_level_objects(std::map<const char*, std::vector<int>> data, LcNwcGroup* to_record);
	void start();
	renga_data* project_data;
};

