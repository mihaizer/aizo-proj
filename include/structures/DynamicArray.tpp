#pragma once

template <typename T>
DynamicArray<T>::DynamicArray(int initialSize)
    : data(nullptr), currentSize(initialSize), capacity(initialSize > 0 ? initialSize : 1)
{
    if (initialSize < 0)
    {
        throw std::invalid_argument("Size must be >= 0");
    }

    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other)
    : data(new T[other.capacity]), currentSize(other.currentSize), capacity(other.capacity)
{
    for (int index = 0; index < currentSize; ++index)
    {
        data[index] = other.data[index];
    }
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray<T> &other)
{
    if (this == &other)
    {
        return *this;
    }

    T *newData = new T[other.capacity];
    for (int index = 0; index < other.currentSize; ++index)
    {
        newData[index] = other.data[index];
    }

    delete[] data;
    data = newData;
    currentSize = other.currentSize;
    capacity = other.capacity;

    return *this;
}

template <typename T>
DynamicArray<T>::DynamicArray(DynamicArray<T> &&other) noexcept
    : data(other.data), currentSize(other.currentSize), capacity(other.capacity)
{
    other.data = nullptr;
    other.currentSize = 0;
    other.capacity = 0;
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(DynamicArray<T> &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    delete[] data;
    data = other.data;
    currentSize = other.currentSize;
    capacity = other.capacity;

    other.data = nullptr;
    other.currentSize = 0;
    other.capacity = 0;

    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;
}

template <typename T>
void DynamicArray<T>::resize(int newCapacity)
{
    T *newData = new T[newCapacity];
    for (int index = 0; index < currentSize; ++index)
    {
        newData[index] = data[index];
    }

    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
int DynamicArray<T>::size() const
{
    return currentSize;
}

template <typename T>
int DynamicArray<T>::getSize() const
{
    return currentSize;
}

template <typename T>
int DynamicArray<T>::getCapacity() const
{
    return capacity;
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
void DynamicArray<T>::pushBack(const T &value)
{
    if (currentSize == capacity)
    {
        resize(capacity > 0 ? capacity * 2 : 1);
    }

    data[currentSize] = value;
    ++currentSize;
}

template <typename T>
void DynamicArray<T>::swap(int firstIndex, int secondIndex)
{
    if (firstIndex < 0 || firstIndex >= currentSize || secondIndex < 0 || secondIndex >= currentSize)
    {
        throw std::out_of_range("Index out of range");
    }

    T temporary = data[firstIndex];
    data[firstIndex] = data[secondIndex];
    data[secondIndex] = temporary;
}

template <typename T>
void DynamicArray<T>::print() const
{
    for (int index = 0; index < currentSize; ++index)
    {
        std::cout << data[index] << " ";
    }
    std::cout << '\n';
}
