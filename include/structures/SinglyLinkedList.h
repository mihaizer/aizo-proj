#pragma once

#include "IStructure.h"

template <typename T>
class SinglyLinkedList;

template <typename T>
bool isSortedAscending(const SinglyLinkedList<T> &values);

namespace CocktailSort
{
template <typename T>
struct SinglyLinkedListAccess;
}

namespace InsertionSort
{
template <typename T>
struct SinglyLinkedListAccess;
}

namespace MergeSort
{
template <typename T>
struct SinglyLinkedListAccess;
}

template <typename T> // SinglyLinkedList<int> list; tutaj T oznacza typ przechowywanej wartosci.
class SinglyLinkedList : public IStructure<T> // Implementuje IStructure<T> jako lista jednokierunkowa.
{
private: // Wezly i wskazniki sa szczegolem implementacji listy.
    struct Node // Node przechowuje jedna wartosc i wskaznik na nastepny element listy.
    {
        T value; // node.value; przechowuje dane elementu.
        Node *next; // node.next; prowadzi do kolejnego wezla albo nullptr na koncu listy.
    };

    Node *head; // head; wskazuje pierwszy element listy.
    Node *tail; // tail; wskazuje ostatni element, zeby pushBack byl szybki.
    int currentSize; // list.size(); zwraca aktualna liczbe elementow.

    void clear(); // clear(); usuwa wszystkie wezly i ustawia pusta liste.
    void copyFrom(const SinglyLinkedList &other); // copyFrom(other); kopiuje elementy przez pushBack.
    Node *nodeAt(int index); // nodeAt(2); znajduje wezel pod indeksem do zapisu.
    const Node *nodeAt(int index) const; // nodeAt(2); znajduje wezel pod indeksem do odczytu const.

    friend struct CocktailSort::SinglyLinkedListAccess<T>; // CocktailSort::sort(list); helper przechodzi po wezlowach bez operator[].
    friend struct InsertionSort::SinglyLinkedListAccess<T>; // InsertionSort::sort(list); helper przepina wezly bez wolnego dostepu.
    friend struct MergeSort::SinglyLinkedListAccess<T>; // MergeSort::sort(list); helper moze przepinac prywatne wezly listy.
    friend bool isSortedAscending<T>(const SinglyLinkedList<T> &values); // isSortedAscending(list); sprawdza liste liniowo.

public: // Publiczne metody sa zgodne z interfejsem IStructure<T>.
    SinglyLinkedList(); // SinglyLinkedList<int> list; tworzy pusta liste.
    ~SinglyLinkedList() override; // delete ptr; nadpisuje wirtualny destruktor i zwalnia wezly.

    SinglyLinkedList(const SinglyLinkedList &other); // SinglyLinkedList<int> copy = list; tworzy niezalezna kopie.
    SinglyLinkedList &operator=(const SinglyLinkedList &other); // copy = list; zamienia zawartosc listy na kopie other.

    int size() const override; // int n = list.size(); nadpisuje IStructure<T>::size().
    void pushBack(const T &value) override; // list.pushBack(7); nadpisuje IStructure<T>::pushBack() i dodaje na koniec.

    T &operator[](int index) override; // list[0] = 10; nadpisuje IStructure<T>::operator[] do zapisu.
    const T &operator[](int index) const override; // int x = list[0]; nadpisuje operator[] do odczytu const.

    void swap(int i, int j) override; // list.swap(0, 1); nadpisuje IStructure<T>::swap() i zamienia wartosci wezlow.
};

#include "SinglyLinkedList.tpp"
