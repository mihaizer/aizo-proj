#include <iostream>

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

    // DynamicArray<int> anotherArr(5);
    // anotherArr = myArr;
    return 0;
}
