#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class DynamicArray
{
private:
    T *data;
    int size;

public:
    /// Konstruktor.
    /// \param n długość (liczba elementów) tablicy dynamicznej.
    DynamicArray(int n)
    {
        if (n <= 0)
            throw std::invalid_argument("Size must be > 0");
        size = n;
        data = new T[size]; // Dynamiczna alokacja
    }

    // Destruktor (zwalnianie pamięci)
    ~DynamicArray()
    {
        delete[] data;
    }

    // Zakaz kopiowania struktury (ochrona przed wyciekami i awariami)
    DynamicArray(const DynamicArray &) = delete;
    DynamicArray &operator=(const DynamicArray &) = delete;

    // Dostęp po indeksie (jak w zwykłej tablicy)
    T &operator[](int index)
    {
        return data[index];
    }

    // Pobierz rozmiar
    int getSize() const
    {
        return size;
    }

    // Metoda do wypisywania (przydatna do debugowania)
    void print() const
    {
        for (int i = 0; i < size; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
};
