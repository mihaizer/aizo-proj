#pragma once

template <typename T>
class IStructure
{
public:
    virtual ~IStructure() = default;

    virtual int size() const = 0; // = 0 znaczy ze niema realizacji w bazowym klasie

    virtual T &operator[](int index) = 0;             // dostemp przez array[i]
    virtual const T &operator[](int index) const = 0; // dla const DynamicArray arr(5); arr[0] = 7; niemozna
    virtual void pushBack(const T &value) = 0;
    virtual void swap(int i, int j) = 0;
};
