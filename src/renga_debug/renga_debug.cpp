// renga_debug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "renga_core.h"

int main()
{
    std::string current_path = "E:\\DataTest\\Renga\\SummerSchool_IS.rnp"; //"ttage.rnp" E:\\Temp\\BridgeEx.rnp"

    const char* new_path = current_path.c_str();

    default_start(new_path, 0);

    std::cout << "End!\n";
    return 0;
}

