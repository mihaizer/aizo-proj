#pragma once

#include "Parameters.h"

template <typename Structure>
class QuickSort
{
private:
    Parameters::Pivots pivotMode;
    unsigned int randomState;

    int choosePivotIndex(int leftIndex, int rightIndex);
    void sortRange(Structure &data, int leftIndex, int rightIndex);

public:
    explicit QuickSort(Parameters::Pivots selectedPivot);
    void sort(Structure &data);
};

#include "QuickSort.tpp"
