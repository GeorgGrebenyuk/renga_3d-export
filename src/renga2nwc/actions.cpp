#include "actions.h"

actions::actions(Renga::IApplicationPtr app) {
	this->app = app;
	this->current_project_file_path = this->app->Project->FilePath;
}
int actions::start() {
	// Initialise low-level API first. 
	LiNwcApiErrorInitialise();

	// Then initialise the rest of the API. 
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		test_process();
		break;
	case LI_NWC_API_NOT_LICENSED:
		return 1;

	case LI_NWC_API_INTERNAL_ERROR:
	default:
		return 1;
	}
	LiNwcApiTerminate();
	return 0;
}
void actions::test_process() {
	//std::string dir_path = "E:\\Temp\\";
	GUID uniq_id;
	CoCreateGuid(&uniq_id);
	std::string  uniq_id2 = std::to_string(uniq_id.Data1);
	std::string all_temp_path = this->current_project_file_path + uniq_id2 + ".nwc";
	std::wstring all_temp_path_w(std::begin(all_temp_path), std::end(all_temp_path));
	LtWideString wfilename = all_temp_path_w.c_str();

	LtNwcScene scene;
	LtNwcGeometry geom;
	LtNwcGeometryStream stream;


	//create scene and geometry
	scene = LiNwcSceneCreate();
	geom = LiNwcGeometryCreate();

	//open geometry stream, draw a triangle, close stream
	stream = LiNwcGeometryOpenStream(geom);
	LiNwcGeometryStreamBegin(stream, 0);    LiNwcGeometryStreamTriangleVertex(stream, 1, 0, 0);
	LiNwcGeometryStreamTriangleVertex(stream, 2, 0, 10);
	LiNwcGeometryStreamTriangleVertex(stream, 3, 10, 10);

	//********texture********
	/*LcNwcPresenterMaterial* mat = create_texture(L"TestLayout.bmp");
	LiNwcNodeAddAttribute(geom, *mat);
	delete mat;*/

	LiNwcGeometryStreamEnd(stream);    LiNwcGeometryCloseStream(geom, stream);

	//add the geometry to the scene and cleanup geom
	LiNwcSceneAddNode(scene, geom);
	LiNwcGeometryDestroy(geom);

	//write out the NWC file
	LiNwcSceneWriteCacheEx(scene, wfilename, wfilename, 0, 0);

	LiNwcSceneDestroy(scene);
}