#include "pch.h"
static bool compare_levels(Renga::IModelObjectPtr l1, Renga::IModelObjectPtr l2) {

	Renga::ILevelPtr pLevel1;
	l1->QueryInterface(&pLevel1);
	Renga::ILevelPtr pLevel2;
	l2->QueryInterface(&pLevel2);

	assert(pLevel1 != nullptr);
	assert(pLevel2 != nullptr);
	double l1_h = pLevel1->GetElevation();
	double l2_h = pLevel2->GetElevation();
	return l1_h <= l2_h;
}
static void sort_objects(std::vector<Renga::IModelObjectPtr>*level_objects, std::map<GUID, std::vector<int>>* type2objects) 
{
	for (Renga::IModelObjectPtr one_object : *level_objects)
	{
		GUID object_type = one_object->GetObjectType();
		if ((*type2objects).empty())  (*type2objects).insert(std::pair< GUID, std::vector<int>>{object_type, { one_object->Id }});
		else
		{
			bool is_find_that = false;
			for (auto data : (*type2objects))
			{
				if (data.first == object_type)
				{
					data.second.push_back(one_object->Id);
					is_find_that = true;
					break;
				}
			}
			if (!is_find_that) (*type2objects).insert(std::pair< GUID, std::vector<int>>{object_type, { one_object->Id }});
		}
	}
}