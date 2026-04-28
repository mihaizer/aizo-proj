#pragma once

template <typename Structure>
QuickSort<Structure>::QuickSort(Parameters::Pivots selectedPivot)
    : pivotMode(selectedPivot == Parameters::Pivots::undefined ? Parameters::Pivots::middle : selectedPivot),
      randomState(2463534242U)
{
}

template <typename Structure>
int QuickSort<Structure>::choosePivotIndex(int leftIndex, int rightIndex)
{
    switch (pivotMode)
    {
    case Parameters::Pivots::left:
        return leftIndex;
    case Parameters::Pivots::right:
        return rightIndex;
    case Parameters::Pivots::random:
    {
        randomState = randomState * 1664525U + 1013904223U;
        const int rangeLength = rightIndex - leftIndex + 1;
        return leftIndex + static_cast<int>(randomState % static_cast<unsigned int>(rangeLength));
    }
    case Parameters::Pivots::middle:
    case Parameters::Pivots::undefined:
    case Parameters::Pivots::count:
        return leftIndex + (rightIndex - leftIndex) / 2;
    }

    return leftIndex + (rightIndex - leftIndex) / 2;
}

template <typename Structure>
void QuickSort<Structure>::sortRange(Structure &data, int leftIndex, int rightIndex)
{
    while (leftIndex < rightIndex)
    {
        int forwardIndex = leftIndex;
        int backwardIndex = rightIndex;
        const auto pivotValue = data[choosePivotIndex(leftIndex, rightIndex)];

        while (forwardIndex <= backwardIndex)
        {
            while (data[forwardIndex] < pivotValue)
            {
                ++forwardIndex;
            }

            while (pivotValue < data[backwardIndex])
            {
                --backwardIndex;
            }

            if (forwardIndex <= backwardIndex)
            {
                data.swap(forwardIndex, backwardIndex);
                ++forwardIndex;
                --backwardIndex;
            }
        }

        if (backwardIndex - leftIndex < rightIndex - forwardIndex)
        {
            if (leftIndex < backwardIndex)
            {
                sortRange(data, leftIndex, backwardIndex);
            }
            leftIndex = forwardIndex;
        }
        else
        {
            if (forwardIndex < rightIndex)
            {
                sortRange(data, forwardIndex, rightIndex);
            }
            rightIndex = backwardIndex;
        }
    }
}

template <typename Structure>
void QuickSort<Structure>::sort(Structure &data)
{
    if (data.size() > 1)
    {
        sortRange(data, 0, data.size() - 1);
    }
}
