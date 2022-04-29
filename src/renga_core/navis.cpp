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

	//Create arrays with objects id on each level and as other list - objects with non level property
	//std::map<Renga::ILevelObjectPtr, std::list<int>> level2ids;
	std::list<int> non_level_objects;
	//Parse objects on level
	LcNwcGroup non_levels_objects;
	non_levels_objects.SetName(L"Models objects");
	non_levels_objects.SetLayer(TRUE);

	for (Renga::IModelObjectPtr one_level : m_levels)
	{
		Renga::ILevelPtr pLevel;
		one_level->QueryInterface(&pLevel);
		LcNwcGroup levels_objects;
		levels_objects.SetLayer(TRUE);
		levels_objects.SetName(pLevel->LevelName);

		for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++)
		{
			Renga::IExportedObject3DPtr internal_object = objects_collection3d->Get(counter_object);
			int internal_object_id = internal_object->GetModelObjectId();
			Renga::IModelObjectPtr pModelObject = objects_collection->GetById(internal_object_id);

			bool is_level = (std::find(c_levelTreeTypes.begin(), c_levelTreeTypes.end(), pModelObject->ObjectType) != c_levelTreeTypes.end());
			bool is_level_non = (std::find(c_nonLevelTreeTypes.begin(), c_nonLevelTreeTypes.end(), pModelObject->ObjectType) != c_nonLevelTreeTypes.end());
			if (is_level) tools::nwc_object(&counter_object, &levels_objects, this->r_project);
			else if (!is_level && is_level_non)
			{
				bool was_id = (std::find(non_level_objects.begin(), non_level_objects.end(), counter_object) != non_level_objects.end());
				if (!was_id)
				{
					tools::nwc_object(&counter_object, &non_levels_objects, this->r_project);
				}
			}
		}
		scene.AddNode(levels_objects);
		
	}
	scene.AddNode(non_levels_objects);
	
	
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
