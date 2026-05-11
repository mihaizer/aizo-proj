#pragma once

#include <string>

#include "structures/IStructure.h"

namespace DataFile
{
// Funkcje pracuja na formacie z tresci zadania: pierwsza linia to liczba elementow,
// a kolejne linie zawieraja wartosci tego samego typu.
template <typename T, typename Structure>
bool readValues(const std::string &path, Structure &values, std::string &error); // DataFile::readValues<int>("in.txt", data, error); wczytuje wartosci do dowolnej struktury z pushBack().

template <typename T>
bool writeValues(const std::string &path, const IStructure<T> &values, std::string &error); // DataFile::writeValues("out.txt", data, error); zapisuje format zgodny z plikiem wejsciowym.
}

#include "DataFile.tpp"
