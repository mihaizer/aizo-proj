#pragma once

#include "structures/IStructure.h"
#include "structures/SinglyLinkedList.h"

namespace InsertionSort
{
template <typename T>
struct SinglyLinkedListAccess; // SinglyLinkedListAccess<int>::sort(list); sortuje liste przez przepinanie wezlow.

template <typename T>
void sort(IStructure<T> &values); // InsertionSort::sort(data); sortuje strukture rosnaco przez operator[] z interfejsu IStructure.

template <typename T>
void sort(SinglyLinkedList<T> &values); // InsertionSort::sort(list); sortuje liste bez losowego dostepu po indeksach.
}

#include "InsertionSort.tpp"
