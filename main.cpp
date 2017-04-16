#include <iostream>
#include <fstream>
#include "Mirror.h"

bool file_exists(std::string filename)
{
    std::ifstream f(filename);
    return f.good();
}

int main(int argc, char** argv) {
    Mirror* mirror;


    if (file_exists(Mirror::CONFIG_FILENAME))
    {
        mirror = new Mirror(Mirror::CONFIG_FILENAME);
    }
    else
    {
        //no file found
        mirror = new Mirror();
    }

    mirror->run();

    std::cout << "Have a nice day." << std::endl;

    return 0;
}