#pragma once
#include "fbxsdk.h"
#include "renga_data.hpp"
class fbx
{
public:
	fbx(renga_data* data);
private:
	FbxManager* lSdkManager;
	std::map<std::vector<unsigned short>, FbxSurfacePhong*> color2materials;
	std::map<std::vector<unsigned short>, FbxSurfacePhong*>::iterator check_objects;
	void materials_work(object_3d_info* info, int counter, FbxNode* to_assign);
};

