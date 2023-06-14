#include <iostream>

#include "../hashTable.h"

int main() {
    HashTable<std::string, int> table(10);

    table.insert("apple", 5);
    table.insert("banana", 3);
    table.insert("orange", 8);

    int value;
    if (table.find("banana", value)) {
        std::cout << "Value of banana: " << value << std::endl;
    }
}