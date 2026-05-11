#pragma once

#include "structures/DynamicArray.h"
#include "structures/SinglyLinkedList.h"

namespace MergeSort
{
template <typename T>
struct SinglyLinkedListAccess; // SinglyLinkedListAccess<int>::sort(list); ma dostep do prywatnych wezlow listy dla merge sort.

template <typename T>
void sort(DynamicArray<T> &values); // MergeSort::sort(arr); sortuje tablice dynamiczna przez indeksy i bufor pomocniczy.

template <typename T>
void sort(SinglyLinkedList<T> &values); // MergeSort::sort(list); sortuje liste jednokierunkowa przez przepinanie wezlow.
}

#include "MergeSort.tpp"
