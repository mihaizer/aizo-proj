#pragma once

#include "structures/IStructure.h"

namespace InsertionSort
{
template <typename T>
void sort(IStructure<T> &values); // InsertionSort::sort(data); sortuje strukture rosnaco przez operator[] z interfejsu IStructure.
}

#include "InsertionSort.tpp"
