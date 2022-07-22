#include "pch.h"
#include "external_functions.h"
#include "general_data.hpp"
#include "Renga\CreateApplication.hpp"
#include "renga_data.hpp"
#include "navisworks.hpp"

int32_t run_exporter(bool use_hidden, int use_max_triangles,
	bool recalc, int export_mode, int export_format) 
{
	auto renga_app = Renga::CreateApplication();
	if (renga_app)
	{
		//renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", "Начата процедура экспорта");

		if (use_max_triangles > 0) max_triangles = use_max_triangles;
		use_recalc = recalc;
		use_hidded = use_hidden;
		export_formats = export_format;

		renga_data project_info(renga_app, export_mode);
		//run other classes for export needing data

		if (export_format == 0) new navisworks(&project_info);
		//else if (export_format == 1) new fbx (&project_info);

		renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Сообщение", "Экспорт окончен!");
	}
	else
	{
		//error
	}
	return 0;
}