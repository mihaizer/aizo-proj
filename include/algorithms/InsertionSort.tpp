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

template <typename T>
struct SinglyLinkedListAccess
{
private:
    using List = SinglyLinkedList<T>;
    using Node = typename SinglyLinkedList<T>::Node;

    static void insertSorted(Node *&sortedHead, Node *node)
    {
        if (sortedHead == nullptr || node->value < sortedHead->value)
        {
            node->next = sortedHead;
            sortedHead = node;
            return;
        }

        Node *current = sortedHead;
        while (current->next != nullptr && !(node->value < current->next->value))
        {
            current = current->next;
        }

        node->next = current->next;
        current->next = node;
    }

    static Node *findTail(Node *start)
    {
        Node *current = start;
        while (current != nullptr && current->next != nullptr)
        {
            current = current->next;
        }

        return current;
    }

public:
    static void sort(List &values)
    {
        Node *sortedHead = nullptr;
        Node *current = values.head;

        while (current != nullptr)
        {
            Node *next = current->next;
            insertSorted(sortedHead, current);
            current = next;
        }

        values.head = sortedHead;
        values.tail = findTail(values.head);
    }
};

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    SinglyLinkedListAccess<T>::sort(values);
}
}
