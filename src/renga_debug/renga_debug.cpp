// renga_debug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "console_start.h"
#include <windows.h>
#include <filesystem>
#include <comdef.h>
namespace fs = std::filesystem;

int main()
{
    setlocale(LC_ALL, "");
    //TestHouse_Offset problem with getting props in IBuildingInfoPtr
    std::string path_to_files_in_dir;
    std::cout << "Press path to folder with Renga projects (rnp):" << std::endl;
    std::cout << "Введите путь до папки с файлами проектов (rnp):\n" << std::endl;
    std::cin >> path_to_files_in_dir;
    std::cout << std::endl;

    int conv_type;
    std::cout << "Press type of project conversion (0: Navisworks)" << std::endl;
    std::cout << "Укажите тип для преобразования проекта (0: Navisworks)\n" << std::endl;
    std::cin >> conv_type;
    std::cout << std::endl;

    for (const auto& entry : fs::directory_iterator(path_to_files_in_dir))
    {
        fs::path one_path = entry.path();
        if (one_path.has_extension() && one_path.extension() == ".rnp")
        {
            std::cout << "Start for file " << one_path << std::endl;
            _bstr_t str_path(one_path.c_str());
            const char* new_path = str_path;
            default_start(new_path, 0);
            std::cout << "End!\n";
        }
    }
        

    //std::string current_path = "E:\\VC_Projects\\BDC_2022\\20_Models\\Renga\\EKB-3-Гараж-АР.rnp"; //"ttage.rnp" E:\\Temp\\BridgeEx.rnp"
    //

    return 0;
}

