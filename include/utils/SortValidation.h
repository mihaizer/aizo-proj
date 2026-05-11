#pragma once

#include "structures/IStructure.h"

template <typename T>
bool isSortedAscending(const IStructure<T> &values); // isSortedAscending(data); sprawdza kolejne pary elementow bez uzywania gotowego sortowania.

#include "SortValidation.tpp"
