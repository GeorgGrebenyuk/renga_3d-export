#include "renga2nwc_plugin.h"
#include <Renga/CreateApplication.hpp>
#include <random>
#include<string>


bool renga2nwc::initialize(const wchar_t* pluginPath) {

	auto pApplication = Renga::CreateApplication();
	if (!pApplication)
		return false;
	r_app = pApplication;
	r_project = r_app->Project;

	if (auto pUI = pApplication->GetUI())
	{
		Renga::IImagePtr pImage = pUI->CreateImage();
		pImage->LoadFromFile(L"C:\\Program Files\\Renga\\Plugins\\renga2nwc\\navis_logo.png");

		Renga::IActionPtr pAction = pUI->CreateAction();
		pAction->PutDisplayName(L"Navisworks_logo");
		pAction->PutToolTip(L"Click to start export procedure");
		pAction->PutIcon(pImage);
		

		Renga::IUIPanelExtensionPtr pUIPanelExtension = pUI->CreateUIPanelExtension();
		pUIPanelExtension->AddToolButton(pAction);

		// Add controls to the primary panel:
		pUI->AddExtensionToPrimaryPanel(pUIPanelExtension);

		// Add controls to the action panel, specific for the 3D view:
		pUI->AddExtensionToActionsPanel(pUIPanelExtension, Renga::ViewType::ViewType_View3D);

		const GUID actionId = { 0xa101fd63, 0x9e62, 0x48fa,{ 0xb6, 0xda, 0x2b, 0x56, 0x12, 0xd6, 0x95, 0xad } };
		auto pAction = pUI->CreateActionWithId(actionId);
	}
}
void ButtonBaseHandler::OnTriggered() {
	start();
}


void renga2nwc::stop (){
	r_app = nullptr;
	r_project = nullptr;
}
int renga2nwc::start() {
	// Initialise low-level API first. 
	LiNwcApiErrorInitialise();

	// Then initialise the rest of the API. 
	switch (LiNwcApiInitialise())
	{
	case LI_NWC_API_OK:
		GettingGeometry();
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
void renga2nwc::GettingGeometry() {
	std::string dir_path = "E:\\Temp\\";
	GUID uniq_id;
	CoCreateGuid(&uniq_id);
	std::string  uniq_id2 = std::to_string(uniq_id.Data1);
	std::string all_temp_path = dir_path + uniq_id2 + ".nwc";
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
