#pragma once

template <typename Structure>
void InsertionSort<Structure>::sort(Structure &data) const
{
    const int dataSize = data.size();
    for (int index = 1; index < dataSize; ++index)
    {
        const auto value = data[index];
        int sortedIndex = index - 1;

        while (sortedIndex >= 0 && value < data[sortedIndex])
        {
            data[sortedIndex + 1] = data[sortedIndex];
            --sortedIndex;
        }

        data[sortedIndex + 1] = value;
    }
}
