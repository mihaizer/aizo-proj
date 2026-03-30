#include <iostream>

#include "structures/DynamicArray.h"

int main() {
    // Primer 1: bazovy tip int
    int sizeFromFile = 5;

    DynamicArray<int> myArr(sizeFromFile);
    myArr[0] = 76;
    myArr[1] = -142;
    myArr[2] = 0;
    myArr[3] = 2138;
    myArr[4] = -4;

    std::cout << "Rozmiar tablicy: " << myArr.getSize() << "\n";
    std::cout << "Element pod indeksem 3: " << myArr[3] << "\n";

    std::cout << "Cala tablica: ";
    myArr.print();

    // Primer 2: inny typ danych
    DynamicArray<char> charArr(3);
    charArr[0] = 'A';
    charArr[1] = 'B';
    charArr[2] = 'C';

    std::cout << "Pierwszy znak: " << charArr[0] << "\n";

    return 0;
}
