#pragma once

#include <fstream>
#include <limits>
#include <sstream>
#include <type_traits>

namespace DataFile
{
namespace Detail
{
// Pomocnicze parsowanie dla char: token moze byc znakiem albo liczba z zakresu typu char.
bool parseIntToken(const std::string &token, int &value)
{
    std::istringstream stream(token);
    stream >> value;
    return !stream.fail() && stream.eof();
}

// Unsigned char nie powinien przyjmowac wartosci ujemnych zapisanych tekstowo.
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
    // Char traktujemy osobno, bo operator>> dla char czyta pojedynczy znak, a pliki moga zawierac tez kod liczbowy.
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
    // Unsigned char tez obslugujemy recznie, zeby przy zapisie i odczycie nie pomylic go z tekstowym znakiem.
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
        // Dla liczb i std::string wystarcza standardowy operator strumieniowy.
        input >> value;
        return !input.fail();
    }
}

template <typename T>
void writeValue(std::ostream &output, const T &value)
{
    // Unsigned char zapisujemy jako liczbe, bo bez rzutowania strumien potraktowalby go jak znak.
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

template <typename T, typename Structure>
bool readValues(const std::string &path, Structure &values, std::string &error)
{
    // Odczytujemy caly plik do tymczasowej struktury. Dopiero poprawny odczyt podmienia wynik.
    std::ifstream input(path);
    if (!input.is_open())
    {
        error = "Cannot open input file: " + path;
        return false;
    }

    // Pierwsza wartosc okresla rozmiar struktury, zgodnie z formatem z PDF.
    int count = 0;
    input >> count;
    if (input.fail() || count < 0)
    {
        error = "Input file has invalid element count.";
        return false;
    }

    // Dane dodajemy przez pushBack, wiec ta funkcja nie zalezy od konkretnej implementacji struktury.
    Structure loaded;
    for (int i = 0; i < count; i++)
    {
        T value;
        if (!Detail::readValue(input, value))
        {
            error = "Input file ended early or contains an invalid value.";
            return false;
        }
        loaded.pushBack(value);
    }

    // Przypisanie wykonujemy dopiero po pelnym sukcesie, aby nie zostawic czesciowo wczytanych danych.
    values = loaded;
    error.clear();
    return true;
}

template <typename T>
bool writeValues(const std::string &path, const IStructure<T> &values, std::string &error)
{
    // Zapis ma ten sam format co wejscie: liczba elementow, potem jedna wartosc w kazdej linii.
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
