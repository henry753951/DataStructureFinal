
#ifndef HEAP_H
#define HEAP_H

#include <algorithm>
#include <iostream>
#include <vector>


using namespace std;

struct OptionData {
    std::string tradeDate;
    std::string productCode;
    double strikePrice;
    std::string expirationDate;
    std::string callPut;
    std::string tradeTime;
    double tradePrice;
    int tradeQuantity;
    std::string openingPrice;
};

class HeapSort {
public:
	HeapSort() {

	}
    void heapify(OptionData arr[], int n, int i, bool ascending) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;

        // If left child is larger than root
        if (l < n && ((ascending && arr[l].tradePrice > arr[largest].tradePrice) || (!ascending && arr[l].tradePrice < arr[largest].tradePrice)))
            largest = l;

        // If right child is larger than largest so far
        if (r < n && ((ascending && arr[r].tradePrice > arr[largest].tradePrice) || (!ascending && arr[r].tradePrice < arr[largest].tradePrice)))
            largest = r;

        // If largest is not root
        if (largest != i) {
            swap(arr[i], arr[largest]);

            // Recursively heapify the affected sub-tree
            heapify(arr, n, largest, ascending);
        }
    }
    void heapSort(OptionData arr[], int n, bool ascending = false) {
        // Build heap (rearrange array)
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i, ascending);

        // One by one extract an element from heap
        for (int i = n - 1; i > 0; i--) {
            // Move current root to end
            swap(arr[0], arr[i]);

            // call max heapify on the reduced heap
            heapify(arr, i, 0, ascending);
        }
    }
};


#endif