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
struct SinglyLinkedListAccess
{
private:
    using List = SinglyLinkedList<T>;
    using Node = typename SinglyLinkedList<T>::Node;

    static void swapValues(Node *left, Node *right)
    {
        T temp = left->value;
        left->value = right->value;
        right->value = temp;
    }

public:
    static void sort(List &values)
    {
        if (values.currentSize < 2)
        {
            return;
        }

        // nodes to tablica wskaznikow na wezly:
        // nodes[i] przechowuje adres i-tego wezla listy.
        Node **nodes = new Node *[values.currentSize];
        Node *current = values.head;
        for (int index = 0; index < values.currentSize; index++)
        {
            // Zapamietujemy adres biezacego wezla, aby potem miec szybki dostep
            // do kolejnych elementow bez przechodzenia od head za kazdym razem.
            nodes[index] = current;
            current = current->next;
        }

        bool swapped = true;
        int left = 0;
        int right = values.currentSize - 1;

        while (swapped)
        {
            swapped = false;

            // Dla listy przechodzimy po zapamietanych wskaznikach, a nie przez kosztowne nodeAt(index).
            for (int index = left; index < right; index++)
            {
                if (nodes[index + 1]->value < nodes[index]->value)
                {
                    swapValues(nodes[index], nodes[index + 1]);
                    swapped = true;
                }
            }

            right--;
            if (!swapped)
            {
                break;
            }

            swapped = false;

            for (int index = right; index > left; index--)
            {
                if (nodes[index]->value < nodes[index - 1]->value)
                {
                    swapValues(nodes[index], nodes[index - 1]);
                    swapped = true;
                }
            }

            left++;
        }

        delete[] nodes;
    }
};

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    SinglyLinkedListAccess<T>::sort(values);
}
}
