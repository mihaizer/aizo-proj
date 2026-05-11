#pragma once

namespace InsertionSort
{
template <typename T>
void sort(IStructure<T> &values)
{
    // Sortowanie przez wstawianie buduje posortowany prefiks po lewej stronie tablicy.
    for (int index = 1; index < values.size(); index++)
    {
        T value = values[index];
        int sortedIndex = index - 1;

        // Elementy wieksze od value przesuwamy o jedno miejsce w prawo.
        while (sortedIndex >= 0 && value < values[sortedIndex])
        {
            values[sortedIndex + 1] = values[sortedIndex];
            sortedIndex--;
        }

        values[sortedIndex + 1] = value;
    }
}
}
