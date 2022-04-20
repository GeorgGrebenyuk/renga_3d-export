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
