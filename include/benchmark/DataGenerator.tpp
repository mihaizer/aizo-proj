#pragma once

#include <cmath>
#include <limits>
#include <string>
#include <type_traits>

namespace DataGenerator
{
namespace Detail
{
    inline std::string makeOrderedString(int rank)
    {
        std::string number = std::to_string(rank);
        const int width = 10;

        if (static_cast<int>(number.size()) >= width)
        {
            return number;
        }

        return std::string(width - static_cast<int>(number.size()), '0') + number;
    }

    inline std::string makeRandomString(std::mt19937 &rng)
    {
        std::uniform_int_distribution<int> lengthDistribution(4, 12);
        std::uniform_int_distribution<int> charDistribution(32, 126);

        int length = lengthDistribution(rng);
        std::string value;

        for (int index = 0; index < length; index++)
        {
            value += static_cast<char>(charDistribution(rng));
        }

        return value;
    }

    template <typename T>
    T makeOrderedValue(int rank, int size)
    {
        if constexpr (std::is_same<T, std::string>::value)
        {
            return makeOrderedString(rank);
        }
        else
        {
            long double minValue = 0.0L;
            long double maxValue = static_cast<long double>(std::numeric_limits<T>::max());

            if constexpr (std::is_signed<T>::value)
            {
                minValue = static_cast<long double>(std::numeric_limits<T>::lowest());
            }

            long double ratio = 0.0L;
            if (size > 1)
            {
                ratio = static_cast<long double>(rank) / static_cast<long double>(size - 1);
            }

            long double value = minValue + (maxValue - minValue) * ratio;
            if (value < minValue)
            {
                value = minValue;
            }
            if (value > maxValue)
            {
                value = maxValue;
            }

            return static_cast<T>(value);
        }
    }

    template <typename T>
    T makeRandomFloatingValue(std::mt19937 &rng)
    {
        std::uniform_int_distribution<int> signDistribution(0, 1);
        std::uniform_int_distribution<int> exponentDistribution(std::numeric_limits<T>::min_exponent,
                                                                std::numeric_limits<T>::max_exponent - 1);
        std::uniform_real_distribution<long double> fractionDistribution(0.5L, 1.0L);

        long double value = std::ldexp(fractionDistribution(rng), exponentDistribution(rng));
        if (signDistribution(rng) == 0)
        {
            value = -value;
        }

        return static_cast<T>(value);
    }

    template <typename T>
    T makeRandomValue(std::mt19937 &rng)
    {
        if constexpr (std::is_same<T, std::string>::value)
        {
            return makeRandomString(rng);
        }
        else if constexpr (std::is_floating_point<T>::value)
        {
            return makeRandomFloatingValue<T>(rng);
        }
        else if constexpr (std::is_signed<T>::value)
        {
            std::uniform_int_distribution<long long> distribution(
                static_cast<long long>(std::numeric_limits<T>::lowest()),
                static_cast<long long>(std::numeric_limits<T>::max()));
            return static_cast<T>(distribution(rng));
        }
        else
        {
            std::uniform_int_distribution<unsigned long long> distribution(
                0,
                static_cast<unsigned long long>(std::numeric_limits<T>::max()));
            return static_cast<T>(distribution(rng));
        }
    }

    template <typename T>
    T makeValue(int index, int size, Parameters::Distribution distribution, std::mt19937 &rng)
    {
        switch (distribution)
        {
        case Parameters::Distribution::random:
            return makeRandomValue<T>(rng);
        case Parameters::Distribution::ascending:
            return makeOrderedValue<T>(index, size);
        case Parameters::Distribution::ascending50Per:
            // Pierwsza polowa danych jest juz posortowana, a druga polowa zostaje losowa.
            if (index < size / 2)
            {
                return makeOrderedValue<T>(index, size);
            }
            return makeRandomValue<T>(rng);
        case Parameters::Distribution::descending:
            return makeOrderedValue<T>(size - index - 1, size);
        case Parameters::Distribution::undefined:
        case Parameters::Distribution::count:
            return T{};
        }

        return T{};
    }
}

template <typename T>
void fill(DynamicArray<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng)
{
    // Tablica jest tworzona od razu z rozmiarem size, dlatego tutaj tylko wpisujemy wartosci pod indeksy.
    for (int index = 0; index < size; index++)
    {
        values[index] = Detail::makeValue<T>(index, size, distribution, rng);
    }
}

template <typename T>
void fill(SinglyLinkedList<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng)
{
    // Lista nie ma losowego dostepu do pamieci, wiec generator buduje ja kolejnymi pushBack.
    for (int index = 0; index < size; index++)
    {
        values.pushBack(Detail::makeValue<T>(index, size, distribution, rng));
    }
}
}
