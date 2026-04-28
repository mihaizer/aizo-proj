#pragma once

#include <iostream>
#include <stdexcept>

#include "IStructure.h"

template <typename T>
class DoublyLinkedList : public IStructure<T>
{
private:
    struct Node
    {
        T value;
        Node *previous;
        Node *next;
    };

    Node *head;
    Node *tail;
    int currentSize;

    Node *nodeAt(int index);
    const Node *nodeAt(int index) const;
    void copyFrom(const DoublyLinkedList<T> &other);
    void release();

public:
    using ValueType = T;

    DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList<T> &other);
    DoublyLinkedList &operator=(const DoublyLinkedList<T> &other);
    DoublyLinkedList(DoublyLinkedList<T> &&other) noexcept;
    DoublyLinkedList &operator=(DoublyLinkedList<T> &&other) noexcept;
    ~DoublyLinkedList() override;

    int size() const override;
    T &operator[](int index) override;
    const T &operator[](int index) const override;

    void pushBack(const T &value) override;
    void swap(int firstIndex, int secondIndex) override;
    void print() const;
};

#include "DoublyLinkedList.tpp"
