#pragma once

#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <type_traits>

namespace DataFile
{
inline void stripTrailingCarriageReturn(std::string &line)
{
    if (!line.empty() && line[line.size() - 1] == '\r')
    {
        line.erase(line.size() - 1);
    }
}

template <typename T>
bool parseValue(const std::string &line, T &value)
{
    std::istringstream input(line);
    input >> value;
    return !input.fail();
}

template <>
inline bool parseValue<char>(const std::string &line, char &value)
{
    if (line.empty())
    {
        return false;
    }

    value = line[0];
    return true;
}

template <typename T, typename Structure>
bool load(const std::string &filePath, Structure &data, int &loadedSize, std::string &errorMessage)
{
    std::ifstream input(filePath);
    if (!input.is_open())
    {
        errorMessage = "Cannot open input file: " + filePath;
        return false;
    }

    std::string line;
    if (!std::getline(input, line))
    {
        errorMessage = "Input file is empty: " + filePath;
        return false;
    }

    stripTrailingCarriageReturn(line);
    std::istringstream sizeInput(line);
    sizeInput >> loadedSize;
    if (sizeInput.fail() || loadedSize < 0)
    {
        errorMessage = "First input line must be a non-negative size.";
        return false;
    }

    for (int index = 0; index < loadedSize; ++index)
    {
        if (!std::getline(input, line))
        {
            errorMessage = "Input file contains fewer values than declared.";
            return false;
        }

        stripTrailingCarriageReturn(line);
        T value{};
        if (!parseValue<T>(line, value))
        {
            errorMessage = "Cannot parse value on data line " + std::to_string(index + 2) + ".";
            return false;
        }

        data.pushBack(value);
    }

    return true;
}

template <typename T, typename Structure>
bool write(const std::string &filePath, const Structure &data, std::string &errorMessage)
{
    std::ofstream output(filePath);
    if (!output.is_open())
    {
        errorMessage = "Cannot open output file: " + filePath;
        return false;
    }

    if constexpr (std::is_floating_point<T>::value)
    {
        output << std::setprecision(std::numeric_limits<T>::max_digits10);
    }

    const int dataSize = data.size();
    output << dataSize << '\n';
    for (int index = 0; index < dataSize; ++index)
    {
        output << data[index] << '\n';
    }

    if (!output.good())
    {
        errorMessage = "Failed while writing output file: " + filePath;
        return false;
    }

    return true;
}
}
