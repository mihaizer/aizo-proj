#pragma once

namespace SortValidation
{
template <typename Structure>
bool isSortedAscending(const Structure &data)
{
    const int dataSize = data.size();
    for (int index = 1; index < dataSize; ++index)
    {
        if (data[index] < data[index - 1])
        {
            return false;
        }
    }

    return true;
}
}
