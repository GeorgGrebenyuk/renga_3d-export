#include "navis.h"
navis::navis(std::vector<renga_object>* objects, std::wstring path_to_save) {

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
	LtWideString wfilename = path_to_save.c_str();
	LtNwcScene scene;
	LtNwcGeometry geom;
	LtNwcGeometryStream stream;
	//create scene and geometry
	scene = LiNwcSceneCreate();
	geom = LiNwcGeometryCreate();
	//open geometry stream, draw a triangle, close stream
	stream = LiNwcGeometryOpenStream(geom);
	LiNwcGeometryStreamBegin(stream, 0);    
	LiNwcGeometryStreamTriangleVertex(stream, 1, 0, 0);
	LiNwcGeometryStreamTriangleVertex(stream, 2, 0, 10);
	LiNwcGeometryStreamTriangleVertex(stream, 3, 10, 10);
	LiNwcGeometryStreamEnd(stream);    
	LiNwcGeometryCloseStream(geom, stream);
	//add the geometry to the scene and cleanup geom
	LiNwcSceneAddNode(scene, geom);
	LiNwcGeometryDestroy(geom);
	//write out the NWC file
	LiNwcSceneWriteCacheEx(scene, wfilename, wfilename, 0, 0);
}