#pragma once

#include "structures/IStructure.h"
#include "structures/SinglyLinkedList.h"

namespace CocktailSort
{
template <typename T>
void sort(IStructure<T> &values); // CocktailSort::sort(data); sortuje rosnaco przez przejscia od lewej i od prawej strony.

template <typename T>
void sort(SinglyLinkedList<T> &values); // CocktailSort::sort(list); fallback przez operator[] z interfejsu IStructure.
}

#include "CocktailSort.tpp"
