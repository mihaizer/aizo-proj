#pragma once // Dolacza ten header tylko raz w jednym pliku .cpp.

#include "IStructure.h" // Uzywa wspolnego interfejsu struktury: size(), operator[], pushBack(), swap().

template <typename T> // DynamicArray<int> arr; tutaj T oznacza typ int.
class DynamicArray : public IStructure<T> // Implementuje interfejs IStructure<T> dla tablicy dynamicznej.
{
private: // Te pola sa dostepne tylko wewnatrz klasy DynamicArray.
    T *data; // data[0] przechowuje pierwszy element; jest to recznie zaalokowana tablica new T[capacity].
    int currentSize; // arr.size(); zwraca liczbe elementow aktualnie uzywanych w tablicy.
    int capacity; // arr.getCapacity(); zwraca liczbe miejsc aktualnie zaalokowanych w pamieci.

public: // Te metody mozna wywolywac z main.cpp oraz innych klas.
    DynamicArray(int n = 0); // DynamicArray<int> arr(5); tworzy tablice na 5 elementow, domyslnie pusta.
    ~DynamicArray() override; // delete ptr; nadpisuje wirtualny destruktor z IStructure<T> i zwalnia data.

    DynamicArray(const DynamicArray &other); // DynamicArray<int> copy = arr; tworzy niezalezna kopie tablicy.
    DynamicArray &operator=(const DynamicArray &other); // copy = arr; zastepuje zawartosc copy kopia arr.

    T &operator[](int index) override; // arr[0] = 10; nadpisuje IStructure<T>::operator[] do zapisu.
    const T &operator[](int index) const override; // int x = constArr[0]; nadpisuje operator[] do odczytu z obiektu const.

    int size() const override; // int n = arr.size(); nadpisuje IStructure<T>::size().
    void pushBack(const T &value) override; // arr.pushBack(7); nadpisuje IStructure<T>::pushBack() i dodaje element na koniec.
    void swap(int i, int j) override; // arr.swap(0, 2); nadpisuje IStructure<T>::swap() i zamienia elementy miejscami.

    void print() const; // arr.print(); wypisuje elementy w konsoli do prostego testu.
    int getCapacity() const; // int cap = arr.getCapacity(); zwraca aktualnie zaalokowana pojemnosc.
};

#include "DynamicArray.tpp" // Dolacza implementacje szablonu, aby kompilator widzial kod metod.
