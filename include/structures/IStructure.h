#pragma once

template <typename T>
class IStructure
{
public:
    virtual ~IStructure() = default;

    virtual int size() const = 0; // Interfejs wymaga znajomosci aktualnego rozmiaru struktury.

    virtual T &operator[](int index) = 0;             // Dostep do elementu do zapisu przez indeks.
    virtual const T &operator[](int index) const = 0; // Dostep do elementu do odczytu przez indeks.
    virtual void pushBack(const T &value) = 0;
    virtual void swap(int i, int j) = 0;
};
