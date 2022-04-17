#include "actions.h"
renga_object::renga_object(Renga::IExportedObject3DPtr internal_object) {
	//Выборка только стен
	this->obj_type = internal_object->GetModelObjectType();

	//Multti mesh for doors and windows. Need an exception
	for (int counter_mesh = 0; counter_mesh < internal_object->GetMeshCount(); counter_mesh++)
	{
		Renga::IMeshPtr pMesh = internal_object->GetMesh(counter_mesh);
		//pMesh is closed shell structure defined by some of grids
		for (int counter_grid = 0; counter_grid < pMesh->GetGridCount(); counter_grid++)
		{
			Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grid);

			for (int counter_triangle = 0; counter_triangle < pGrid->GetTriangleCount(); counter_triangle++)
			{
				Renga::Triangle grid_triangle = pGrid->GetTriangle(counter_triangle);
				Renga::FloatPoint3D p1 = pGrid->GetVertex(grid_triangle.V0);
				Renga::FloatPoint3D p2 = pGrid->GetVertex(grid_triangle.V1);
				Renga::FloatPoint3D p3 = pGrid->GetVertex(grid_triangle.V2);
				this->points_list.push_back(p1);
				this->points_list.push_back(p2);
				this->points_list.push_back(p3);

				unsigned int current_triangle_count_global = counter_mesh + counter_grid + counter_triangle;
				std::list<unsigned int> triangles_indexes{ current_triangle_count_global,current_triangle_count_global + 1,current_triangle_count_global + 2 };
				this->triangles_definition.push_back(triangles_indexes);

				this->grid_definition.push_back(counter_mesh + counter_grid + counter_triangle);
			}
			this->mesh_definition.push_back(counter_mesh + counter_grid);
		}
		this->meshes_definition.push_back(counter_mesh);
	}


}