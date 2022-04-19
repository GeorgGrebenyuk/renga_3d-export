#include "actions.h"
renga_object::renga_object(Renga::IExportedObject3DPtr internal_object) {
	//Выборка только стен
	this->obj_type = internal_object->GetModelObjectType();
	unsigned int counter_points = 0;
	//unsigned int counter_mesh_temp = 0;
	//unsigned int counter_grid_temp = 0;
	//unsigned int counter_triangle_temp = 0;
	//Multti mesh for doors and windows. Need an exception

	//std::vector<std::vector<std::vector< std::vector<unsigned int>>>> meshes;
	for (int counter_mesh = 0; counter_mesh < internal_object->GetMeshCount(); counter_mesh++)
	{
		Renga::IMeshPtr pMesh = internal_object->GetMesh(counter_mesh);
		std::vector<std::vector< std::vector<unsigned int>>> mesh_grids;
		//pMesh is closed shell structure defined by some of grids
		for (int counter_grid = 0; counter_grid < pMesh->GetGridCount(); counter_grid++)
		{
			Renga::IGridPtr pGrid = pMesh->GetGrid(counter_grid);
			std::vector< std::vector<unsigned int>> grid_triangles;
			for (int counter_triangle = 0; counter_triangle < pGrid->GetTriangleCount(); counter_triangle++)
			{
				Renga::Triangle grid_triangle = pGrid->GetTriangle(counter_triangle);
				Renga::FloatPoint3D p1 = pGrid->GetVertex(grid_triangle.V0);
				Renga::FloatPoint3D p2 = pGrid->GetVertex(grid_triangle.V1);
				Renga::FloatPoint3D p3 = pGrid->GetVertex(grid_triangle.V2);
				this->points_list.push_back(p1);
				this->points_list.push_back(p2);
				this->points_list.push_back(p3);

				std::vector<unsigned int> triangles_indexes{ counter_points,counter_points + 1,counter_points + 2 };
				grid_triangles.push_back(triangles_indexes);
				//this->triangles_definition.push_back(triangles_indexes);
				counter_points += 3;
				//this->grid_definition.push_back(counter_triangle_temp);
				//counter_triangle_temp++;
			}
			mesh_grids.push_back(grid_triangles);
			//this->mesh_definition.push_back(counter_grid_temp);
			//counter_grid_temp++;
		}
		this->geometry_data.push_back(mesh_grids);
		//this->meshes_definition.push_back(counter_mesh_temp);
		//counter_mesh_temp++;
	}


}