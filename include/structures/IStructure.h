#pragma once

class IStructure
{
public:
    virtual ~IStructure() = default;

    virtual int size() const = 0; // = 0 znaczy ze niema realizacji w bazowym klasie

    virtual int &operator[](int index) = 0;             // dostemp przez array[i]
    virtual const int &operator[](int index) const = 0; // dla const DynamicArray arr(5); arr[0] = 7; niemozna
    virtual void pushBack(const int &value) = 0;
    virtual void swap(int i, int j) = 0;
};
