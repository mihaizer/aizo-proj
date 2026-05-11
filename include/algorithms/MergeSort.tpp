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

    struct Chain
    {
        Node *head;
        Node *tail;
    };

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

    static void appendNode(Chain &result, Node *node)
    {
        node->next = nullptr;

        if (result.head == nullptr)
        {
            result.head = node;
            result.tail = node;
            return;
        }

        result.tail->next = node;
        result.tail = node;
    }

    static Chain merge(Chain left, Chain right)
    {
        Chain result{nullptr, nullptr};
        Node *leftCurrent = left.head;
        Node *rightCurrent = right.head;

        while (leftCurrent != nullptr && rightCurrent != nullptr)
        {
            Node *selected = nullptr;

            // Tak jak w tablicy, rowne wartosci zostaja w swojej kolejnosci z lewej czesci.
            if (!(rightCurrent->value < leftCurrent->value))
            {
                selected = leftCurrent;
                leftCurrent = leftCurrent->next;
            }
            else
            {
                selected = rightCurrent;
                rightCurrent = rightCurrent->next;
            }

            appendNode(result, selected);
        }

        Chain rest = (leftCurrent != nullptr) ? Chain{leftCurrent, left.tail} : Chain{rightCurrent, right.tail};
        if (result.head == nullptr)
        {
            return rest;
        }

        result.tail->next = rest.head;
        if (rest.tail != nullptr)
        {
            result.tail = rest.tail;
        }
        return result;
    }

    static Chain mergeSortNodes(Node *start)
    {
        if (start == nullptr || start->next == nullptr)
        {
            return Chain{start, start};
        }

        Node *right = split(start);
        Chain leftSorted = mergeSortNodes(start);
        Chain rightSorted = mergeSortNodes(right);
        return merge(leftSorted, rightSorted);
    }

public:
    static void sort(List &values)
    {
        if (values.currentSize < 2)
        {
            return;
        }

        // Lista jest sortowana przez przepinanie next, bez wolnego dostepu values[index].
        Chain sorted = mergeSortNodes(values.head);
        values.head = sorted.head;
        values.tail = sorted.tail;
    }
};

template <typename T>
void sort(SinglyLinkedList<T> &values)
{
    SinglyLinkedListAccess<T>::sort(values);
}
}
