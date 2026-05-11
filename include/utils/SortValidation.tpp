#pragma once

template <typename T>
bool isSortedAscending(const IStructure<T> &values)
{
    for (int i = 1; i < values.size(); i++)
    {
        if (values[i] < values[i - 1])
        {
            return false;
        }
    }

    return true;
}
