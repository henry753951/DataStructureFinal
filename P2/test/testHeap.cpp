
#include <iostream>
#include <string>
#include "heap.h"

// test heap

int main() {
    Node<int>* root = nullptr;
    root = HeapSort<int, std::greater<int>>::insertNode(root, 3);
    root = HeapSort<int, std::greater<int>>::insertNode(root, 2);
    root = HeapSort<int, std::greater<int>>::insertNode(root, 15);
    root = HeapSort<int, std::greater<int>>::insertNode(root, 5);
    root = HeapSort<int, std::greater<int>>::insertNode(root, 4);
    root = HeapSort<int, std::greater<int>>::insertNode(root, 45);

    HeapSort<int, std::greater<int>>::printNode(root);
    
    
    

    // std::cout << "Heap: ";

    // Node<int>* current = root;  // 使用另一個指標變數來迭代節點

    // while (current != nullptr) {
    //     // std::cout << current->data << " ";
    //     current = HeapSort<int, std::greater<int>>::removeMax(current);
    // }
    // std::cout << std::endl;

    return 0;
}