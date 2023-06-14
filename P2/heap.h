
#ifndef HEAP_H
#define HEAP_H

#include <algorithm>
#include <iostream>
#include <vector>

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;

    explicit Node(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class HeapSort {
   public:
    // 建立最大堆
    static void heapify(Node<T>* root) {
        if (root == nullptr)
            return;

        Node<T>* largest = root;
        Node<T>* left = root->left;
        Node<T>* right = root->right;

        if (left != nullptr && left->data > largest->data)
            largest = left;

        if (right != nullptr && right->data > largest->data)
            largest = right;

        if (largest != root) {
            std::swap(root->data, largest->data);
            heapify(largest);
        }
    }

    // 建立節點（連結串列）
    static Node<T>* createNode(T value) {
        return new Node<T>(value);
    }

    // 插入節點並建立最大堆
    static Node<T>* insertNode(Node<T>* root, T value) {
        if (root == nullptr)
            return createNode(value);

        if (value > root->data) {
            std::swap(value, root->data);
            root->left = insertNode(root->left, value);
        } else {
            root->right = insertNode(root->right, value);
        }

        return root;
    }

    // 將節點排序並輸出結果
    static void heapSort(Node<T>* root) {
        heapify(root);

        while (root != nullptr) {
            std::cout << root->data << " ";
            root = removeMax(root);
        }
    }

    // 移除最大值節點並重建最大堆
    static Node<T>* removeMax(Node<T>* root) {
        if (root == nullptr)
            return nullptr;

        Node<T>* maxNode = root;

        if (root->left != nullptr && root->left->data > maxNode->data)
            maxNode = root->left;

        if (root->right != nullptr && root->right->data > maxNode->data)
            maxNode = root->right;

        if (maxNode != root) {
            std::swap(root->data, maxNode->data);
            maxNode = removeMax(maxNode);
        } else {
            delete root;
            root = nullptr;
        }

        return maxNode;
    }
};

#endif