#pragma once

namespace MergeSort
{
namespace Detail
{
    template <typename T>
    void mergeArray(DynamicArray<T> &values, T *buffer, int left, int middle, int right)
    {
        int leftIndex = left;
        int rightIndex = middle + 1;
        int bufferIndex = left;

        while (leftIndex <= middle && rightIndex <= right)
        {
            // Przy rownych wartosciach bierzemy element z lewej czesci, zeby zachowac stabilnosc sortowania.
            if (!(values[rightIndex] < values[leftIndex]))
            {
                buffer[bufferIndex] = values[leftIndex];
                leftIndex++;
            }
            else
            {
                buffer[bufferIndex] = values[rightIndex];
                rightIndex++;
            }

            bufferIndex++;
        }

        while (leftIndex <= middle)
        {
            buffer[bufferIndex] = values[leftIndex];
            leftIndex++;
            bufferIndex++;
        }

        while (rightIndex <= right)
        {
            buffer[bufferIndex] = values[rightIndex];
            rightIndex++;
            bufferIndex++;
        }

        for (int index = left; index <= right; index++)
        {
            values[index] = buffer[index];
        }
    }

    template <typename T>
    void mergeSortArray(DynamicArray<T> &values, T *buffer, int left, int right)
    {
        if (left >= right)
        {
            return;
        }

        int middle = left + (right - left) / 2;
        mergeSortArray(values, buffer, left, middle);
        mergeSortArray(values, buffer, middle + 1, right);
        mergeArray(values, buffer, left, middle, right);
    }
}

template <typename T>
void sort(DynamicArray<T> &values)
{
    if (values.size() < 2)
    {
        return;
    }

    // Jeden bufor wystarcza dla calej rekurencji, wiec nie alokujemy pamieci przy kazdym scalaniu.
    T *buffer = new T[values.size()];
    Detail::mergeSortArray(values, buffer, 0, values.size() - 1);
    delete[] buffer;
}

template <typename T>
struct SinglyLinkedListAccess
{
private:
    using List = SinglyLinkedList<T>;
    using Node = typename SinglyLinkedList<T>::Node;

    static Node *split(Node *start)
    {
        Node *slow = start;
        Node *fast = start->next;

        // Wskaznik fast idzie dwa razy szybciej, dlatego slow zostaje przy koncu lewej polowy.
        while (fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        Node *right = slow->next;
        slow->next = nullptr;
        return right;
    }

    static void appendNode(Node *&result, Node *&tail, Node *node)
    {
        node->next = nullptr;

        if (result == nullptr)
        {
            result = node;
            tail = node;
            return;
        }

        tail->next = node;
        tail = node;
    }

    static Node *merge(Node *left, Node *right)
    {
        Node *result = nullptr;
        Node *tail = nullptr;

        while (left != nullptr && right != nullptr)
        {
            Node *selected = nullptr;

            // Tak jak w tablicy, rowne wartosci zostaja w swojej kolejnosci z lewej czesci.
            if (!(right->value < left->value))
            {
                selected = left;
                left = left->next;
            }
            else
            {
                selected = right;
                right = right->next;
            }

            appendNode(result, tail, selected);
        }

        Node *rest = (left != nullptr) ? left : right;
        if (tail == nullptr)
        {
            return rest;
        }

        tail->next = rest;
        return result;
    }

    static Node *mergeSortNodes(Node *start)
    {
        if (start == nullptr || start->next == nullptr)
        {
            return start;
        }

        Node *right = split(start);
        Node *leftSorted = mergeSortNodes(start);
        Node *rightSorted = mergeSortNodes(right);
        return merge(leftSorted, rightSorted);
    }

    static Node *findTail(Node *start)
    {
        if (start == nullptr)
        {
            return nullptr;
        }

        Node *current = start;
        while (current->next != nullptr)
        {
            current = current->next;
        }

        return current;
    }

public:
    static void sort(List &values)
    {
        if (values.currentSize < 2)
        {
            return;
        }

        // Lista jest sortowana przez przepinanie next, bez wolnego dostepu values[index].
        values.head = mergeSortNodes(values.head);
        values.tail = findTail(values.head);
    }
};

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    SinglyLinkedListAccess<T>::sort(values);
}
}
