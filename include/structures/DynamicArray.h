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

public:
    // Konstruktor
    // Dodana została wartość domyślna (n = 0), aby można było utworzyć pustą tablicę
    DynamicArray(int n = 0)
    {
        if (n < 0)
            throw std::invalid_argument("Size must be >= 0");

        currentSize = n;

        if (n == 0)
            // zeby mozna było zwiekszyc przy mnozeniu na 2
            capacity = 1;
        else
            capacity = n;

        data = new T[capacity];
    }

    // Destruktor (zwalnianie pamięci)
    ~DynamicArray() override
    {
        delete[] data;
    }

    // Zakaz kopiowania struktury (ochrona przed wyciekami i awariami)
    // todo fix dodac sesowny mechanizm kopiowania
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

    // Dodawanie elementu na koniec tablicy dynamicznej
    void pushBack(const T &value) override
    {
        // Jeżeli nie ma już wolnego miejsca, trzeba zaalokować większą tablicę
        if (currentSize == capacity)
        {
            int newCapacity = capacity * 2;
            T *newData = new T[newCapacity];

            // Kopiowanie starych danych do nowej, większej tablicy
            for (int i = 0; i < currentSize; i++)
            {
                newData[i] = data[i];
            }

            // Zwolnienie starej pamięci i podmiana wskaźnika
            delete[] data;
            data = newData;
            capacity = newCapacity;
        }

        // Po zapewnieniu miejsca można dopisać nowy element na końcu
        data[currentSize] = value;
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

    // Metoda pomocnicza do sprawdzania aktualnej pojemności tablicy
    int getCapacity() const
    {
        return capacity;
    }
};
