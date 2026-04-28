#pragma once

template <typename Structure>
long double BucketSort<Structure>::numericValue(const ValueType &value) const
{
    if constexpr (std::is_same<ValueType, char>::value)
    {
        return static_cast<long double>(static_cast<unsigned char>(value));
    }
    else
    {
        return static_cast<long double>(value);
    }
}

template <typename Structure>
int BucketSort<Structure>::calculateBucketIndex(
    const ValueType &value,
    long double minValue,
    long double range,
    int bucketCount) const
{
    const long double normalized = (numericValue(value) - minValue) / range;
    int bucketIndex = static_cast<int>(normalized * static_cast<long double>(bucketCount - 1));

    if (bucketIndex < 0)
    {
        bucketIndex = 0;
    }
    else if (bucketIndex >= bucketCount)
    {
        bucketIndex = bucketCount - 1;
    }

    return bucketIndex;
}

template <typename Structure>
void BucketSort<Structure>::sortBucket(DynamicArray<ValueType> &bucket) const
{
    const int bucketSize = bucket.size();
    for (int index = 1; index < bucketSize; ++index)
    {
        const ValueType value = bucket[index];
        int sortedIndex = index - 1;

        while (sortedIndex >= 0 && value < bucket[sortedIndex])
        {
            bucket[sortedIndex + 1] = bucket[sortedIndex];
            --sortedIndex;
        }

        bucket[sortedIndex + 1] = value;
    }
}

template <typename Structure>
void BucketSort<Structure>::sort(Structure &data) const
{
    const int dataSize = data.size();
    if (dataSize < 2)
    {
        return;
    }

    long double minValue = numericValue(data[0]);
    long double maxValue = minValue;
    for (int index = 1; index < dataSize; ++index)
    {
        const long double currentValue = numericValue(data[index]);
        if (currentValue < minValue)
        {
            minValue = currentValue;
        }
        if (currentValue > maxValue)
        {
            maxValue = currentValue;
        }
    }

    const long double range = maxValue - minValue;
    if (range == 0.0L)
    {
        return;
    }

    int bucketCount = dataSize;
    if (bucketCount > 256)
    {
        bucketCount = 256;
    }

    DynamicArray<ValueType> *buckets = new DynamicArray<ValueType>[bucketCount];
    for (int index = 0; index < dataSize; ++index)
    {
        const int bucketIndex = calculateBucketIndex(data[index], minValue, range, bucketCount);
        buckets[bucketIndex].pushBack(data[index]);
    }

    int outputIndex = 0;
    for (int bucketIndex = 0; bucketIndex < bucketCount; ++bucketIndex)
    {
        sortBucket(buckets[bucketIndex]);
        const int bucketSize = buckets[bucketIndex].size();
        for (int index = 0; index < bucketSize; ++index)
        {
            data[outputIndex] = buckets[bucketIndex][index];
            ++outputIndex;
        }
    }

    delete[] buckets;
}
