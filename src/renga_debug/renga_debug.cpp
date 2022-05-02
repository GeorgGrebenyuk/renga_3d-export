// renga_debug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "renga_core.h"
#include <windows.h>

int main()
{
    setlocale(LC_ALL, "");
    //TestHouse_Offset problem with getting props in IBuildingInfoPtr
    std::string path_to_files_in_dir;
    std::cout << "Press path to folder with Renga projects (rnp):" << std::endl;
    std::cout << "Введите путь до папки с файлами проектов (rnp):" << std::endl;
    std::cin >> path_to_files_in_dir;

    int conv_type;
    std::cout << "Press type of project conversion (0: Navisworks)" << std::endl;
    std::cout << "Укажите тип для преобразования проекта (0: Navisworks)" << std::endl;
    std::cin >> conv_type;


    std::string current_path = "E:\\VC_Projects\\BDC_2022\\20_Models\\Renga\\EKB-3-Гараж-АР.rnp"; //"ttage.rnp" E:\\Temp\\BridgeEx.rnp"
    
    const char* new_path = current_path.c_str();

    default_start(new_path, 0);

    std::cout << "End!\n";
    return 0;
}

