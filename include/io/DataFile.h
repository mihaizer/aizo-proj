#pragma once

#include <string>

namespace DataFile
{
template <typename T, typename Structure>
bool load(const std::string &filePath, Structure &data, int &loadedSize, std::string &errorMessage);

template <typename T, typename Structure>
bool write(const std::string &filePath, const Structure &data, std::string &errorMessage);
}

#include "DataFile.tpp"
