#pragma once

#include <cstring>
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

    // Konstruktor kopiujący (głęboka kopia)
    DynamicArray(const DynamicArray &other)
    {
        // 1. Kopiujemy rozmiar i pojemność.
        currentSize = other.currentSize;
        capacity = other.capacity;

        // 2. Przydzielamy nową pamięć.
        data = new T[capacity];

        // 3. Kopiujemy dane ze starej tablicy do nowej.
        std::memcpy(data, other.data, currentSize * sizeof(T));
    }

    // Operator przypisania kopiującego (głęboka kopia z czyszczeniem)
    DynamicArray &operator=(const DynamicArray &other)
    {
        // 1. Ochrona przed przypisaniem obiektu do samego siebie (arr = arr;)
        // Jeśli adresy obiektów są takie same, nic nie robimy,
        // żeby nie usunąć własnych danych.
        if (this == &other)
            return *this;

        // 2. Usuwamy stare dane, które znajdowały się w bieżącej tablicy.
        delete[] data;

        // 3. Kopiujemy nowy rozmiar i pojemność.
        currentSize = other.currentSize;
        capacity = other.capacity;

        // 4. Przydzielamy nową pamięć i kopiujemy dane.
        data = new T[capacity];
        std::memcpy(data, other.data, currentSize * sizeof(T));

        // Zwracamy referencję do bieżącego obiektu zgodnie ze standardową praktyką C++.
        return *this;
    }

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

            // Kopiujemy cały blok pamięci, ponieważ używamy prostych typów liczbowych.
            std::memcpy(newData, data, currentSize * sizeof(T));

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
