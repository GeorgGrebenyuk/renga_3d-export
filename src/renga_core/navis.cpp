#include "actions.h"
#include "nwcreate/LiNwcAll.h"
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
	
	std::list<Renga::IModelObjectPtr> m_levels;
	for (int counter_object = 0; counter_object < objects_collection3d->Count; counter_object++) 
	{
		Renga::IModelObjectPtr pModelObject = objects_collection->GetByIndex(counter_object);

		if (pModelObject->GetObjectType() == Renga::ObjectTypes::Level) 
		{
			m_levels.push_back(pModelObject);
		}
	}

	//Parse objects
	LcNwcGroup non_level_objects;
	non_level_objects.SetName(L"Objects without level");
	non_level_objects.SetLayer(TRUE);
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
			Renga::IModelObjectPtr object_data = objects_collection->GetById(internal_object_id);
			Renga::ILevelObjectPtr object_on_level;
			object_data->QueryInterface(&object_on_level);
			bool is_level = (std::find(c_levelTreeTypes.begin(), c_levelTreeTypes.end(), object_data->ObjectType) != c_levelTreeTypes.end());
			if (is_level && object_on_level->GetLevelId() == one_level->Id)
			{
				LcNwcGroup one_object_instance;
				one_object_instance.SetLayer(TRUE);
				one_object_instance.SetName(object_data->GetName());

				std::vector<double> color_of_object = this->object_color(object_data);
				LcNwcMaterial body_material;
				body_material.SetDiffuseColor(color_of_object[0], color_of_object[1], color_of_object[2]);
				body_material.SetAmbientColor(color_of_object[0], color_of_object[1], color_of_object[2]);
				one_object_instance.AddAttribute(body_material);

				for (int counter_meshes = 0; counter_meshes < internal_object->GetMeshCount(); counter_meshes++)
				{
					Renga::IMeshPtr pMesh = internal_object->GetMesh(counter_meshes);
					LcNwcGroup one_mesh;
					one_mesh.SetLayer(TRUE);
					one_mesh.SetName(L"one_internal_mesh");

					for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
					{
						Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
						LcNwcGeometry grid_triangles_geometry;
						grid_triangles_geometry.SetName(L"one_internal_grid");
						LcNwcGeometryStream stream_grid_record = grid_triangles_geometry.OpenStream();
						for (int counter_triangles = 0; counter_triangles < pGrid->GetTriangleCount(); counter_triangles++)
						{
							Renga::Triangle triangle_definition = pGrid->GetTriangle(counter_triangles);

							Renga::FloatPoint3D p1 = pGrid->GetVertex(triangle_definition.V0);
							Renga::FloatPoint3D p2 = pGrid->GetVertex(triangle_definition.V1);
							Renga::FloatPoint3D p3 = pGrid->GetVertex(triangle_definition.V2);

							stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
							stream_grid_record.TriFanVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
							stream_grid_record.TriFanVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
							stream_grid_record.TriFanVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);
							stream_grid_record.End();
						}
						grid_triangles_geometry.CloseStream(stream_grid_record);
						one_mesh.AddNode(grid_triangles_geometry);
					}
					one_object_instance.AddNode(one_mesh);
				}
				levels_objects.AddNode(one_object_instance);
			}
			else 
			{
				std::cout << "non level for " << object_data->GetName() << std::endl;
			}
		}
		scene.AddNode(levels_objects);
	}
	
	scene.WriteCache(L"", wfilename, LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
std::vector<double> renga_data::object_color(Renga::IModelObjectPtr obj) {

	std::vector<double> object_color = { 1.0,1.0,1.0 };

	Renga::IObjectWithMaterialPtr pObjectWithMaterial;
	obj->QueryInterface(&pObjectWithMaterial);
	if (pObjectWithMaterial)
	{
		if (Renga::IMaterialPtr obj_material = this->r_project->GetMaterialManager()->GetMaterial(pObjectWithMaterial->GetMaterialId())) {
			Renga::Color mat_color = obj_material->GetColor();

			double color_red(mat_color.Red / 255.0);
			double color_green(mat_color.Green / 255.0);
			double color_blue(mat_color.Blue / 255.0);
			object_color = { color_red ,color_green,color_blue };
			//return object_color;
		}
		
	}
	Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
	obj->QueryInterface(&pObjectWithLayeredMaterial);
	if (pObjectWithLayeredMaterial) {
		if (Renga::ILayeredMaterialPtr pLayeredMaterial = this->r_project->GetLayeredMaterialManager()
			->GetLayeredMaterialByIdGroupPair(pObjectWithLayeredMaterial->GetLayeredMaterialIdGroupPair())) {
			Renga::IMaterialLayerCollectionPtr pLayers = pLayeredMaterial->GetLayers();
			if (pLayers->GetCount() > 0) {
				Renga::IMaterialLayerPtr pLayer = pLayers->Get(0);
				Renga::IMaterialPtr obj_material = pLayer->GetMaterial();

				if (obj_material != NULL)
				{
					Renga::Color mat_color = obj_material->GetColor();

					double color_red(mat_color.Red / 255.0);
					double color_green(mat_color.Green / 255.0);
					double color_blue(mat_color.Blue / 255.0);
					object_color = { color_red ,color_green,color_blue };
				}

				else
				{
					std::cout << "error " << obj->GetName() << std::endl;
				}

			}
			
		}
	}

	return object_color;
}