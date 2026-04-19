#include <iostream>

#include "Project/Application.h"
#include "Project/CmdVarPass.h"

int main(int argc, char* argv[])
{
    if(argc>1)
    {
        int number = std::stoi(argv[1]);
        CmdVarPass::sideLength = number;
    }

    Application application;

    application.Gameloop();
    
    return 0;
}
