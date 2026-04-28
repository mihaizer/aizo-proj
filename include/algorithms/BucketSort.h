#pragma once

#include <type_traits>

#include "structures/DynamicArray.h"

template <typename Structure>
class BucketSort
{
private:
    using ValueType = typename Structure::ValueType;

    long double numericValue(const ValueType &value) const;
    int calculateBucketIndex(const ValueType &value, long double minValue, long double range, int bucketCount) const;
    void sortBucket(DynamicArray<ValueType> &bucket) const;

public:
    void sort(Structure &data) const;
};

#include "BucketSort.tpp"
