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
		printf("Not Licensed\n");
		status = 1;
	case LI_NWC_API_INTERNAL_ERROR:
	default:
		printf("Internal Error\n");
		status = 2;

	}

	std::cout << "status = " << status << std::endl;

}
void renga_data::navis_export() {
	LcNwcScene scene;

	LtWideString wfilename = this->file_export_path.c_str();
	//Getting geometry, properties, materials ...
	Renga::IDataExporterPtr pDataExporter = this->r_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = this->r_project->GetModel()->GetObjects();
	
	std::cout << "objects_collection count = " << objects_collection->GetCount() << std::endl;
	std::cout << "objects_collection3d count = " << objects_collection3d->GetCount() << std::endl;

	//Getting levels
	std::list<Renga::IModelObjectPtr> m_levels;
	for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++) 
	{
		Renga::IModelObjectPtr pModelObject = objects_collection->GetByIndex(counter_object);
		if (pModelObject->GetObjectType() == Renga::ObjectTypes::Level)
		{
			m_levels.push_back(pModelObject);
		}
	}
	//Sort levels
	m_levels.sort(tools::compare_levels);
	
	//Create containers for objects on level and as other group -- objects without level's reference
	std::list<Renga::IExportedObject3DPtr> non_level_objects;
	std::list<level_objects> level2objects;

	std::map<int, Renga::IModelObjectPtr> id2level;
	for (Renga::IModelObjectPtr one_level : m_levels)
	{
		if (one_level != NULL)
		{
			id2level.insert(std::pair<int, Renga::IModelObjectPtr>(one_level->Id, one_level));
		}
		
	}
	std::cout << "end level work" << std::endl;
	tools::level2ids(this->r_project, &level2objects, &non_level_objects);
	std::cout << "end sort objects on levels" << std::endl;
	int counter_objects = 0;
	for (level_objects one_group : level2objects)
	{
		Renga::IModelObjectPtr level_object = id2level[one_group.level_model_id];
		Renga::ILevelPtr level_instanse;
		level_object->QueryInterface(&level_instanse);

		LcNwcGroup levels_objects;
		levels_objects.SetName(level_instanse->LevelName);
		levels_objects.SetLayer(TRUE);

		
		for (Renga::IExportedObject3DPtr obj : one_group.objects)
		{
			//std::cout << counter_objects << " " << std::endl;
			navis_object::navis_object(this->r_project, &this->projects_offset, &levels_objects, obj);
			counter_objects++;
		}
		scene.AddNode(levels_objects);
	}

	/*LcNwcGroup non_levels_objects;
	non_levels_objects.SetName(L"Models objects");
	non_levels_objects.SetLayer(TRUE);
	for (Renga::IExportedObject3DPtr obj : non_level_objects)
	{
		std::cout << counter_objects << " " << std::endl;
		navis_object::navis_object(this->r_project, &this->projects_offset, &non_levels_objects, obj);
	}
	scene.AddNode(non_levels_objects);*/
		
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
