#pragma once

#include "IStructure.h"

template <typename T>
class SinglyLinkedList;

template <typename T>
// Specjalna wersja sprawdzania posortowania dla listy jednokierunkowej.
bool isSortedAscending(const SinglyLinkedList<T> &values);

namespace InsertionSort
{
template <typename T>
// Pomocnik z dostepem friend dla wersji InsertionSort przeznaczonej dla listy.
struct SinglyLinkedListAccess;
}

namespace MergeSort
{
template <typename T>
// Pomocnik z dostepem friend dla wersji MergeSort przeznaczonej dla listy.
struct SinglyLinkedListAccess;
}

template <typename T>
class SinglyLinkedList : public IStructure<T>
{
private:
    // Wewnetrzny wezel listy jednokierunkowej.
    struct Node
    {
        T value;
        Node *next;
    };

    Node *head; // Pierwszy wezel listy albo nullptr, gdy lista jest pusta.
    Node *tail; // Ostatni wezel listy, aby pushBack dzialal w O(1).
    int currentSize; // Liczba przechowywanych elementow, a nie indeks ostatniego.

    // Usuwa wszystkie wezly i przywraca stan pustej listy.
    void clear();
    // Kopiuje wezly z innej listy z zachowaniem kolejnosci.
    void copyFrom(const SinglyLinkedList &other);
    // Zwraca wskaznik do wezla pod podanym indeksem.
    Node *nodeAt(int index);
    // Wersja const nodeAt do dostepu tylko do odczytu.
    const Node *nodeAt(int index) const;

    // Umozliwia helperom sortowania i funkcji walidujacej bezposredni dostep do wnetrza listy.
    friend struct InsertionSort::SinglyLinkedListAccess<T>;
    friend struct MergeSort::SinglyLinkedListAccess<T>;
    friend bool isSortedAscending<T>(const SinglyLinkedList<T> &values);

public:
    // Tworzy pusta liste.
    SinglyLinkedList();
    // Zwalnia wszystkie zaalokowane wezly.
    ~SinglyLinkedList() override;

    // Konstruktor kopiujacy wykonuje gleboka kopie wezlow.
    SinglyLinkedList(const SinglyLinkedList &other);
    // Zastepuje biezaca zawartosc gleboka kopia other.
    SinglyLinkedList &operator=(const SinglyLinkedList &other);

    // Zwraca biezaca liczbe elementow.
    int size() const override;
    // Dodaje wartosc na koniec listy.
    void pushBack(const T &value) override;

    // Zwraca dostep do wartosci pod podanym indeksem.
    T &operator[](int index) override;
    // Zwraca dostep tylko do odczytu do wartosci pod podanym indeksem.
    const T &operator[](int index) const override;

    // Zamienia miejscami wartosci w dwoch wezlach wskazanych indeksami.
    void swap(int i, int j) override;
};

#include "SinglyLinkedList.tpp"
