#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <type_traits>

namespace DataGenerator
{
namespace Detail
{
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
    void fillRandomBuffer(T *buffer, int size, std::mt19937 &rng)
    {
        for (int index = 0; index < size; index++)
        {
            buffer[index] = makeRandomValue<T>(rng);
        }
    }

    template <typename T>
    void prepareDistribution(T *buffer, int size, Parameters::Distribution distribution)
    {
        switch (distribution)
        {
        case Parameters::Distribution::random:
            return;
        case Parameters::Distribution::ascending:
            std::sort(buffer, buffer + size);
            return;
        case Parameters::Distribution::ascending50Per:
            // Najpierw powstaje caly losowy zestaw, a potem sortujemy tylko pierwsza polowe danych.
            std::sort(buffer, buffer + (size + 1) / 2);
            return;
        case Parameters::Distribution::descending:
            std::sort(buffer, buffer + size, [](const T &left, const T &right) {
                return right < left;
            });
            return;
        case Parameters::Distribution::undefined:
        case Parameters::Distribution::count:
            return;
        }
    }
}

template <typename T>
void fill(DynamicArray<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng)
{
    if (size <= 0)
    {
        return;
    }

    T *buffer = new T[size];
    Detail::fillRandomBuffer(buffer, size, rng);
    Detail::prepareDistribution(buffer, size, distribution);

    // Tablica jest tworzona od razu z rozmiarem size, dlatego tutaj tylko przepisujemy gotowe dane.
    for (int index = 0; index < size; index++)
    {
        values[index] = buffer[index];
    }

    delete[] buffer;
}

template <typename T>
void fill(SinglyLinkedList<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng)
{
    if (size <= 0)
    {
        return;
    }

    T *buffer = new T[size];
    Detail::fillRandomBuffer(buffer, size, rng);
    Detail::prepareDistribution(buffer, size, distribution);

    // Lista nie ma losowego dostepu do pamieci, wiec gotowe dane dopisujemy kolejnymi pushBack.
    for (int index = 0; index < size; index++)
    {
        values.pushBack(buffer[index]);
    }

    delete[] buffer;
}
}
