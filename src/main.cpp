#include <iostream>

#include "Parameters.h"
#include "app/SortApplication.h"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        std::cerr << "ERROR: no arguments provided.\n";
        Parameters::help();
        SortApplication::printProjectHelpNotes();
        return 1;
    }

    if (Parameters::readParameters(argc - 1, argv + 1) != 0)
    {
        std::cerr << "ERROR: failed to parse parameters.\n";
        return 1;
    }

    if (Parameters::runMode == Parameters::RunModes::help)
    {
        Parameters::help();
        SortApplication::printProjectHelpNotes();
        return 0;
    }

    return SortApplication::run();
}
