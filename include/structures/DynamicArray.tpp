#pragma once

#include <iostream>

template <typename T>
DynamicArray<T>::DynamicArray(int n)
{
    // Rozmiar jest walidowany przed utworzeniem struktury, wiec tutaj zakladamy poprawne n.
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
    // Kopia musi miec wlasny bufor, zeby obiekty nie dzielily pamieci.
    currentSize = other.currentSize;
    capacity = other.capacity;
    data = new T[capacity];
    for (int i = 0; i < currentSize; i++)
    {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T> &DynamicArray<T>::operator=(const DynamicArray &other)
{
    // Najpierw zwalniamy stary bufor, potem alokujemy nowy i kopiujemy dane.
    if (this == &other)
    {
        return *this;
    }

    delete[] data;

    currentSize = other.currentSize;
    capacity = other.capacity;
    data = new T[capacity];
    for (int i = 0; i < currentSize; i++)
    {
        data[i] = other.data[i];
    }

    return *this;
}

template <typename T>
T &DynamicArray<T>::operator[](int index)
{
    // Algorytmy i generator pracuja na zweryfikowanych zakresach indeksow.
    return data[index];
}

template <typename T>
const T &DynamicArray<T>::operator[](int index) const
{
    // Algorytmy i generator pracuja na zweryfikowanych zakresach indeksow.
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
    // Gdy brak miejsca, zwiekszamy pojemnosc o 1 i przepisujemy zawartosc.
    if (currentSize == capacity)
    {
        int newCapacity = capacity + 1;
        T *newData = new T[newCapacity];

        for (int i = 0; i < currentSize; i++)
        {
            newData[i] = data[i];
        }

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
    // Zamiana elementow jest prosta, bo tablica ma bezposredni dostep do komorek pamieci.
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
