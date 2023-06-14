#ifndef HEAP_H
#define HEAP_H

#include <iostream>


using namespace std;

struct OptionData {
    std::string tradeDate;
    std::string productCode;
    std::string strikePrice;
    std::string expirationDate;
    std::string callPut;
    std::string tradeTime;
    double tradePrice;
    int tradeQuantity;
    std::string openingPrice;
};

template <typename T, typename Compare>
class HeapSort {
public:
    HeapSort() {

    }

    void heapify(T arr[], int n, int i, Compare compare, bool ascending) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        // If left child is larger than root
        if (l < n && compare(arr[l], arr[largest], ascending))
            largest = l;

        // If right child is larger than largest so far
        if (r < n && compare(arr[r], arr[largest], ascending))
            largest = r;

        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify(arr, n, largest, compare, ascending);
        }
    }

    void heapSort(T arr[], int n, Compare compare, bool ascending = false) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i, compare, ascending);

        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify(arr, i, 0, compare, ascending);
        }
    }
};

#endif  // HEAP_H