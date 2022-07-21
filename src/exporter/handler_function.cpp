#include "pch.h"
#include "external_functions.h"
#include "general_data.hpp"
#include "Renga\CreateApplication.hpp"
#include "renga_data.hpp"

void run_exporter(bool use_hidden, int use_max_triangles,
	bool recalc, int export_mode, int export_format) 
{
	auto renga_app = Renga::CreateApplication();
	if (renga_app)
	{
		if (use_max_triangles > 0) max_triangles = use_max_triangles;
		use_recalc = recalc;
		use_hidded = use_hidden;

		renga_app->UI->ShowMessageBox(Renga::MessageIcon_Info, "Note", "OK!");

		//renga_data project_info(renga_app, export_mode);
		//run other classes for export needing data
		
	}
	else
	{
		//error
	}
}