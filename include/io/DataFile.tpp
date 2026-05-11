#pragma once

#include <fstream>
#include <limits>
#include <sstream>
#include <type_traits>

namespace DataFile
{
namespace Detail
{
bool parseIntToken(const std::string &token, int &value)
{
    std::istringstream stream(token);
    stream >> value;
    return !stream.fail() && stream.eof();
}

bool parseUnsignedIntToken(const std::string &token, unsigned int &value)
{
    if (!token.empty() && token[0] == '-')
    {
        return false;
    }

    std::istringstream stream(token);
    stream >> value;
    return !stream.fail() && stream.eof();
}

template <typename T>
bool readValue(std::istream &input, T &value)
{
    if constexpr (std::is_same<T, char>::value)
    {
        std::string token;
        if (!(input >> token))
        {
            return false;
        }

        if (token.size() == 1)
        {
            value = token[0];
            return true;
        }

        int number = 0;
        if (!parseIntToken(token, number) ||
            number < static_cast<int>(std::numeric_limits<char>::min()) ||
            number > static_cast<int>(std::numeric_limits<char>::max()))
        {
            return false;
        }

        value = static_cast<char>(number);
        return true;
    }
    else if constexpr (std::is_same<T, unsigned char>::value)
    {
        std::string token;
        if (!(input >> token))
        {
            return false;
        }

        if (token.size() == 1)
        {
            value = static_cast<unsigned char>(token[0]);
            return true;
        }

        unsigned int number = 0;
        if (!parseUnsignedIntToken(token, number) ||
            number > static_cast<unsigned int>(std::numeric_limits<unsigned char>::max()))
        {
            return false;
        }

        value = static_cast<unsigned char>(number);
        return true;
    }
    else
    {
        input >> value;
        return !input.fail();
    }
}

template <typename T>
void writeValue(std::ostream &output, const T &value)
{
    if constexpr (std::is_same<T, unsigned char>::value)
    {
        output << static_cast<unsigned int>(value);
    }
    else
    {
        output << value;
    }
}
}

template <typename T>
bool readValues(const std::string &path, DynamicArray<T> &values, std::string &error)
{
    std::ifstream input(path);
    if (!input.is_open())
    {
        error = "Cannot open input file: " + path;
        return false;
    }

    int count = 0;
    input >> count;
    if (input.fail() || count < 0)
    {
        error = "Input file has invalid element count.";
        return false;
    }

    DynamicArray<T> loaded(count);
    for (int i = 0; i < count; i++)
    {
        if (!Detail::readValue(input, loaded[i]))
        {
            error = "Input file ended early or contains an invalid value.";
            return false;
        }
    }

    values = loaded;
    error.clear();
    return true;
}

template <typename T>
bool writeValues(const std::string &path, const IStructure<T> &values, std::string &error)
{
    std::ofstream output(path);
    if (!output.is_open())
    {
        error = "Cannot open output file: " + path;
        return false;
    }

    output << values.size() << '\n';
    for (int i = 0; i < values.size(); i++)
    {
        Detail::writeValue(output, values[i]);
        output << '\n';
    }

    if (output.fail())
    {
        error = "Failed while writing output file: " + path;
        return false;
    }

    error.clear();
    return true;
}
}
