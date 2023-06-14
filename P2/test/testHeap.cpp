
#include <iostream>
#include <string>

#include "heap.h"

// test heap
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
struct CompareByTradePrice {
    bool operator()(const OptionData& a, const OptionData& b) {
        return a.tradePrice > b.tradePrice;
    }
};
int main() {

    // Node<int>* root = nullptr;
    // int n = 0;
    // while (true) {
    //     n++;
    //     std::cout << "N:" << n << std::endl;
    //     root = HeapSort<int, std::greater<int>>::insertNode(root, 45);
    // }

    // HeapSort<int, std::greater<int>>::printNode(root);

    // std::cout << "Heap: ";

    // Node<int>* current = root;  // 使用另一個指標變數來迭代節點

    // while (current != nullptr) {
    //     // std::cout << current->data << " ";
    //     current = HeapSort<int, std::greater<int>>::removeMax(current);
    // }
    // std::cout << std::endl;

    return 0;
}