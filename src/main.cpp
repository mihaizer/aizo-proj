#include <iostream>

#include "Parameters.h"

namespace
{
bool isGrade3Algorithm(Parameters::Algorithms algorithm)
{
    switch (algorithm)
    {
    case Parameters::Algorithms::cocktail:
    case Parameters::Algorithms::merge:
    case Parameters::Algorithms::insertion:
        return true;
    default:
        return false;
    }
}

bool isGrade3Structure(Parameters::Structures structure)
{
    switch (structure)
    {
    case Parameters::Structures::array:
    case Parameters::Structures::singleList:
        return true;
    default:
        return false;
    }
}

bool isGrade3DataType(Parameters::DataTypes dataType)
{
    switch (dataType)
    {
    case Parameters::DataTypes::typeInt:
    case Parameters::DataTypes::typeFloat:
    case Parameters::DataTypes::tyleUnsignedInt:
        return true;
    default:
        return false;
    }
}

bool isKnownDistribution(Parameters::Distribution distribution)
{
    switch (distribution)
    {
    case Parameters::Distribution::random:
    case Parameters::Distribution::ascending:
    case Parameters::Distribution::ascending50Per:
    case Parameters::Distribution::descending:
        return true;
    default:
        return false;
    }
}

void printProjectHelpNotes()
{
    std::cout << "\nProject scope for grade 3.0:\n";
    std::cout << "  algorithms: 1 - Cocktail, 2 - Merge, 3 - Insertion\n";
    std::cout << "  structures: 0 - Array, 1 - Single linked list\n";
    std::cout << "  data types: 0 - int, 1 - float, 5 - unsigned int\n";
    std::cout << "  benchmark distributions: 0 - random, 1 - ascending, 2 - ascending 50%, 3 - descending\n";
}

int failMissing(const char *parameterName)
{
    std::cerr << "ERROR: missing required parameter " << parameterName << ".\n";
    return 1;
}

int validateRunMode()
{
    switch (Parameters::runMode)
    {
    case Parameters::RunModes::singleFile:
    case Parameters::RunModes::benchmark:
        return 0;
    case Parameters::RunModes::undefined:
        return failMissing("--singleFile, --benchmark or --help");
    case Parameters::RunModes::help:
        return 0;
    default:
        std::cerr << "ERROR: unsupported run mode.\n";
        return 1;
    }
}

int validateCommonGrade3Parameters()
{
    if (Parameters::algorithm == Parameters::Algorithms::undefined)
    {
        return failMissing("--algorithm");
    }

    if (!isGrade3Algorithm(Parameters::algorithm))
    {
        std::cerr << "ERROR: selected algorithm is not implemented for grade 3.0 scope.\n";
        std::cerr << "Supported algorithms: 1 - Cocktail, 2 - Merge, 3 - Insertion.\n";
        return 1;
    }

    if (Parameters::structure == Parameters::Structures::undefined)
    {
        return failMissing("--structure");
    }

    if (!isGrade3Structure(Parameters::structure))
    {
        std::cerr << "ERROR: selected structure is not implemented for grade 3.0 scope.\n";
        std::cerr << "Supported structures: 0 - Array, 1 - Single linked list.\n";
        return 1;
    }

    if (Parameters::dataType == Parameters::DataTypes::undefined)
    {
        return failMissing("--dataType");
    }

    if (!isGrade3DataType(Parameters::dataType))
    {
        std::cerr << "ERROR: selected data type is not implemented for grade 3.0 scope.\n";
        std::cerr << "Supported data types: 0 - int, 1 - float, 5 - unsigned int.\n";
        return 1;
    }

    if (Parameters::pivot != Parameters::Pivots::undefined)
    {
        std::cerr << "ERROR: --pivot is outside grade 3.0 scope because quicksort is not required.\n";
        return 1;
    }

    if (Parameters::shellParameter != Parameters::ShellParameters::undefined)
    {
        std::cerr << "ERROR: --shellParameter is outside grade 3.0 scope because shell sort is not required.\n";
        return 1;
    }

    return 0;
}

int validateBenchmarkParameters()
{
    if (Parameters::runMode != Parameters::RunModes::benchmark)
    {
        return 0;
    }

    if (Parameters::distribution == Parameters::Distribution::undefined)
    {
        return failMissing("--distribution");
    }

    if (!isKnownDistribution(Parameters::distribution))
    {
        std::cerr << "ERROR: selected distribution is not a real benchmark distribution.\n";
        std::cerr << "Supported distributions: 0 - random, 1 - ascending, 2 - ascending 50%, 3 - descending.\n";
        return 1;
    }

    return 0;
}

int validateParameters()
{
    int runModeResult = validateRunMode();
    if (runModeResult != 0)
    {
        return runModeResult;
    }

    int commonResult = validateCommonGrade3Parameters();
    if (commonResult != 0)
    {
        return commonResult;
    }

    return validateBenchmarkParameters();
}

void printValidatedSkeletonMessage()
{
    std::cout << "Library version: " << Parameters::getVersion() << "\n";
    std::cout << "Parameters are valid for grade 3.0 scope.\n";

    if (Parameters::runMode == Parameters::RunModes::singleFile)
    {
        std::cout << "singleFile runner skeleton is ready. File IO and sorting will be added in the next PRs.\n";
    }
    else if (Parameters::runMode == Parameters::RunModes::benchmark)
    {
        std::cout << "benchmark runner skeleton is ready. Data generation and CSV results will be added in later PRs.\n";
    }
}
}

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        std::cerr << "ERROR: no arguments provided.\n";
        Parameters::help();
        printProjectHelpNotes();
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
        printProjectHelpNotes();
        return 0;
    }

    int validationResult = validateParameters();
    if (validationResult != 0)
    {
        return validationResult;
    }

    printValidatedSkeletonMessage();
    return 0;
}
