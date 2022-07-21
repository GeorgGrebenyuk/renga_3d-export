#pragma once
#ifdef EXPORT_3D_FUNCTIONS
#define EXPORT_3D_FUNCTIONS_API __declspec(dllexport)
#else
#define EXPORT_3D_FUNCTIONS_API __declspec(dllimport)
#endif
/// <summary>
/// General method to start actions in that cpp library
/// </summary>
/// <param name="use_hidden">1 = use, 0 = ignor</param>
/// <param name="use_max_triangles">Count of max triangles</param>
/// <param name="recalc">1 = use, 0 = ignor</param>
/// <param name="export_mode">1 = use grids, 0 = use objects 3d</param>
/// <param name="export_format">0 = nwc, 1 = fbx</param>
/// <returns></returns>
extern "C" void EXPORT_3D_FUNCTIONS_API __stdcall run_exporter(bool use_hidden, int use_max_triangles,
	bool recalc, int export_mode, int export_format);