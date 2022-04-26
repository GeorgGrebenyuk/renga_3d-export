stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				stream_grid_record.BeginPolygon();
				for (int counter_triangles = 0; counter_triangles < grids_info.size(); counter_triangles++)
				{
					std::vector<unsigned int> triangle_definition = grids_info[counter_triangles];

					Renga::FloatPoint3D p1 = one_object.points_list[triangle_definition[0]];
					Renga::FloatPoint3D p2 = one_object.points_list[triangle_definition[1]];
					Renga::FloatPoint3D p3 = one_object.points_list[triangle_definition[2]];

					stream_grid_record.BeginPolygonContour();
					stream_grid_record.PolygonVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					stream_grid_record.PolygonVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					stream_grid_record.PolygonVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);
					stream_grid_record.EndPolygonContour();
				}
				stream_grid_record.EndPolygon();
				stream_grid_record.End();
				
				
LcNwcGeometryStream stream_grid_record = grid_triangles_geometry.OpenStream();
				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				//stream_grid_record.BeginPolygon();
				for (int counter_triangles = 0; counter_triangles < grids_info.size(); counter_triangles++)
				{
					std::vector<unsigned int> triangle_definition = grids_info[counter_triangles];

					Renga::FloatPoint3D p1 = one_object.points_list[triangle_definition[0]];
					Renga::FloatPoint3D p2 = one_object.points_list[triangle_definition[1]];
					Renga::FloatPoint3D p3 = one_object.points_list[triangle_definition[2]];

					stream_grid_record.TriFanVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					stream_grid_record.TriFanVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					stream_grid_record.TriFanVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);
					
				}
				//stream_grid_record.EndPolygon();
				stream_grid_record.End();

				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);
				stream_grid_record.End();

void navis::start_export() {
	LcNwcScene scene;

	LtWideString wfilename = path_to_save.c_str();
	for (int counter_object = 0; counter_object < this->renga_objects.size(); counter_object++) 
	{
		renga_object one_object = this->renga_objects.at(counter_object);

		//LtNwcBody object_body = LiNwcBodyCreate();
		LcNwcGroup body_meshes;
		body_meshes.SetName(L"meshes_coolection");
		body_meshes.SetLayer(TRUE);
		

		for (int counter_meshes = 0; counter_meshes < one_object.geometry_data.size(); counter_meshes++)
		{
			std::vector<std::vector< std::vector<unsigned int>>> meshes_grids = one_object.geometry_data[counter_meshes];
			//LtNwcSolid object_solid = LiNwcSolidCreate();
			//LtNwcShell object_shell = LiNwcShellCreate();
			LcNwcGroup solid_grids;
			solid_grids.SetName(L"grids_collection");
			solid_grids.SetLayer(TRUE);

			for (int counter_grids = 0; counter_grids < meshes_grids.size(); counter_grids++)
			{
				LcNwcGeometry en_geometry;
				en_geometry.SetName(L"faces_collection");
				LcNwcGeometryStream stream_grid_record = en_geometry.OpenStream();
				stream_grid_record.Begin(LI_NWC_VERTEX_NONE);

				std::vector< std::vector<unsigned int>> grids_info = meshes_grids[counter_grids];
				for (int counter_triangles = 0; counter_triangles < grids_info.size(); counter_triangles++)
				{
					std::vector<unsigned int> triangle_definition = grids_info[counter_triangles];

					Renga::FloatPoint3D p1 = one_object.points_list[triangle_definition[0]];
					Renga::FloatPoint3D p2 = one_object.points_list[triangle_definition[1]];
					Renga::FloatPoint3D p3 = one_object.points_list[triangle_definition[2]];

					//LtFloat p1_row[3]{ p1.X / 1000, p1.Z / 1000, p1.Y / 1000 };
					//LtFloat p2_row[3]{ p2.X / 1000, p2.Z / 1000, p2.Y / 1000 };
					//LtFloat p3_row[3]{ p3.X / 1000, p3.Z / 1000, p3.Y / 1000 };

					//LtNwcVertex p1_v = LiNwcVertexCreate(p1_row);
					//LtNwcVertex p2_v = LiNwcVertexCreate(p2_row);
					//LtNwcVertex p3_v = LiNwcVertexCreate(p3_row);

					stream_grid_record.TriFanVertex(p1.X / 1000, p1.Z / 1000, p1.Y / 1000);
					stream_grid_record.TriFanVertex(p2.X / 1000, p2.Z / 1000, p2.Y / 1000);
					stream_grid_record.TriFanVertex(p3.X / 1000, p3.Z / 1000, p3.Y / 1000);

					//LtNwcLine c1 = LiNwcLineCreate(p1_row, p2_row);
					//LtNwcLine c2 = LiNwcLineCreate(p3_row, p2_row);
					//LtNwcLine c3 = LiNwcLineCreate(p1_row, p3_row);
					
					//LtNwcCoedge k = LiNwcCoedgeCreate();

					//LtNwcEdge e1 = LiNwcEdgeCreate(c1, k, LI_NWC_SENSE_POSITIVE);
					//LtNwcEdge e2 = LiNwcEdgeCreate(c2, k, LI_NWC_SENSE_POSITIVE);
					//LtNwcEdge e3 = LiNwcEdgeCreate(c3, k, LI_NWC_SENSE_POSITIVE);
					
					//LtNwcPlane pl = LiNwcPlaneCreate(p1_row, p2_row, p3_row);
					//LtNwcFace p_face = LiNwcFaceCreate(pl, LI_NWC_SENSE_POSITIVE);
					//
					////body_stream.Begin(LI_NWC_BREP_BODY);
					//
					//tr_stream.BRepEntity(p_face);
					//tr_stream.End();
					//body_loops.AddNode(en_geometry);
					
				}
				stream_grid_record.End();
				en_geometry.CloseStream(stream_grid_record);
				body_meshes.AddNode(solid_grids);
			}

			//LiNwcSolidAddShell(object_solid, object_shell);
			//LiNwcBodyAddSolid(object_body, object_solid);
		}
		//LcNwcBRepEntity brep_body(object_body);
		
		
		//body_stream.BRepEntity(brep_body);
		//body_stream.End();
		
		scene.AddNode(body_meshes);
	}
	scene.WriteCache(L"", this->path_to_save.c_str(), LI_NWC_NO_PROGRESS_CALLBACKS, LI_NWC_NO_USER_DATA);
}