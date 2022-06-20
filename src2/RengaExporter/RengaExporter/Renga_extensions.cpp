#include "pch.hpp"
#include "Renga_extensions.hpp"
#include "Renga\GridTypes.h"
#include "Renga\ObjectTypes.h"

Renga_getting_data::Renga_getting_data(Renga::IApplicationPtr app, bool UseHiddenObjects,
    std::vector<double> coord_offset)
{
	this->renga_project = app->Project;
    //Start parse objects geometry
    Renga::IDataExporterPtr data_exporter = this->renga_project->GetDataExporter();
    Renga::IExportedObject3DCollectionPtr objects_3d_collection =
        data_exporter->GetObjects3D();
    Renga::IModelObjectCollectionPtr objects_model_collection =
		this->renga_project->Model->GetObjects();
	Renga::IViewPtr pView = app->GetActiveView();
	Renga::IModelViewPtr pModelView;
	pView->QueryInterface(&pModelView);
	
    
    //Start working with mesh data
    for (int counter_objects_3d = 0; counter_objects_3d < objects_3d_collection->GetCount(); counter_objects_3d++)
    {
		Renga_object_definition obj_info;
        Renga::IExportedObject3DPtr object_3d = objects_3d_collection->Get(counter_objects_3d);
		int model_object_id = object_3d->GetModelObjectId();
        Renga::IModelObjectPtr object_model = objects_model_collection->GetById(model_object_id);
		bool obj_vis = pModelView->IsObjectVisible(model_object_id);
		obj_info.object_visiable = obj_vis;
        //std::list<std::list<std::map<std::vector<int>,std::list<std::vector<double>>>>>;
		
        for (int counter_meshes = 0; counter_meshes < object_3d->GetMeshCount(); counter_meshes++)
        {
			std::vector<double> object_color;
			this->get_material(object_model, &object_color);
            Renga::IMeshPtr pMesh = object_3d->GetMesh(counter_meshes);
			std::list<grid_definition> grids_in_mesh;
			for (int counter_grids = 0; counter_grids < pMesh->GetGridCount(); counter_grids++)
			{
				grid_definition grid_info;
				Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grids);
				std::vector<double> color = object_color;
                //Add data to temp arrays in future
				this->get_material_grid(object_model->GetObjectType(), pGrid->GetGridType(), &color);
				grid_info.color = object_color;
                for (int counter_triangles = 0; counter_triangles < pGrid->GetTriangleCount(); counter_triangles++)
                {
                    Renga::Triangle triangle_definition = pGrid->GetTriangle(counter_triangles);
                    Renga::FloatPoint3D p1 = pGrid->GetVertex(triangle_definition.V0);
                    Renga::FloatPoint3D p2 = pGrid->GetVertex(triangle_definition.V1);
                    Renga::FloatPoint3D p3 = pGrid->GetVertex(triangle_definition.V2);
                    this->recalc_coords(&p1);
                    this->recalc_coords(&p2);
                    this->recalc_coords(&p3);
					grid_info.grid_geometry.push_back({ 
						p1.X,p1.Y,p1.Z,
						p2.X,p2.Y,p2.Z, 
						p3.X,p3.Y,p3.Z });
                }
				grids_in_mesh.push_back(grid_info);
			}
			obj_info.object_geometry.push_back(grids_in_mesh);
        }
		//End of adding data
		this->objects[model_object_id] = obj_info;
    }
}
void Renga_getting_data::get_material(Renga::IModelObjectPtr obj, std::vector<double>* link_color)
{
	//R-tree for IGridWithMaterialCollection in future (3d index)
	std::vector<double> object_color = { 1.0,1.0,1.0 };
	double transparency = 0.0;

	Renga::IObjectWithMaterialPtr pObjectWithMaterial;
	obj->QueryInterface(&pObjectWithMaterial);
	if (pObjectWithMaterial)
	{
		if (Renga::IMaterialPtr obj_material = this->renga_project->GetMaterialManager()->GetMaterial(pObjectWithMaterial->GetMaterialId())) {
			Renga::Color mat_color = obj_material->GetColor();

			double color_red(mat_color.Red / 255.0);
			double color_green(mat_color.Green / 255.0);
			double color_blue(mat_color.Blue / 255.0);
			object_color = { color_red ,color_green,color_blue, 1.0 };
			//return object_color;
		}

	}
	Renga::IObjectWithLayeredMaterialPtr pObjectWithLayeredMaterial;
	obj->QueryInterface(&pObjectWithLayeredMaterial);
	if (pObjectWithLayeredMaterial) {
		if (Renga::ILayeredMaterialPtr pLayeredMaterial = this->renga_project->GetLayeredMaterialManager()
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
					object_color = { color_red ,color_green,color_blue, 1.0 };
				}
				else
				{
					//std::cout << "error " << this->current_model_object->GetName() << std::endl;
				}
			}
		}
	}

	(*link_color) = object_color;
}
void Renga_getting_data::get_material_grid(GUID object_type, int grid_type, std::vector<double>*link_color)
{
	if ((object_type) == Renga::ObjectTypes::Window)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Window::Frame:
			*link_color = { 0.729,0.596,0.274,1.0 };
			break;
		case Renga::GridTypes::Window::Glass:
			*link_color = { 0.760,0.964,0.929,0.6 };
			break;
		case Renga::GridTypes::Window::Sill:
			*link_color = { 0.674,0.674,0.674,1.0 };
			break;
		case Renga::GridTypes::Window::OutwardSill:
			*link_color = { 0.674,0.674,0.674,1.0 };
			break;
		case Renga::GridTypes::Window::Reveal:
			*link_color = { 0.674,0.674,0.674,0.1 };
			break;
		}
	}
	else if ((object_type) == Renga::ObjectTypes::Door)
	{
		switch (grid_type)
		{
		case Renga::GridTypes::Door::Glass:
			*link_color = { 0.760,0.964,0.929, 0.6 };
			break;
		case Renga::GridTypes::Door::Frame:
			*link_color = { 0.4,0.2,1.0,1.0 };
			break;
		case Renga::GridTypes::Door::Solid:
			*link_color = { 0.4,0.2,0.0,1.0 };
			break;
		case Renga::GridTypes::Door::DoorLining:
			*link_color = { 0.4,0.0,0.0,1.0 };
			break;
		case Renga::GridTypes::Door::Threshold:
			*link_color = { 0.4,0.0,0.0,1.0 };
			break;
		}
	}
}
void Renga_getting_data::properties_work(Renga::IModelObjectPtr obj)
{

}
