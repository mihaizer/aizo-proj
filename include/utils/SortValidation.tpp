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

template <typename T>
bool isSortedAscending(const SinglyLinkedList<T> &values)
{
    auto current = values.head;
    while (current != nullptr && current->next != nullptr)
    {
        if (current->next->value < current->value)
        {
            return false;
        }

        current = current->next;
    }

    return true;
}
