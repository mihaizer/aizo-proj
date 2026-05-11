#pragma once

namespace InsertionSort
{
template <typename T>
void sort(DynamicArray<T> &values)
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

template <typename T>
struct SinglyLinkedListAccess
{
private:
    using List = SinglyLinkedList<T>;
    using Node = typename SinglyLinkedList<T>::Node;

    static void insertSorted(Node *&sortedHead, Node *&sortedTail, Node *node)
    {
        node->next = nullptr;

        if (sortedHead == nullptr || node->value < sortedHead->value)
        {
            node->next = sortedHead;
            sortedHead = node;
            if (sortedTail == nullptr)
            {
                sortedTail = node;
            }
            return;
        }

        Node *current = sortedHead;
        while (current->next != nullptr && !(node->value < current->next->value))
        {
            current = current->next;
        }

        node->next = current->next;
        current->next = node;
        if (node->next == nullptr)
        {
            sortedTail = node;
        }
    }

public:
    static void sort(List &values)
    {
        Node *sortedHead = nullptr;
        Node *sortedTail = nullptr;
        Node *current = values.head;

        while (current != nullptr)
        {
            Node *next = current->next;
            insertSorted(sortedHead, sortedTail, current);
            current = next;
        }

        values.head = sortedHead;
        values.tail = sortedTail;
    }
};

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    SinglyLinkedListAccess<T>::sort(values);
}
}
