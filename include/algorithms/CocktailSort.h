#pragma once

#include "structures/IStructure.h"

namespace CocktailSort
{
template <typename T>
void sort(IStructure<T> &values); // CocktailSort::sort(data); sortuje rosnaco przez przejscia od lewej i od prawej strony.
}

#include "CocktailSort.tpp"
