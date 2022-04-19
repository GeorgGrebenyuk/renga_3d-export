#pragma once
#include "nwcreate/LiNwcAll.h"
#include "actions.h"
#include <list>
#include "string"
class navis
{
public:
	navis(std::vector<renga_object> objects, std::wstring path_to_save);
private:
	std::vector<renga_object> renga_objects;
	std::wstring path_to_save;
	void start_export();

};

