#pragma once

#include <cstring>
#include <iostream>
#include <stdexcept>

template <typename T>
DynamicArray<T>::DynamicArray(int n)
{
    if (n < 0)
    {
        throw std::invalid_argument("Size must be >= 0");
    }

    currentSize = n;
    capacity = n == 0 ? 1 : n;
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray &other)
{
    currentSize = other.currentSize;
    capacity = other.capacity;
    data = new T[capacity];
    std::memcpy(data, other.data, currentSize * sizeof(T));
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other)
{
    if (this == &other)
    {
        return *this;
    }

    delete[] data;

    currentSize = other.currentSize;
    capacity = other.capacity;
    data = new T[capacity];
    std::memcpy(data, other.data, currentSize * sizeof(T));

    return *this;
}

template <typename T>
T &DynamicArray<T>::operator[](int index)
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

template <typename T>
const T &DynamicArray<T>::operator[](int index) const
{
    if (index < 0 || index >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    return data[index];
}

template <typename T>
int DynamicArray<T>::size() const
{
    return currentSize;
}

template <typename T>
void DynamicArray<T>::pushBack(const T &value)
{
    if (currentSize == capacity)
    {
        int newCapacity = capacity + 1;
        T *newData = new T[newCapacity];

        std::memcpy(newData, data, currentSize * sizeof(T));

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

    data[currentSize] = value;
    currentSize++;
}

template <typename T>
void DynamicArray<T>::swap(int i, int j)
{
    if (i < 0 || i >= currentSize || j < 0 || j >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    T temp = data[i];
    data[i] = data[j];
    data[j] = temp;
}

template <typename T>
void DynamicArray<T>::print() const
{
    for (int i = 0; i < currentSize; i++)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

template <typename T>
int DynamicArray<T>::getCapacity() const
{
    return capacity;
}
