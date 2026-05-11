#pragma once

#include <random>

#include "Parameters.h"
#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"

namespace DataGenerator
{
template <typename T>
void fill(DynamicArray<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng); // DataGenerator::fill(arr, n, dist, rng); wypelnia istniejaca tablice n wartosciami testowymi.

template <typename T>
void fill(SinglyLinkedList<T> &values, int size, Parameters::Distribution distribution, std::mt19937 &rng); // DataGenerator::fill(list, n, dist, rng); dopisuje do pustej listy n wartosci testowych.
}

#include "DataGenerator.tpp"
