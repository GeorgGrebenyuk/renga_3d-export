#include "navis.h"
#define LI_NWC_NO_PROGRESS_CALLBACKS NULL
#define LI_NWC_NO_USER_DATA NULL

navis::navis(std::vector<renga_object> objects, std::wstring path_to_save) {

	this->renga_objects = objects;
	this->path_to_save = path_to_save;
	// Then initialise the rest of the API.

	// Initialise low-level API first.
	LiNwcApiErrorInitialise();
	int status = 0;
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		this->start_export();
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
void navis::start_export() {
	LcNwcScene scene;

	LtWideString wfilename = path_to_save.c_str();
	for (int counter_object = 0; counter_object < this->renga_objects.size(); counter_object++) 
	{
		renga_object one_object = this->renga_objects.at(counter_object);
		LcNwcGroup one_object_instance;
		//LcNwcGeometry object_geometry;
		one_object_instance.SetClassName(L"some class", "");
		for (int counter_meshes = 0; counter_meshes < one_object.geometry_data.size(); counter_meshes++)
		{
			std::vector<std::vector< std::vector<unsigned int>>> meshes_grids = one_object.geometry_data[counter_meshes];
			LcNwcGroup one_mesh;
			one_mesh.SetLayer(TRUE);
			one_mesh.SetName(L"one_internal_mesh");
			for (int counter_grids = 0; counter_grids < meshes_grids.size(); counter_grids++)
			{
				std::vector< std::vector<unsigned int>> grids_info = meshes_grids[counter_grids];
				LcNwcGeometry grid_triangles_geometry;
				grid_triangles_geometry.SetName(L"one_internal_grid");
				LcNwcGeometryStream stream_grid_record = grid_triangles_geometry.OpenStream();
				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				for (int counter_triangles = 0; counter_triangles < grids_info.size(); counter_triangles++)
				{
					std::vector<unsigned int> triangle_definition = grids_info[counter_triangles];

					Renga::FloatPoint3D p1 = one_object.points_list[triangle_definition[0]];
					Renga::FloatPoint3D p2 = one_object.points_list[triangle_definition[1]];
					Renga::FloatPoint3D p3 = one_object.points_list[triangle_definition[2]];

					//stream_grid_record.IndexedVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					//stream_grid_record.IndexedVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					//stream_grid_record.IndexedVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);

					stream_grid_record.IndexedVertex(p1.X / 1000, p1.Y / 1000, p1.Z / 1000);
					stream_grid_record.IndexedVertex(p2.X / 1000, p2.Y / 1000, p2.Z / 1000);
					stream_grid_record.IndexedVertex(p3.X / 1000, p3.Y / 1000, p3.Z / 1000);

					stream_grid_record.TriangleIndex(counter_triangles);
					//stream_grid_record.ConvexPolyIndex(counter_triangles + 1);
					
				}
				stream_grid_record.SeqEnd();
				stream_grid_record.End();
				grid_triangles_geometry.CloseStream(stream_grid_record);
				one_mesh.AddNode(grid_triangles_geometry);
			}
			one_object_instance.AddNode(one_mesh);
		}
		scene.AddNode(one_object_instance);
	}
	scene.WriteCache(L"", this->path_to_save.c_str(), LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}
static LtBoolean LI_NWC_API grid_creating(LtNwcGeometry grid_geometry, LtNwcGeometryStream stream) 
{
	LcNwcGeometryStream s(stream);
}
