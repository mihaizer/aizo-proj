#pragma once

#include <iostream>
#include <stdexcept>

#include "IStructure.h"

template <typename T>
class SinglyLinkedList : public IStructure<T>
{
private:
    struct Node
    {
        T value;
        Node *next;
    };

    Node *head;
    Node *tail;
    int currentSize;

    Node *nodeAt(int index);
    const Node *nodeAt(int index) const;
    void copyFrom(const SinglyLinkedList<T> &other);
    void release();

public:
    using ValueType = T;

    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList<T> &other);
    SinglyLinkedList &operator=(const SinglyLinkedList<T> &other);
    SinglyLinkedList(SinglyLinkedList<T> &&other) noexcept;
    SinglyLinkedList &operator=(SinglyLinkedList<T> &&other) noexcept;
    ~SinglyLinkedList() override;

    int size() const override;
    T &operator[](int index) override;
    const T &operator[](int index) const override;

    void pushBack(const T &value) override;
    void swap(int firstIndex, int secondIndex) override;
    void print() const;
};

#include "SinglyLinkedList.tpp"
