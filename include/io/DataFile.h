#pragma once

#include <string>

#include "structures/DynamicArray.h"
#include "structures/IStructure.h"

namespace DataFile
{
template <typename T>
bool readValues(const std::string &path, DynamicArray<T> &values, std::string &error); // DataFile::readValues("in.txt", data, error); wczytuje format: liczba elementow i kolejne wartosci.

template <typename T>
bool writeValues(const std::string &path, const IStructure<T> &values, std::string &error); // DataFile::writeValues("out.txt", data, error); zapisuje format zgodny z plikiem wejsciowym.
}

#include "DataFile.tpp"
