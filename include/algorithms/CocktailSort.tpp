#pragma once

namespace CocktailSort
{
template <typename T>
void sort(IStructure<T> &values)
{
    if (values.size() < 2)
    {
        return;
    }

    bool swapped = true;
    int left = 0;
    int right = values.size() - 1;

    while (swapped)
    {
        swapped = false;

        // Pierwsze przejscie przesuwa najwiekszy element aktualnego zakresu na prawa strone.
        for (int index = left; index < right; index++)
        {
            if (values[index + 1] < values[index])
            {
                values.swap(index, index + 1);
                swapped = true;
            }
        }

        right--;
        if (!swapped)
        {
            break;
        }

        swapped = false;

        // Drugie przejscie przesuwa najmniejszy element aktualnego zakresu na lewa strone.
        for (int index = right; index > left; index--)
        {
            if (values[index] < values[index - 1])
            {
                values.swap(index, index - 1);
                swapped = true;
            }
        }

        left++;
    }
}

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    // Cocktail sort wymaga przejsc w obie strony, wiec dla listy jednokierunkowej
    // zostawiamy tylko wolny fallback przez operator[] z interfejsu IStructure.
    IStructure<T> &indexedValues = values;
    sort(indexedValues);
}
}
