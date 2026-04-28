#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

#include "Parameters.h"
#include "algorithms/BucketSort.h"
#include "algorithms/InsertionSort.h"
#include "algorithms/QuickSort.h"
#include "io/DataFile.h"
#include "structures/DoublyLinkedList.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"
#include "utils/SortValidation.h"

namespace SortApplication
{
inline const char *algorithmName(Parameters::Algorithms algorithm)
{
    switch (algorithm)
    {
    case Parameters::Algorithms::bucket:
        return "bucket";
    case Parameters::Algorithms::quick:
        return "quick";
    case Parameters::Algorithms::insertion:
        return "insertion";
    case Parameters::Algorithms::bubble:
        return "bubble";
    case Parameters::Algorithms::cocktail:
        return "cocktail";
    case Parameters::Algorithms::merge:
        return "merge";
    case Parameters::Algorithms::shell:
        return "shell";
    case Parameters::Algorithms::undefined:
    case Parameters::Algorithms::count:
        return "undefined";
    }

    return "unknown";
}

inline const char *structureName(Parameters::Structures structure)
{
    switch (structure)
    {
    case Parameters::Structures::array:
        return "array";
    case Parameters::Structures::singleList:
        return "single_list";
    case Parameters::Structures::doubleList:
        return "double_list";
    case Parameters::Structures::queue:
        return "queue";
    case Parameters::Structures::stack:
        return "stack";
    case Parameters::Structures::binaryTree:
        return "binary_tree";
    case Parameters::Structures::undefined:
    case Parameters::Structures::count:
        return "undefined";
    }

    return "unknown";
}

inline const char *dataTypeName(Parameters::DataTypes dataType)
{
    switch (dataType)
    {
    case Parameters::DataTypes::typeInt:
        return "int";
    case Parameters::DataTypes::typeFloat:
        return "float";
    case Parameters::DataTypes::typeChar:
        return "char";
    case Parameters::DataTypes::tyleUnsignedInt:
        return "unsigned_int";
    case Parameters::DataTypes::typeDouble:
        return "double";
    case Parameters::DataTypes::typeString:
        return "string";
    case Parameters::DataTypes::typeUnsignedLong:
        return "unsigned_long";
    case Parameters::DataTypes::typeUnsignedChar:
        return "unsigned_char";
    case Parameters::DataTypes::undefined:
    case Parameters::DataTypes::count:
        return "undefined";
    }

    return "unknown";
}

inline const char *pivotName(Parameters::Pivots pivot)
{
    switch (pivot)
    {
    case Parameters::Pivots::random:
        return "random";
    case Parameters::Pivots::left:
        return "left";
    case Parameters::Pivots::right:
        return "right";
    case Parameters::Pivots::middle:
        return "middle";
    case Parameters::Pivots::undefined:
        return "middle_default";
    case Parameters::Pivots::count:
        return "undefined";
    }

    return "unknown";
}

inline bool isSupportedAlgorithm()
{
    return Parameters::algorithm == Parameters::Algorithms::bucket ||
           Parameters::algorithm == Parameters::Algorithms::quick ||
           Parameters::algorithm == Parameters::Algorithms::insertion;
}

inline bool isSupportedStructure()
{
    return Parameters::structure == Parameters::Structures::array ||
           Parameters::structure == Parameters::Structures::singleList ||
           Parameters::structure == Parameters::Structures::doubleList;
}

inline bool isSupportedDataType()
{
    return Parameters::dataType == Parameters::DataTypes::typeInt ||
           Parameters::dataType == Parameters::DataTypes::typeFloat ||
           Parameters::dataType == Parameters::DataTypes::typeChar ||
           Parameters::dataType == Parameters::DataTypes::tyleUnsignedInt;
}

inline bool validateParameters()
{
    if (Parameters::runMode != Parameters::RunModes::singleFile &&
        Parameters::runMode != Parameters::RunModes::benchmark)
    {
        std::cerr << "ERROR: choose --singleFile or --benchmark.\n";
        return false;
    }

    if (!isSupportedAlgorithm())
    {
        std::cerr << "ERROR: algorithm '" << algorithmName(Parameters::algorithm)
                  << "' is not implemented for this project scope.\n";
        return false;
    }

    if (!isSupportedStructure())
    {
        std::cerr << "ERROR: structure '" << structureName(Parameters::structure)
                  << "' is not implemented for this project scope.\n";
        return false;
    }

    if (!isSupportedDataType())
    {
        std::cerr << "ERROR: data type '" << dataTypeName(Parameters::dataType)
                  << "' is not supported for this project scope.\n";
        return false;
    }

    if (Parameters::inputFile.empty())
    {
        std::cerr << "ERROR: --inputFile is required for this project runner.\n";
        return false;
    }

    if (Parameters::runMode == Parameters::RunModes::benchmark)
    {
        if (Parameters::iterations <= 0)
        {
            std::cerr << "ERROR: --iterations must be greater than 0 in benchmark mode.\n";
            return false;
        }

        if (Parameters::resultsFile.empty())
        {
            std::cerr << "ERROR: --resultsFile is required in benchmark mode.\n";
            return false;
        }
    }

    return true;
}

inline void printProjectHelpNotes()
{
    std::cout << "\n";
    std::cout << "Project runner notes:\n";
    std::cout << "  Implemented algorithms: 3 - Insertion, 4 - Bucket, 5 - Quick\n";
    std::cout << "  Implemented structures: 0 - Array, 1 - Single linked list, 2 - Double linked list\n";
    std::cout << "  Implemented data types: 0 - int, 1 - float, 3 - char, 5 - unsigned int\n";
    std::cout << "  Benchmark mode requires --inputFile and reuses the same test_data file each iteration.\n";
    std::cout << "  Measured time includes only sorting, not loading, copying, verification or output.\n";
}

template <typename Structure>
bool sortSelectedAlgorithm(Structure &data)
{
    switch (Parameters::algorithm)
    {
    case Parameters::Algorithms::bucket:
    {
        BucketSort<Structure> sorter;
        sorter.sort(data);
        return true;
    }
    case Parameters::Algorithms::quick:
    {
        QuickSort<Structure> sorter(Parameters::pivot);
        sorter.sort(data);
        return true;
    }
    case Parameters::Algorithms::insertion:
    {
        InsertionSort<Structure> sorter;
        sorter.sort(data);
        return true;
    }
    case Parameters::Algorithms::bubble:
    case Parameters::Algorithms::cocktail:
    case Parameters::Algorithms::merge:
    case Parameters::Algorithms::shell:
    case Parameters::Algorithms::undefined:
    case Parameters::Algorithms::count:
        return false;
    }

    return false;
}

inline std::string currentTimestamp()
{
    const std::time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm localTime{};

#if defined(_WIN32)
    localtime_s(&localTime, &currentTime);
#else
    localtime_r(&currentTime, &localTime);
#endif

    char buffer[32]{};
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return std::string(buffer);
}

inline bool isFileEmptyOrMissing(const std::string &filePath)
{
    std::ifstream input(filePath);
    return !input.good() || input.peek() == std::ifstream::traits_type::eof();
}

inline void writeCsvString(std::ofstream &output, const std::string &value)
{
    output << '"';
    for (char character : value)
    {
        if (character == '"')
        {
            output << "\"\"";
        }
        else
        {
            output << character;
        }
    }
    output << '"';
}

inline bool appendBenchmarkResult(
    int dataSize,
    long long minTimeUs,
    double averageTimeUs,
    long long maxTimeUs,
    std::string &errorMessage)
{
    const bool shouldWriteHeader = isFileEmptyOrMissing(Parameters::resultsFile);
    std::ofstream output(Parameters::resultsFile, std::ios::app);
    if (!output.is_open())
    {
        errorMessage = "Cannot open results file: " + Parameters::resultsFile;
        return false;
    }

    if (shouldWriteHeader)
    {
        output << "timestamp,algorithm,structure,data_type,pivot,input_file,size,iterations,min_us,avg_us,max_us,status\n";
    }

    writeCsvString(output, currentTimestamp());
    output << ',';
    writeCsvString(output, algorithmName(Parameters::algorithm));
    output << ',';
    writeCsvString(output, structureName(Parameters::structure));
    output << ',';
    writeCsvString(output, dataTypeName(Parameters::dataType));
    output << ',';
    writeCsvString(output, pivotName(Parameters::pivot));
    output << ',';
    writeCsvString(output, Parameters::inputFile);
    output << ',' << dataSize << ',' << Parameters::iterations << ',' << minTimeUs << ','
           << std::fixed << std::setprecision(2) << averageTimeUs << ',' << maxTimeUs << ',';
    writeCsvString(output, "ok");
    output << '\n';

    if (!output.good())
    {
        errorMessage = "Failed while writing results file: " + Parameters::resultsFile;
        return false;
    }

    return true;
}

template <typename T, typename Structure>
int runSingleFile()
{
    Structure data;
    int loadedSize = 0;
    std::string errorMessage;
    if (!DataFile::load<T>(Parameters::inputFile, data, loadedSize, errorMessage))
    {
        std::cerr << "ERROR: " << errorMessage << '\n';
        return 1;
    }

    const auto startTime = std::chrono::steady_clock::now();
    if (!sortSelectedAlgorithm(data))
    {
        std::cerr << "ERROR: algorithm is not implemented.\n";
        return 1;
    }
    const auto finishTime = std::chrono::steady_clock::now();
    const long long elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

    if (!SortValidation::isSortedAscending(data))
    {
        std::cerr << "ERROR: sorting verification failed.\n";
        return 1;
    }

    if (!Parameters::outputFile.empty() &&
        !DataFile::write<T>(Parameters::outputFile, data, errorMessage))
    {
        std::cerr << "ERROR: " << errorMessage << '\n';
        return 1;
    }

    std::cout << "OK: sorted " << loadedSize << " values in " << elapsedUs << " us.\n";
    if (!Parameters::outputFile.empty())
    {
        std::cout << "Output file: " << Parameters::outputFile << '\n';
    }

    return 0;
}

template <typename T, typename Structure>
int runBenchmark()
{
    Structure originalData;
    int loadedSize = 0;
    std::string errorMessage;
    if (!DataFile::load<T>(Parameters::inputFile, originalData, loadedSize, errorMessage))
    {
        std::cerr << "ERROR: " << errorMessage << '\n';
        return 1;
    }

    if (Parameters::structureSize > 0 && Parameters::structureSize != loadedSize)
    {
        std::cerr << "ERROR: --structureSize does not match input file size.\n";
        return 1;
    }

    long long minTimeUs = std::numeric_limits<long long>::max();
    long long maxTimeUs = 0;
    long long totalTimeUs = 0;

    for (int iteration = 0; iteration < Parameters::iterations; ++iteration)
    {
        Structure data(originalData);

        const auto startTime = std::chrono::steady_clock::now();
        if (!sortSelectedAlgorithm(data))
        {
            std::cerr << "ERROR: algorithm is not implemented.\n";
            return 1;
        }
        const auto finishTime = std::chrono::steady_clock::now();
        const long long elapsedUs = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime).count();

        if (!SortValidation::isSortedAscending(data))
        {
            std::cerr << "ERROR: sorting verification failed on iteration " << iteration + 1 << ".\n";
            return 1;
        }

        if (elapsedUs < minTimeUs)
        {
            minTimeUs = elapsedUs;
        }
        if (elapsedUs > maxTimeUs)
        {
            maxTimeUs = elapsedUs;
        }
        totalTimeUs += elapsedUs;
    }

    const double averageTimeUs = static_cast<double>(totalTimeUs) / static_cast<double>(Parameters::iterations);
    if (!appendBenchmarkResult(loadedSize, minTimeUs, averageTimeUs, maxTimeUs, errorMessage))
    {
        std::cerr << "ERROR: " << errorMessage << '\n';
        return 1;
    }

    std::cout << "OK: benchmark completed. size=" << loadedSize
              << ", iterations=" << Parameters::iterations
              << ", min=" << minTimeUs
              << " us, avg=" << std::fixed << std::setprecision(2) << averageTimeUs
              << " us, max=" << maxTimeUs << " us.\n";

    return 0;
}

template <typename T, typename Structure>
int runSelectedMode()
{
    if (Parameters::runMode == Parameters::RunModes::singleFile)
    {
        return runSingleFile<T, Structure>();
    }

    return runBenchmark<T, Structure>();
}

template <typename T>
int runForStructure()
{
    switch (Parameters::structure)
    {
    case Parameters::Structures::array:
        return runSelectedMode<T, DynamicArray<T>>();
    case Parameters::Structures::singleList:
        return runSelectedMode<T, SinglyLinkedList<T>>();
    case Parameters::Structures::doubleList:
        return runSelectedMode<T, DoublyLinkedList<T>>();
    case Parameters::Structures::queue:
    case Parameters::Structures::stack:
    case Parameters::Structures::binaryTree:
    case Parameters::Structures::undefined:
    case Parameters::Structures::count:
        std::cerr << "ERROR: structure is not implemented.\n";
        return 1;
    }

    std::cerr << "ERROR: structure is not implemented.\n";
    return 1;
}

inline int runForDataType()
{
    switch (Parameters::dataType)
    {
    case Parameters::DataTypes::typeInt:
        return runForStructure<int>();
    case Parameters::DataTypes::typeFloat:
        return runForStructure<float>();
    case Parameters::DataTypes::typeChar:
        return runForStructure<char>();
    case Parameters::DataTypes::tyleUnsignedInt:
        return runForStructure<unsigned int>();
    case Parameters::DataTypes::typeDouble:
    case Parameters::DataTypes::typeString:
    case Parameters::DataTypes::typeUnsignedLong:
    case Parameters::DataTypes::typeUnsignedChar:
    case Parameters::DataTypes::undefined:
    case Parameters::DataTypes::count:
        std::cerr << "ERROR: data type is not supported.\n";
        return 1;
    }

    std::cerr << "ERROR: data type is not supported.\n";
    return 1;
}

inline int run()
{
    if (!validateParameters())
    {
        return 1;
    }

    return runForDataType();
}
}
