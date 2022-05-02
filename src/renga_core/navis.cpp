#include "actions.h"

#define LI_NWC_NO_PROGRESS_CALLBACKS NULL
#define LI_NWC_NO_USER_DATA NULL

void renga_data::navis_init() {
	//this->path_to_save = path_to_save;
	// Then initialise the rest of the API.

	// Initialise low-level API first.
	LiNwcApiErrorInitialise();
	int status = 0;
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		this->navis_export();
		break;
	case LI_NWC_API_NOT_LICENSED:
		printf("\n\nNot Licensed\n");
		status = 1;
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("\n\nInternal Error\n");
		status = 2;

	}

	std::cout << "\n\nnwcreate lib status = " << status << std::endl;

}
void renga_data::navis_export() {
	LcNwcScene scene;

	LtWideString wfilename = this->file_export_path.c_str();
	//Getting geometry, properties, materials ...
	Renga::IDataExporterPtr pDataExporter = this->r_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = this->r_project->GetModel()->GetObjects();
	
	int all_objects_3d = objects_collection3d->GetCount();
	std::cout << "\n\nStart export to NWC\n\n"<< std::endl;

	
	//std::cout << "end sort objects on levels" << std::endl;
	int counter_objects = 0;
	for (level_objects one_group : this->level2objects)
	{
		Renga::IModelObjectPtr level_object = this->id2level[one_group.level_model_id];
		Renga::ILevelPtr level_instanse;
		level_object->QueryInterface(&level_instanse);

		LcNwcGroup levels_objects;
		levels_objects.SetName(level_instanse->LevelName);
		levels_objects.SetLayer(TRUE);

		
		for (int obj : one_group.objects)
		{
			Renga::IModelObjectPtr obj_model = objects_collection->GetById(objects_collection3d->Get(obj)->GetModelObjectId());
			tools::log_status_work(all_objects_3d, counter_objects, "writing", obj_model->GetName());
			navis_object::navis_object(this->r_project, &this->grids_collection_in_meshes.at(obj), &levels_objects, obj_model);
			counter_objects++;
		}
		scene.AddNode(levels_objects);
	}

	LcNwcGroup non_levels_objects;
	non_levels_objects.SetName(L"Models objects");
	non_levels_objects.SetLayer(TRUE);
	for (int obj : this->non_level_objects)
	{
		Renga::IModelObjectPtr obj_model = objects_collection->GetById(objects_collection3d->Get(obj)->GetModelObjectId());
		tools::log_status_work(all_objects_3d, counter_objects, "writing", obj_model->GetName());
		navis_object::navis_object(this->r_project, &this->grids_collection_in_meshes.at(obj), &non_levels_objects, obj_model);
		counter_objects++;
	}
	scene.AddNode(non_levels_objects);
		
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
