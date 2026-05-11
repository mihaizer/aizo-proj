#include <iostream>
#include <string>

#include "Parameters.h"
#include "algorithms/CocktailSort.h"
#include "algorithms/InsertionSort.h"
#include "io/DataFile.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "utils/SortValidation.h"

namespace
{
    // Program obsluguje wybrany zakres projektu: sortowanie koktajlowe, przez scalanie i przez wstawianie;
    // tablice oraz listy jednokierunkowe. Typ danych przyjmujemy zgodnie z lista z Parameters::help().
    bool isSupportedAlgorithm(Parameters::Algorithms algorithm)
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

    bool isSupportedStructure(Parameters::Structures structure)
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

    bool isSupportedDataType(Parameters::DataTypes dataType)
    {
        switch (dataType)
        {
        case Parameters::DataTypes::typeInt:
        case Parameters::DataTypes::typeFloat:
        case Parameters::DataTypes::typeDouble:
        case Parameters::DataTypes::typeChar:
        case Parameters::DataTypes::typeString:
        case Parameters::DataTypes::tyleUnsignedInt:
        case Parameters::DataTypes::typeUnsignedLong:
        case Parameters::DataTypes::typeUnsignedChar:
            return true;
        default:
            return false;
        }
    }

    // Distribution::count jest tylko granica enuma; prawdziwe rozklady to cztery wartosci z helpa.
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

    // Help z biblioteki pokazuje wszystkie opcje, wiec dopisujemy zakres faktycznie realizowany tutaj.
    void printProjectHelpNotes()
    {
        std::cout << "\nProject scope:\n";
        std::cout << "  algorithms: 1 - Cocktail, 2 - Merge, 3 - Insertion\n";
        std::cout << "  structures: 0 - Array, 1 - Single linked list\n";
        std::cout << "  data types: 0 - int, 1 - float, 2 - double, 3 - char, 4 - std::string\n";
        std::cout << "              5 - unsigned int, 6 - unsigned long, 7 - unsigned char\n";
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

    // Ta walidacja zatrzymuje opcje spoza zakresu zanim program zacznie tworzyc struktury
    // albo uruchamiac algorytmy. Sama biblioteka sprawdza tylko format argumentow i granice enumow.
    int validateCommonParameters()
    {
        if (Parameters::algorithm == Parameters::Algorithms::undefined)
        {
            return failMissing("--algorithm");
        }

        if (!isSupportedAlgorithm(Parameters::algorithm))
        {
            std::cerr << "ERROR: selected algorithm is not implemented in this project scope.\n";
            std::cerr << "Supported algorithms: 1 - Cocktail, 2 - Merge, 3 - Insertion.\n";
            return 1;
        }

        if (Parameters::structure == Parameters::Structures::undefined)
        {
            return failMissing("--structure");
        }

        if (!isSupportedStructure(Parameters::structure))
        {
            std::cerr << "ERROR: selected structure is not implemented in this project scope.\n";
            std::cerr << "Supported structures: 0 - Array, 1 - Single linked list.\n";
            return 1;
        }

        if (Parameters::dataType == Parameters::DataTypes::undefined)
        {
            return failMissing("--dataType");
        }

        if (!isSupportedDataType(Parameters::dataType))
        {
            std::cerr << "ERROR: selected data type is not implemented in this project scope.\n";
            std::cerr << "Supported data types: 0 - int, 1 - float, 2 - double, 3 - char, 4 - std::string, ";
            std::cerr << "5 - unsigned int, 6 - unsigned long, 7 - unsigned char.\n";
            return 1;
        }

        if (Parameters::pivot != Parameters::Pivots::undefined)
        {
            std::cerr << "ERROR: --pivot is not used because quicksort is outside this project scope.\n";
            return 1;
        }

        if (Parameters::shellParameter != Parameters::ShellParameters::undefined)
        {
            std::cerr << "ERROR: --shellParameter is not used because shell sort is outside this project scope.\n";
            return 1;
        }

        return 0;
    }

    int validateSingleFileParameters()
    {
        // Ta kontrola dotyczy tylko trybu singleFile; benchmark nie korzysta z pliku wejsciowego jako zrodla danych.
        if (Parameters::runMode != Parameters::RunModes::singleFile)
        {
            return 0;
        }

        // Bez inputFile nie da sie wykonac pojedynczego testu, bo dane maja pochodzic z pliku w formacie zadania.
        if (Parameters::inputFile.empty())
        {
            return failMissing("--inputFile");
        }

        return 0;
    }

    // Rozklad danych ma sens tylko w benchmarku; w trybie singleFile kolejnosc danych wynika z pliku.
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

        int commonResult = validateCommonParameters();
        if (commonResult != 0)
        {
            return commonResult;
        }

        // Po parametrach wspolnych sprawdzamy wymagania zalezne od trybu pracy.
        // Dla singleFile jest to przede wszystkim obecnosc pliku wejsciowego.
        int singleFileResult = validateSingleFileParameters();
        if (singleFileResult != 0)
        {
            return singleFileResult;
        }

        return validateBenchmarkParameters();
    }

    template <typename T>
    int sortSingleFileValues(IStructure<T> &values)
    {
        switch (Parameters::algorithm)
        {
        case Parameters::Algorithms::cocktail:
            CocktailSort::sort(values);
            return 0;
        case Parameters::Algorithms::insertion:
            InsertionSort::sort(values);
            return 0;
        case Parameters::Algorithms::merge:
            std::cerr << "ERROR: selected algorithm is not connected to singleFile sorting yet.\n";
            std::cerr << "Implemented now: 1 - Cocktail sort, 3 - Insertion sort.\n";
            return 1;
        case Parameters::Algorithms::bubble:
        case Parameters::Algorithms::bucket:
        case Parameters::Algorithms::quick:
        case Parameters::Algorithms::shell:
        case Parameters::Algorithms::undefined:
        case Parameters::Algorithms::count:
            std::cerr << "ERROR: unsupported algorithm.\n";
            return 1;
        }

        std::cerr << "ERROR: unsupported algorithm.\n";
        return 1;
    }

    template <typename T, typename Structure>
    int runSingleFileForStructure()
    {
        // Typ T jest wybierany na podstawie Parameters::dataType, a sam odczyt korzysta z tego samego formatu:
        // pierwsza liczba oznacza rozmiar, kolejne tokeny to elementy struktury.
        Structure values;
        std::string error;
        if (!DataFile::readValues<T>(Parameters::inputFile, values, error))
        {
            std::cerr << "ERROR: " << error << "\n";
            return 1;
        }

        std::cout << "Loaded " << values.size() << " values from " << Parameters::inputFile << ".\n";

        if (sortSingleFileValues(values) != 0)
        {
            return 1;
        }

        // Po sortowaniu sprawdzamy wynik wlasna funkcja, bez uzywania gotowych algorytmow sortujacych.
        if (!isSortedAscending(values))
        {
            std::cerr << "ERROR: sorting verification failed.\n";
            return 1;
        }

        // Plik wyjsciowy jest opcjonalny w singleFile, wiec brak --outputFile nie jest bledem.
        if (Parameters::outputFile.empty())
        {
            std::cout << "Data was sorted ascending. No output file was requested.\n";
            return 0;
        }

        // Zapisujemy ten sam format, ktory byl wymagany na wejsciu: rozmiar i wartosci w kolejnych liniach.
        if (!DataFile::writeValues(Parameters::outputFile, values, error))
        {
            std::cerr << "ERROR: " << error << "\n";
            return 1;
        }

        std::cout << "Data was sorted ascending and written to " << Parameters::outputFile << ".\n";
        return 0;
    }

    template <typename T>
    int runSingleFileForType()
    {
        // Po wyborze typu danych wybieramy konkretna strukture wskazana przez Parameters::structure.
        switch (Parameters::structure)
        {
        case Parameters::Structures::array:
            return runSingleFileForStructure<T, DynamicArray<T>>();
        case Parameters::Structures::singleList:
            return runSingleFileForStructure<T, SinglyLinkedList<T>>();
        case Parameters::Structures::doubleList:
        case Parameters::Structures::queue:
        case Parameters::Structures::stack:
        case Parameters::Structures::binaryTree:
        case Parameters::Structures::undefined:
        case Parameters::Structures::count:
            std::cerr << "ERROR: unsupported structure.\n";
            return 1;
        }

        std::cerr << "ERROR: unsupported structure.\n";
        return 1;
    }

    int runSingleFile()
    {
        // Biblioteka Parameters zapisuje typ jako enum, a tutaj wybieramy konkretna instancje szablonu.
        switch (Parameters::dataType)
        {
        case Parameters::DataTypes::typeInt:
            return runSingleFileForType<int>();
        case Parameters::DataTypes::typeFloat:
            return runSingleFileForType<float>();
        case Parameters::DataTypes::typeDouble:
            return runSingleFileForType<double>();
        case Parameters::DataTypes::typeChar:
            return runSingleFileForType<char>();
        case Parameters::DataTypes::typeString:
            return runSingleFileForType<std::string>();
        case Parameters::DataTypes::tyleUnsignedInt:
            return runSingleFileForType<unsigned int>();
        case Parameters::DataTypes::typeUnsignedLong:
            return runSingleFileForType<unsigned long>();
        case Parameters::DataTypes::typeUnsignedChar:
            return runSingleFileForType<unsigned char>();
        default:
            std::cerr << "ERROR: unsupported data type.\n";
            return 1;
        }
    }

    int runValidatedMode()
    {
        std::cout << "Library version: " << Parameters::getVersion() << "\n";
        std::cout << "Parameters are valid for the current project scope.\n";

        // Po walidacji trybu mozna przejsc do wlasciwej sciezki wykonania.
        if (Parameters::runMode == Parameters::RunModes::singleFile)
        {
            return runSingleFile();
        }

        if (Parameters::runMode == Parameters::RunModes::benchmark)
        {
            std::cout << "benchmark mode selected. Data generation and CSV results are not implemented yet.\n";
            return 0;
        }

        std::cerr << "ERROR: unsupported run mode.\n";
        return 1;
    }
}

int main(int argc, char **argv)
{
    // Brak argumentow traktujemy jako blad uruchomienia, ale od razu pokazujemy poprawny format polecenia.
    if (argc <= 1)
    {
        std::cerr << "ERROR: no arguments provided.\n";
        Parameters::help();
        printProjectHelpNotes();
        return 1;
    }

    // argv[0] to nazwa programu, dlatego do readParameters przekazujemy tylko argumenty uzytkownika.
    if (Parameters::readParameters(argc - 1, argv + 1) != 0)
    {
        std::cerr << "ERROR: failed to parse parameters.\n";
        return 1;
    }

    // Help nie wymaga algorytmu, struktury ani typu danych, wiec konczy program przed walidacja.
    if (Parameters::runMode == Parameters::RunModes::help)
    {
        Parameters::help();
        printProjectHelpNotes();
        return 0;
    }

    // Po parsowaniu sprawdzamy juz wymagania projektu, ktorych biblioteka Parameters nie zna.
    int validationResult = validateParameters();
    if (validationResult != 0)
    {
        return validationResult;
    }

    return runValidatedMode();
}
