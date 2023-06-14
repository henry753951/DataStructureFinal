

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>

using namespace std;

template <typename KeyType, typename ValueType>
class HashTable {
   public:
    explicit HashTable(size_t size) : bucketSize(size), count(0) {
        buckets = new std::pair<KeyType, ValueType>[bucketSize];
    }

    ~HashTable() {
        delete[] buckets;
    }

    void insert(const KeyType& key, const ValueType& value) {
        size_t index = hashFunction(key) % bucketSize;
        size_t startIndex = index;
        while (buckets[index].first != KeyType() && buckets[index].first != key) {
            index = (index + 1) % bucketSize;
            if (index == startIndex) {
                // Hash table is full
                std::cerr << "Hash table is full" << std::endl;
                return;
            }
        }
        buckets[index] = std::make_pair(key, value);
        count++;
    }

    bool find(const KeyType& key, ValueType& value) {
        size_t index = hashFunction(key) % bucketSize;
        size_t startIndex = index;
        while (buckets[index].first != KeyType()) {
            if (buckets[index].first == key) {
                value = buckets[index].second;
                return true;
            }
            index = (index + 1) % bucketSize;
            if (index == startIndex) {
                break;
            }
        }
        return false;
    }

    void remove(const KeyType& key) {
        size_t index = hashFunction(key) % bucketSize;
        size_t startIndex = index;
        while (buckets[index].first != KeyType()) {
            if (buckets[index].first == key) {
                // Shift elements to fill the gap
                size_t nextIndex = (index + 1) % bucketSize;
                while (buckets[nextIndex].first != KeyType()) {
                    size_t hash = hashFunction(buckets[nextIndex].first) % bucketSize;
                    if ((nextIndex > index && (hash <= index || hash > nextIndex)) ||
                        (nextIndex < index && (hash <= index && hash > nextIndex))) {
                        buckets[index] = buckets[nextIndex];
                        index = nextIndex;
                    }
                    nextIndex = (nextIndex + 1) % bucketSize;
                }
                buckets[index] = std::make_pair(KeyType(), ValueType());
                count--;
                return;
            }
            index = (index + 1) % bucketSize;
            if (index == startIndex) {
                break;
            }
        }
    }

    size_t size() const {
        return count;
    }

   private:
    size_t bucketSize;
    std::pair<KeyType, ValueType>* buckets;
    size_t count;

    size_t hashFunction(const KeyType& key) {
        return std::hash<KeyType>{}(key);
    }
};

#endif