#include "actions.h"
void tools::level2ids(Renga::IProjectPtr link_project,std::list<level_objects>* level2object, std::list<Renga::IExportedObject3DPtr>* non_level_objects)
{
	Renga::IDataExporterPtr pDataExporter = link_project->GetDataExporter();
	Renga::IExportedObject3DCollectionPtr objects_collection3d = pDataExporter->GetObjects3D();
	Renga::IModelObjectCollectionPtr objects_collection = link_project->GetModel()->GetObjects();

	for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++)
	{
		Renga::IExportedObject3DPtr internal_object = objects_collection3d->Get(counter_object);
		//int internal_object_id = internal_object->GetModelObjectId();
		Renga::IModelObjectPtr pModelObject = objects_collection->GetById(internal_object->GetModelObjectId());
		Renga::ILevelObjectPtr pLevelObject;
		pModelObject->QueryInterface(&pLevelObject);

		bool is_level = (std::find(c_levelTreeTypes.begin(), c_levelTreeTypes.end(), pModelObject->ObjectType) != c_levelTreeTypes.end());
		bool is_level_non = (std::find(c_nonLevelTreeTypes.begin(), c_nonLevelTreeTypes.end(), pModelObject->ObjectType) != c_nonLevelTreeTypes.end());
		if (is_level)
		{
			//level_objects new_item;
			//new_item.ids.push_back(counter_object);
			//new_item.level_model_id = pLevelObject->LevelId;
			if (!(*level2object).empty())
			{
				bool level_find_status = false;
				for (level_objects &one_group : (*level2object))
				{
					if (one_group.level_model_id == pLevelObject->LevelId)
					{
						one_group.add_item(internal_object);
						//one_group.ids.push_back(counter_object);
						level_find_status = true;
						//break;
					}
				}
				if (!level_find_status)
				{
					(*level2object).push_back(level_objects(pLevelObject->LevelId, internal_object));
					//(*level2object).push_back(new_item);
				}
			}
			else 
			{
				(*level2object).push_back(level_objects(pLevelObject->LevelId, internal_object));
				//(*level2object).push_back(new_item);
			}
		}
		else if (!is_level && is_level_non)
		{
			(*non_level_objects).push_back(internal_object);
		}
	}
}