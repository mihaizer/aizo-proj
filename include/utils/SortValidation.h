#pragma once

#include "structures/IStructure.h"
#include "structures/SinglyLinkedList.h"

template <typename T>
bool isSortedAscending(const IStructure<T> &values); // isSortedAscending(data); sprawdza kolejne pary elementow bez uzywania gotowego sortowania.

template <typename T>
bool isSortedAscending(const SinglyLinkedList<T> &values); // isSortedAscending(list); sprawdza liste jednym przejsciem po wezlowach.

#include "SortValidation.tpp"
