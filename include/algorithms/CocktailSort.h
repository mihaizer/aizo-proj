#pragma once

#include "structures/IStructure.h"
#include "structures/SinglyLinkedList.h"

namespace CocktailSort
{
template <typename T>
struct SinglyLinkedListAccess; // SinglyLinkedListAccess<int>::sort(list); sortuje liste bez losowego dostepu po indeksach.

template <typename T>
void sort(IStructure<T> &values); // CocktailSort::sort(data); sortuje rosnaco przez przejscia od lewej i od prawej strony.

template <typename T>
void sort(SinglyLinkedList<T> &values); // CocktailSort::sort(list); sortuje liste przez bezposredni dostep do wezlow.
}

#include "CocktailSort.tpp"
