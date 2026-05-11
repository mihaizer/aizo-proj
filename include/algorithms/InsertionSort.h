#pragma once

#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"

namespace InsertionSort
{
template <typename T>
struct SinglyLinkedListAccess; // SinglyLinkedListAccess<int>::sort(list); sortuje liste przez przepinanie wezlow.

template <typename T>
void sort(DynamicArray<T> &values); // InsertionSort::sort(arr); sortuje tablice przez operator[] z dostepem O(1).

template <typename T>
void sort(SinglyLinkedList<T> &values); // InsertionSort::sort(list); sortuje liste bez losowego dostepu po indeksach.
}

#include "InsertionSort.tpp"
