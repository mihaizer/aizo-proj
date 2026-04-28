#pragma once

#include <iostream>
#include <stdexcept>

#include "IStructure.h"

template <typename T>
class DynamicArray : public IStructure<T>
{
private:
    T *data;
    int currentSize;
    int capacity;

    void resize(int newCapacity);

public:
    using ValueType = T;

    explicit DynamicArray(int initialSize = 0);
    DynamicArray(const DynamicArray &other);
    DynamicArray &operator=(const DynamicArray &other);
    DynamicArray(DynamicArray &&other) noexcept;
    DynamicArray &operator=(DynamicArray &&other) noexcept;
    ~DynamicArray() override;

    int size() const override;
    int getSize() const;
    int getCapacity() const;

    T &operator[](int index) override;
    const T &operator[](int index) const override;

    void pushBack(const T &value) override;
    void swap(int firstIndex, int secondIndex) override;
    void print() const;
};

#include "DynamicArray.tpp"
