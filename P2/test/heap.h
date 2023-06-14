
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

template <typename T, typename Compare>
class HeapSort {
   public:
    // 建立最大堆
    static void heapify(Node<T>* root) {
        if (root == nullptr)
            return;

        Node<T>* largest = root;
        Node<T>* left = root->left;
        Node<T>* right = root->right;

        if (left != nullptr && Compare()(left->data, largest->data))
            largest = left;

        if (right != nullptr && Compare()(right->data, largest->data))
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

        if (Compare()(value, root->data)) {
            std::swap(value, root->data);
            root->left = insertNode(root->left, value);
        } else {
            root->right = insertNode(root->right, value);
        }
        heapify(root);
        return root;
    }

    // 將節點排序並輸出結果
    static void heapSort(Node<T>* root) {
        heapify(root);
    }

    // 移除最大值節點並重建最大堆
    static Node<T>* removeMax(Node<T>* root) {
        if (root == nullptr)
            return nullptr;

        if (root->left == nullptr && root->right == nullptr) {
            delete root;
            return nullptr;
        }

        if (root->left == nullptr) {
            Node<T>* temp = root->right;
            delete root;
            return temp;
        }

        if (root->right == nullptr) {
            Node<T>* temp = root->left;
            delete root;
            return temp;
        }

        if (Compare()(root->left->data, root->right->data)) {
            std::swap(root->data, root->right->data);
            root->right = removeMax(root->right);
        } else {
            std::swap(root->data, root->left->data);
            root->left = removeMax(root->left);
        }
        heapify(root);
        return root;
    }

    // 輸出節點
    static void printNode(Node<T>* root) {
        if (root == nullptr)
            return;

        std::cout << root->data << " ";
        printNode(root->left);
        printNode(root->right);
    }
};


#endif