#pragma once

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList()
    : head(nullptr), tail(nullptr), currentSize(0)
{
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T> &other)
    : head(nullptr), tail(nullptr), currentSize(0)
{
    copyFrom(other);
}

template <typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(const SinglyLinkedList<T> &other)
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
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T> &&other) noexcept
    : head(other.head), tail(other.tail), currentSize(other.currentSize)
{
    other.head = nullptr;
    other.tail = nullptr;
    other.currentSize = 0;
}

template <typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(SinglyLinkedList<T> &&other) noexcept
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
SinglyLinkedList<T>::~SinglyLinkedList()
{
    release();
}

template <typename T>
void SinglyLinkedList<T>::release()
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
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList<T> &other)
{
    const Node *current = other.head;
    while (current != nullptr)
    {
        pushBack(current->value);
        current = current->next;
    }
}

template <typename T>
typename SinglyLinkedList<T>::Node *SinglyLinkedList<T>::nodeAt(int index)
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    Node *current = head;
    for (int position = 0; position < index; ++position)
    {
        current = current->next;
    }

    return current;
}

template <typename T>
const typename SinglyLinkedList<T>::Node *SinglyLinkedList<T>::nodeAt(int index) const
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    const Node *current = head;
    for (int position = 0; position < index; ++position)
    {
        current = current->next;
    }

    return current;
}

template <typename T>
int SinglyLinkedList<T>::size() const
{
    return currentSize;
}

template <typename T>
T &SinglyLinkedList<T>::operator[](int index)
{
    return nodeAt(index)->value;
}

template <typename T>
const T &SinglyLinkedList<T>::operator[](int index) const
{
    return nodeAt(index)->value;
}

template <typename T>
void SinglyLinkedList<T>::pushBack(const T &value)
{
    Node *newNode = new Node{value, nullptr};
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
void SinglyLinkedList<T>::swap(int firstIndex, int secondIndex)
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
void SinglyLinkedList<T>::print() const
{
    const Node *current = head;
    while (current != nullptr)
    {
        std::cout << current->value << " ";
        current = current->next;
    }
    std::cout << '\n';
}
