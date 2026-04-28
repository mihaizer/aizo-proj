#pragma once

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
    : head(nullptr), tail(nullptr), currentSize(0)
{
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T> &other)
    : head(nullptr), tail(nullptr), currentSize(0)
{
    copyFrom(other);
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(const DoublyLinkedList<T> &other)
{
    if (this == &other)
    {
        return *this;
    }

    release();
    copyFrom(other);

    return *this;
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(DoublyLinkedList<T> &&other) noexcept
    : head(other.head), tail(other.tail), currentSize(other.currentSize)
{
    other.head = nullptr;
    other.tail = nullptr;
    other.currentSize = 0;
}

template <typename T>
DoublyLinkedList<T> &DoublyLinkedList<T>::operator=(DoublyLinkedList<T> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    release();
    head = other.head;
    tail = other.tail;
    currentSize = other.currentSize;

    other.head = nullptr;
    other.tail = nullptr;
    other.currentSize = 0;

    return *this;
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    release();
}

template <typename T>
void DoublyLinkedList<T>::release()
{
    Node *current = head;
    while (current != nullptr)
    {
        Node *next = current->next;
        delete current;
        current = next;
    }

    head = nullptr;
    tail = nullptr;
    currentSize = 0;
}

template <typename T>
void DoublyLinkedList<T>::copyFrom(const DoublyLinkedList<T> &other)
{
    const Node *current = other.head;
    while (current != nullptr)
    {
        pushBack(current->value);
        current = current->next;
    }
}

template <typename T>
typename DoublyLinkedList<T>::Node *DoublyLinkedList<T>::nodeAt(int index)
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    if (index < currentSize / 2)
    {
        Node *current = head;
        for (int position = 0; position < index; ++position)
        {
            current = current->next;
        }
        return current;
    }

    Node *current = tail;
    for (int position = currentSize - 1; position > index; --position)
    {
        current = current->previous;
    }

    return current;
}

template <typename T>
const typename DoublyLinkedList<T>::Node *DoublyLinkedList<T>::nodeAt(int index) const
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    if (index < currentSize / 2)
    {
        const Node *current = head;
        for (int position = 0; position < index; ++position)
        {
            current = current->next;
        }
        return current;
    }

    const Node *current = tail;
    for (int position = currentSize - 1; position > index; --position)
    {
        current = current->previous;
    }

    return current;
}

template <typename T>
int DoublyLinkedList<T>::size() const
{
    return currentSize;
}

template <typename T>
T &DoublyLinkedList<T>::operator[](int index)
{
    return nodeAt(index)->value;
}

template <typename T>
const T &DoublyLinkedList<T>::operator[](int index) const
{
    return nodeAt(index)->value;
}

template <typename T>
void DoublyLinkedList<T>::pushBack(const T &value)
{
    Node *newNode = new Node{value, tail, nullptr};
    if (tail == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    ++currentSize;
}

template <typename T>
void DoublyLinkedList<T>::swap(int firstIndex, int secondIndex)
{
    if (firstIndex == secondIndex)
    {
        return;
    }

    Node *firstNode = nodeAt(firstIndex);
    Node *secondNode = nodeAt(secondIndex);
    T temporary = firstNode->value;
    firstNode->value = secondNode->value;
    secondNode->value = temporary;
}

template <typename T>
void DoublyLinkedList<T>::print() const
{
    const Node *current = head;
    while (current != nullptr)
    {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << '\n';
}
