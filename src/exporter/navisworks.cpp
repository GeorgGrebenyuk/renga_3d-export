#include "pch.h"
#include "navisworks.hpp"
#include "nwcreate\LiNwcAll.h"
navisworks::navisworks(renga_data* data)
{
	this->project_data = data;
	LiNwcApiErrorInitialise();
	int status = 0;
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		this->start();
		break;
	case LI_NWC_API_NOT_LICENSED:
		printf("\n\nNot Licensed\n");
		status = 1;
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("\n\nInternal Error\n");
		status = 2;
	}
}
void navisworks::start() {

	LcNwcScene scene;
	const size_t cSize = strlen(this->project_data->project_path) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, this->project_data->project_path, cSize);
	LtWideString wfilename = wc;

	//Getting geometry, properties, materials ...
	for (auto level_info : this->project_data->levels_objects)
	{
		Renga::ILevelPtr level_object = level_info.first;
		LcNwcGroup levels_objects;
		levels_objects.SetName(level_object->LevelName);
		levels_objects.SetLayer(TRUE);

		for (auto type2objects : level_info.second)
		{
			const char* group_name = get_type_as_str(type2objects.first);
			bstr_t group_name_str = group_name;
			LcNwcGroup object_type_group;
			object_type_group.SetName(group_name_str);
			object_type_group.SetLayer(TRUE);

			for (int model_object_id : type2objects.second)
			{
				object_3d_info info = this->project_data->objects_3d_info[model_object_id];
			}

		}
	}
	for (level_objects one_group : this->data->level2objects)
	{
		Renga::IModelObjectPtr level_object = this->data->id2level[one_group.level_model_id];
		Renga::ILevelPtr level_instanse;
		level_object->QueryInterface(&level_instanse);




		for (int obj : one_group.objects)
		{
			Renga::IModelObjectPtr obj_model = objects_collection->GetById(objects_collection3d->Get(obj)->GetModelObjectId());
			tools::log_status_work(all_objects_3d, counter_objects, "writing", obj_model->GetName());
			navis_object::navis_object(this->data->r_project, &this->data->grids_collection_in_meshes.at(obj), &levels_objects, obj_model);
			counter_objects++;
		}
		scene.AddNode(levels_objects);
	}

	LcNwcGroup non_levels_objects;
	non_levels_objects.SetName(L"Models objects");
	non_levels_objects.SetLayer(TRUE);
	for (int obj : this->data->non_level_objects)
	{
		Renga::IModelObjectPtr obj_model = objects_collection->GetById(objects_collection3d->Get(obj)->GetModelObjectId());
		tools::log_status_work(all_objects_3d, counter_objects, "writing", obj_model->GetName());
		navis_object::navis_object(this->data->r_project, &this->data->grids_collection_in_meshes.at(obj), &non_levels_objects, obj_model);
		counter_objects++;
	}
	scene.AddNode(non_levels_objects);

}
