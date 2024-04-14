#include "pch.h"
#include "external_functions.h"
#include "general_data.hpp"
#include "Renga\CreateApplication.hpp"
#include "renga_data.hpp"

#include <chrono>
#include <sstream>
#include <fstream>

#include "navisworks.hpp"
#include "fbx.hpp"

#include <locale>
#include <codecvt>
#include <string>

void get_recalc_parameters(bool* need_using, std::vector<double>* parameters)
{
	auto renga_app = Renga::CreateApplication();
	Renga::IProjectPtr project = renga_app->Project;
	Renga::IBuildingInfoPtr b_info = project->GetBuildingInfo();
	Renga::IPropertyContainerPtr props = b_info->GetProperties();

	bstr_t x_name = "IfcLocationX";
	double x_exist = -1.101;
	bstr_t y_name = "IfcLocationY";
	double y_exist = -1.101;
	bstr_t z_name = "IfcLocationZ";
	double z_exist = -1.101;
	bstr_t angle_name = "IfcDirectionPrecession";
	double angle_exist = -1.101;

	Renga::IGuidCollectionPtr ids = props->GetIds();
	for (int counter_properties = 0; counter_properties < ids->GetCount(); counter_properties++)
	{
		Renga::IPropertyPtr prop = props->Get(ids->Get(counter_properties));
		if (prop->Name == x_name && prop->Type == Renga::PropertyType::PropertyType_Length) x_exist = prop->GetDoubleValue();
		else if (prop->Name == y_name && prop->Type == Renga::PropertyType::PropertyType_Length) y_exist = prop->GetDoubleValue();
		else if (prop->Name == z_name && prop->Type == Renga::PropertyType::PropertyType_Length) z_exist = prop->GetDoubleValue();
		else if (prop->Name == angle_name && prop->Type == Renga::PropertyType::PropertyType_Angle) angle_exist = prop->GetAngleValue(Renga::AngleUnit_Degrees);
	}
	if (x_exist != 0.0 && y_exist != 0.0 && z_exist != 0.0 && angle_exist != 0.0)
	{
		(*need_using) = true;
		(*parameters) = { x_exist/1000.0,y_exist / 1000.0 ,z_exist / 1000.0 ,angle_exist };
	}
}
int32_t run_exporter(bool input_use_hidden, int input_use_max_triangles,
	bool input_recalc, int input_export_mode, int input_export_format, bool include_sub_geometry)
{
	auto renga_app = Renga::CreateApplication();
	if (renga_app)
	{
		//renga_app->Enabled = false;
		//renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", "Начата процедура экспорта");

		export_configs parameters;
		if (input_use_max_triangles > 0)
		{
			parameters.use_max_triangles = true;
			parameters.maximum_triangles_count = input_use_max_triangles;
		}
		else 
		{
			parameters.use_max_triangles = false;
		}
		if (input_recalc)
		{
			bool need_use = false;
			std::vector<double> recalc_data;
			get_recalc_parameters(&need_use, &recalc_data);
			if (need_use) {
				parameters.use_recalc = true;
				parameters.recalc_params = recalc_data;
			}
			else 
			{
				parameters.use_recalc = false;
			}
		}
		else 
		{
			parameters.use_recalc = false;
		}
		parameters.include_sub_geometry = include_sub_geometry;
		parameters.use_recalc = input_recalc;
		parameters.use_hidded = input_use_hidden;
		parameters.export_formats = input_export_format;
		parameters.geometry_mode = input_export_mode;

		std::chrono::system_clock::time_point info_start_time_record = std::chrono::system_clock::now();
		renga_data project_info(renga_app, parameters);
		//run other classes for export needing data

		if (input_export_format == 0) new navisworks(&project_info, &parameters);
		else if (input_export_format == 1) new fbx(&project_info);

		std::chrono::system_clock::time_point info_time_end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = info_time_end - info_start_time_record;

		std::stringstream output_data;
		output_data << "Экспорт окончен! \n";
		output_data << "Число граней =  " << project_info.info_triangles_count << "\n";
		output_data << "Время обработки =  " << elapsed_seconds.count() << " с.\n";

		//HWND app_info = (HWND)renga_app->GetMainWindowHandle();

		//MessageBox(app_info, w_output_data.c_str(), L"Renga 3d export - log", MB_OK | MB_ICONQUESTION);

		//renga_app->Enabled = true;
		//renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", output_data.str().c_str());
	}
	else
	{
		//error
	}
	return 0;
}