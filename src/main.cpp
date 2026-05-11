#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

#include "Parameters.h"
#include "algorithms/CocktailSort.h"
#include "algorithms/InsertionSort.h"
#include "algorithms/MergeSort.h"
#include "benchmark/DataGenerator.h"
#include "io/DataFile.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "utils/SortValidation.h"

namespace
{
    // Pomocnicze funkcje do zamiany enumow na tekst w raporcie CSV.
    const char *algorithmToString(Parameters::Algorithms algorithm)
    {
        switch (algorithm)
        {
        case Parameters::Algorithms::cocktail:
            return "cocktail";
        case Parameters::Algorithms::merge:
            return "merge";
        case Parameters::Algorithms::insertion:
            return "insertion";
        case Parameters::Algorithms::bubble:
            return "bubble";
        case Parameters::Algorithms::bucket:
            return "bucket";
        case Parameters::Algorithms::quick:
            return "quick";
        case Parameters::Algorithms::shell:
            return "shell";
        default:
            return "unknown";
        }
    }

    const char *structureToString(Parameters::Structures structure)
    {
        switch (structure)
        {
        case Parameters::Structures::array:
            return "array";
        case Parameters::Structures::singleList:
            return "singleList";
        case Parameters::Structures::doubleList:
            return "doubleList";
        case Parameters::Structures::queue:
            return "queue";
        case Parameters::Structures::stack:
            return "stack";
        case Parameters::Structures::binaryTree:
            return "binaryTree";
        default:
            return "unknown";
        }
    }

    const char *dataTypeToString(Parameters::DataTypes dataType)
    {
        switch (dataType)
        {
        case Parameters::DataTypes::typeInt:
            return "int";
        case Parameters::DataTypes::typeFloat:
            return "float";
        case Parameters::DataTypes::typeDouble:
            return "double";
        case Parameters::DataTypes::typeChar:
            return "char";
        case Parameters::DataTypes::typeString:
            return "string";
        case Parameters::DataTypes::tyleUnsignedInt:
            return "unsigned_int";
        case Parameters::DataTypes::typeUnsignedLong:
            return "unsigned_long";
        case Parameters::DataTypes::typeUnsignedChar:
            return "unsigned_char";
        default:
            return "unknown";
        }
    }

    const char *distributionToString(Parameters::Distribution distribution)
    {
        switch (distribution)
        {
        case Parameters::Distribution::random:
            return "random";
        case Parameters::Distribution::ascending:
            return "ascending";
        case Parameters::Distribution::ascending50Per:
            return "ascending50Per";
        case Parameters::Distribution::descending:
            return "descending";
        default:
            return "unknown";
        }
    }

    std::string getCurrentTimestamp()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm *now_tm = std::localtime(&now_time);
        char buffer[100];
        if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm))
        {
            return std::string(buffer);
        }
        return "unknown-time";
    }

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

    int failPositiveRequired(const char *parameterName)
    {
        std::cerr << "ERROR: parameter " << parameterName << " must be greater than 0.\n";
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

        if (Parameters::structureSize <= 0)
        {
            return failPositiveRequired("--structureSize");
        }

        if (Parameters::iterations <= 0)
        {
            return failPositiveRequired("--iterations");
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

    template <typename T, typename Structure>
    int sortSingleFileValues(Structure &values)
    {
        switch (Parameters::algorithm)
        {
        case Parameters::Algorithms::cocktail:
            CocktailSort::sort(values);
            return 0;
        case Parameters::Algorithms::merge:
            // Merge sort ma osobne wersje dla tablicy i listy, bo lista nie powinna byc sortowana przez values[index].
            MergeSort::sort(values);
            return 0;
        case Parameters::Algorithms::insertion:
            InsertionSort::sort(values);
            return 0;
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

        if (sortSingleFileValues<T>(values) != 0)
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

    template <typename T, typename Structure>
    int runBenchmarkSort(Structure &values)
    {
        // Wybieramy algorytm do pomiaru.
        switch (Parameters::algorithm)
        {
        case Parameters::Algorithms::cocktail:
            CocktailSort::sort(values);
            return 0;
        case Parameters::Algorithms::merge:
            MergeSort::sort(values);
            return 0;
        case Parameters::Algorithms::insertion:
            InsertionSort::sort(values);
            return 0;
        default:
            return 1;
        }
    }

    int writeBenchmarkCsv(const std::string &path, const DynamicArray<long long> &durations, long long min_us, long long max_us, double avg_us)
    {
        bool fileExists = false;
        {
            std::ifstream test(path);
            fileExists = test.good();
        }

        std::ofstream file(path, std::ios::app);
        if (!file.is_open())
        {
            std::cerr << "ERROR: could not open results file " << path << " for writing.\n";
            return 1;
        }

        // Naglowek z dodatkowymi kolumnami na dane kazdej iteracji.
        if (!fileExists)
        {
            file << "timestamp,algorithm,structure,data_type,distribution,size,iterations,iteration_id,duration_us,min_us,avg_us,max_us,status\n";
        }

        std::string timestamp = getCurrentTimestamp();

        // Zapisujemy osobny wiersz dla kazdej iteracji, powtarzajac statystyki zbiorcze.
        for (int i = 0; i < durations.size(); i++)
        {
            file << timestamp << ",";
            file << algorithmToString(Parameters::algorithm) << ",";
            file << structureToString(Parameters::structure) << ",";
            file << dataTypeToString(Parameters::dataType) << ",";
            file << distributionToString(Parameters::distribution) << ",";
            file << Parameters::structureSize << ",";
            file << Parameters::iterations << ",";
            file << (i + 1) << ","; // iteration_id
            file << durations[i] << ","; // duration_us
            file << min_us << ",";
            file << std::fixed << std::setprecision(2) << avg_us << ",";
            file << max_us << ",";
            file << "OK\n";
        }

        return 0;
    }

    template <typename T, typename Structure>
    int executeBenchmarkLoop(std::mt19937 &rng)
    {
        // Przechowujemy czasy kazdej iteracji w naszej wlasnej tablicy dynamicznej.
        DynamicArray<long long> durations;
        long long total_us = 0;

        for (int iteration = 0; iteration < Parameters::iterations; iteration++)
        {
            Structure values;
            // Dla tablicy rezerwujemy miejsce od razu, zeby uniknac realokacji w trakcie generowania.
            if constexpr (std::is_same_v<Structure, DynamicArray<T>>)
            {
                values = DynamicArray<T>(Parameters::structureSize);
            }

            DataGenerator::fill(values, Parameters::structureSize, Parameters::distribution, rng);

            // Przywracam weryfikacje rozmiaru po generowaniu danych.
            if (values.size() != Parameters::structureSize)
            {
                std::cerr << "ERROR: benchmark generator created structure with invalid size.\n";
                return 1;
            }

            auto start = std::chrono::steady_clock::now();
            if (runBenchmarkSort<T, Structure>(values) != 0)
            {
                std::cerr << "ERROR: sorting failed during benchmark iteration " << iteration << ".\n";
                return 1;
            }
            auto end = std::chrono::steady_clock::now();

            // Weryfikacja nie jest mierzona, ale chroni przed blednymi wynikami w raporcie.
            if (!isSortedAscending(values))
            {
                std::cerr << "ERROR: sorting verification failed during benchmark iteration " << iteration << ".\n";
                return 1;
            }

            long long duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            durations.pushBack(duration);
            total_us += duration;
        }

        long long min_us = durations[0];
        long long max_us = durations[0];
        for (int i = 0; i < durations.size(); i++)
        {
            if (durations[i] < min_us) min_us = durations[i];
            if (durations[i] > max_us) max_us = durations[i];
        }
        double avg_us = static_cast<double>(total_us) / Parameters::iterations;

        std::cout << "Benchmark results (" << Parameters::iterations << " iterations):\n";
        std::cout << "  min: " << min_us << " us\n";
        std::cout << "  avg: " << std::fixed << std::setprecision(2) << avg_us << " us\n";
        std::cout << "  max: " << max_us << " us\n";

        if (!Parameters::resultsFile.empty())
        {
            return writeBenchmarkCsv(Parameters::resultsFile, durations, min_us, max_us, avg_us);
        }

        return 0;
    }

    template <typename T>
    int runBenchmarkForType(std::mt19937 &rng)
    {
        // W benchmarku tworzymy dane bez plikow, zeby pozniej mierzyc tylko czas sortowania.
        switch (Parameters::structure)
        {
        case Parameters::Structures::array:
            return executeBenchmarkLoop<T, DynamicArray<T>>(rng);
        case Parameters::Structures::singleList:
            return executeBenchmarkLoop<T, SinglyLinkedList<T>>(rng);
        default:
            std::cerr << "ERROR: unsupported structure.\n";
            return 1;
        }
    }

    int runBenchmark()
    {
        // Seed laczy zrodlo systemowe i czas, zeby kolejne uruchomienia benchmarku mialy inne dane.
        std::random_device randomDevice;
        unsigned long long now = static_cast<unsigned long long>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count());
        std::seed_seq seed{
            static_cast<unsigned int>(randomDevice()),
            static_cast<unsigned int>(randomDevice()),
            static_cast<unsigned int>(now),
            static_cast<unsigned int>(now >> 32)};
        std::mt19937 rng(seed);

        switch (Parameters::dataType)
        {
        case Parameters::DataTypes::typeInt:
            return runBenchmarkForType<int>(rng);
        case Parameters::DataTypes::typeFloat:
            return runBenchmarkForType<float>(rng);
        case Parameters::DataTypes::typeDouble:
            return runBenchmarkForType<double>(rng);
        case Parameters::DataTypes::typeChar:
            return runBenchmarkForType<char>(rng);
        case Parameters::DataTypes::typeString:
            return runBenchmarkForType<std::string>(rng);
        case Parameters::DataTypes::tyleUnsignedInt:
            return runBenchmarkForType<unsigned int>(rng);
        case Parameters::DataTypes::typeUnsignedLong:
            return runBenchmarkForType<unsigned long>(rng);
        case Parameters::DataTypes::typeUnsignedChar:
            return runBenchmarkForType<unsigned char>(rng);
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
            return runBenchmark();
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
