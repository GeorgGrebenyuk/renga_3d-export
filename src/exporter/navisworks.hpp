#pragma once
#include "renga_data.hpp"
#pragma comment(linker, "/manifestdependency:\"name='nwcreate' version='1.0.0.0' type='win32'\"")
class navisworks
{
public: 
	navisworks(renga_data* data);
private:
	void start();
	renga_data* project_data;
};

