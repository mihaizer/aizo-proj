#pragma once

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList()
    : head(nullptr),
      tail(nullptr),
      currentSize(0)
{
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList()
{
    clear();
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList &other)
    : head(nullptr),
      tail(nullptr),
      currentSize(0)
{
    copyFrom(other);
}

template <typename T>
SinglyLinkedList<T> &SinglyLinkedList<T>::operator=(const SinglyLinkedList &other)
{
    if (this == &other)
    {
        return *this;
    }

    clear();
    copyFrom(other);
    return *this;
}

template <typename T>
void SinglyLinkedList<T>::clear()
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
void SinglyLinkedList<T>::copyFrom(const SinglyLinkedList &other)
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
    // Indeksy sa kontrolowane przez kod wywolujacy, wiec tutaj przechodzimy bezposrednio po liscie.
    Node *current = head;
    for (int currentIndex = 0; currentIndex < index; currentIndex++)
    {
        current = current->next;
    }

    return current;
}

template <typename T>
const typename SinglyLinkedList<T>::Node *SinglyLinkedList<T>::nodeAt(int index) const
{
    // Indeksy sa kontrolowane przez kod wywolujacy, wiec tutaj przechodzimy bezposrednio po liscie.
    const Node *current = head;
    for (int currentIndex = 0; currentIndex < index; currentIndex++)
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

    currentSize++;
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
void SinglyLinkedList<T>::swap(int i, int j)
{
    if (i == j)
    {
        return;
    }

    Node *left = nodeAt(i);
    Node *right = nodeAt(j);

    T temp = left->value;
    left->value = right->value;
    right->value = temp;
}
