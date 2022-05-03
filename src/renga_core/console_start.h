#pragma once

#ifdef RENGA_CORE_EXPORTS
#define RENGA_CORE_API __declspec(dllexport)
#else
#define RENGA_CORE_API __declspec(dllimport)
#endif
//Entry point for console application
extern "C" RENGA_CORE_API void default_start(const char * path_to_project, int type_conversion);