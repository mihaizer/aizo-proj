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

public:
    // Konstruktor
    DynamicArray(int n)
    {
        if (n <= 0)
            throw std::invalid_argument("Size must be > 0");
        currentSize = n;
        data = new T[currentSize];
    }

    // Destruktor (zwalnianie pamięci)
    ~DynamicArray() override
    {
        delete[] data;
    }

    // Zakaz kopiowania struktury (ochrona przed wyciekami i awariami)
    DynamicArray(const DynamicArray &) = delete;
    DynamicArray &operator=(const DynamicArray &) = delete;

    // Dostęp po indeksie (jak w zwykłej tablicy)
    T &operator[](int index) override
    {
        if (index < 0 || index >= currentSize)
            throw std::out_of_range("Index out of range");

        return data[index];
    }

    const T &operator[](int index) const override
    {
        if (index < 0 || index >= currentSize)
            throw std::out_of_range("Index out of range");

        return data[index];
    }

    // Pobierz rozmiar
    int size() const override
    {
        return currentSize;
    }

    int getSize() const
    {
        return currentSize;
    }

    // тут какой то буллшит
    void pushBack(const T &value) override
    {
        T *newData = new T[currentSize + 1];

        for (int i = 0; i < currentSize; i++)
        {
            newData[i] = data[i];
        }

        newData[currentSize] = value;

        delete[] data;
        data = newData;
        currentSize++;
    }

    void swap(int i, int j) override
    {
        if (i < 0 || i >= currentSize || j < 0 || j >= currentSize)
            throw std::out_of_range("Index out of range");

        T temp = data[i];
        data[i] = data[j];
        data[j] = temp;
    }

    // Metoda do wypisywania (przydatna do debugowania)
    void print() const
    {
        for (int i = 0; i < currentSize; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};
