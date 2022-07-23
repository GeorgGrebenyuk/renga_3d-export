#include "pch.h"
#include "external_functions.h"
#include "general_data.hpp"
#include "Renga\CreateApplication.hpp"
#include "renga_data.hpp"
#include "navisworks.hpp"
#include <chrono>
#include <sstream>

int32_t run_exporter(bool input_use_hidden, int input_use_max_triangles,
	bool input_recalc, int input_export_mode, int input_export_format)
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
		parameters.use_recalc = input_recalc;
		parameters.use_hidded = input_use_hidden;
		parameters.export_formats = input_export_format;
		parameters.geometry_mode = input_export_mode;

		std::chrono::system_clock::time_point info_start_time_record = std::chrono::system_clock::now();
		renga_data project_info(renga_app, parameters);
		//run other classes for export needing data

		if (input_export_format == 0) new navisworks(&project_info);
		//else if (export_format == 1) new fbx (&project_info);

		std::chrono::system_clock::time_point info_time_end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = info_time_end - info_start_time_record;

		std::stringstream output_data;
		output_data << "Экспорт окончен! \n";
		output_data << "Число граней =  " << project_info.info_triangles_count << "\n";
		output_data << "Время обработки =  " << elapsed_seconds.count() << " с.\n";
		//bstr_t text_out = output_data.str().c_str();
		//renga_app->Enabled = true;
		renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", output_data.str().c_str());
	}
	else
	{
		//error
	}
	return 0;
}