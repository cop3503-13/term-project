#include <iostream>
#include "Mirror.h"


int main(int argc, char** argv) {
    Mirror* mirror;

    //check for configuration file
    std::string configFileName;
    if (configFileName == "")
    {
        //no file found
        mirror = new Mirror();
    }
    else
    {
        mirror = new Mirror(configFileName);
    }

    mirror->run();

    std::cout << "Have a nice day." << std::endl;

    return 0;
}