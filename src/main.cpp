#include <iostream>
#include <stdexcept>

#include "Parameters.h"
#include "structures/DynamicArray.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (Parameters::readParameters(argc - 1, argv + 1) != 0)
        {
            std::cerr << "Failed to parse parameters.\n";
            return 1;
        }

        if (Parameters::runMode == Parameters::RunModes::help)
        {
            Parameters::help();
            return 0;
        }

        std::cout << "Library version: " << Parameters::getVersion() << "\n";
        Parameters::printParameters();
        return 0;
    }

    // Keep the current local test code when no library arguments are provided.
    int sizeFromFile = 5;

    DynamicArray<int> myArr(sizeFromFile);
    myArr[0] = 76;
    myArr[1] = -142;
    myArr[2] = 0;
    myArr[3] = 2138;
    myArr[4] = -4;

    std::cout << "Rozmiar tablicy: " << myArr.size() << "\n";
    std::cout << "Element pod indeksem 3: " << myArr[3] << "\n";

    std::cout << "Cala tablica: ";
    myArr.print();

    DynamicArray<char> charArr(3);
    charArr[0] = 'A';
    charArr[1] = 'B';
    charArr[2] = 'C';

    std::cout << "Pierwszy znak: " << charArr[0] << "\n";
    charArr.print();

    std::cout << "\nTest pushBack i powiekszania tablicy:\n";
    DynamicArray<int> pushArr;
    pushArr.pushBack(10);
    pushArr.pushBack(20);
    pushArr.pushBack(30);
    std::cout << "Rozmiar po pushBack: " << pushArr.size() << "\n";
    std::cout << "Pojemnosc po pushBack: " << pushArr.getCapacity() << "\n";
    std::cout << "Zawartosc pushArr: ";
    pushArr.print();

    std::cout << "\nTest swap:\n";
    pushArr.swap(0, 2);
    std::cout << "Po swap(0, 2): ";
    pushArr.print();

    std::cout << "\nTest konstruktora kopiujacego:\n";
    DynamicArray<int> copiedArr = pushArr;
    copiedArr[0] = 999;
    std::cout << "Oryginal po zmianie kopii: ";
    pushArr.print();
    std::cout << "Kopia po zmianie: ";
    copiedArr.print();

    std::cout << "\nTest operatora przypisania:\n";
    DynamicArray<int> assignedArr;
    assignedArr.pushBack(-1);
    assignedArr = pushArr;
    assignedArr[1] = 777;
    std::cout << "Oryginal po zmianie przypisanej tablicy: ";
    pushArr.print();
    std::cout << "Tablica po przypisaniu i zmianie: ";
    assignedArr.print();

    std::cout << "\nTest przypisania do samego siebie:\n";
    DynamicArray<int> &sameArr = assignedArr;
    assignedArr = sameArr;
    std::cout << "Po assignedArr = assignedArr: ";
    assignedArr.print();

    std::cout << "\nTest const operator[]:\n";
    const DynamicArray<int> constArr = pushArr;
    std::cout << "constArr[1]: " << constArr[1] << "\n";

    std::cout << "\nTest wyjscia poza zakres:\n";
    try
    {
        std::cout << pushArr[99] << "\n";
    }
    catch (const std::out_of_range &error)
    {
        std::cout << "Zlapano wyjatek: " << error.what() << "\n";
    }

    return 0;
}
