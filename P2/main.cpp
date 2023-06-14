#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

#include "heap.h"
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
    double openingPrice;
};

bool sortByTradePrice(const OptionData& a, const OptionData& b) {
    return a.tradePrice < b.tradePrice;
}

void processCSV(const std::string& filename, Node<OptionData>*& root) {
    std::vector<OptionData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "無法開啟檔案：" << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line); // 略過標題列

    while (std::getline(file, line)) {
        OptionData option;
        std::stringstream ss(line);
        std::string value;

        std::getline(ss, value, ',');
        option.tradeDate = value;

        std::getline(ss, value, ',');
        option.productCode = value;

        std::getline(ss, value, ',');
        option.strikePrice = std::stod(value);

        std::getline(ss, value, ',');
        option.expirationDate = value;

        std::getline(ss, value, ',');
        option.callPut = value;

        std::getline(ss, value, ',');
        option.tradeTime = value;

        std::getline(ss, value, ',');
        option.tradePrice = std::stod(value);

        std::getline(ss, value, ',');
        option.tradeQuantity = std::stoi(value);

        std::getline(ss, value, ',');
        option.openingPrice = std::stod(value);

        data.push_back(option);
    }

    file.close();

    // 鎖定互斥鎖以避免多執行序存取 root
    std::lock_guard<std::mutex> lockGuard(mutex);
    for (const OptionData& option : data) {
        root = HeapSort<OptionData>::insertNode(root, option);
    }
}

int main() {
    std::vector<std::string> filenames = {
        "OptionsDaily_2017_05_15.csv",
        "OptionsDaily_2017_05_16.csv",
        "OptionsDaily_2017_05_17.csv",
        "OptionsDaily_2017_05_18.csv",
        "OptionsDaily_2017_05_19.csv"
    };

    Node<OptionData>* root = nullptr;

    // 使用多執行序讀取並處理 CSV 檔案
    std::vector<std::thread> threads;
    for (const std::string& filename : filenames) {
        threads.emplace_back([&root, filename]() {
            processCSV(filename, root);
        });
    }
    // 等待所有執行緒完成
    for (std::thread& thread : threads) {
        thread.join();
    }

    // 使用成交價格進行排序
    HeapSort<OptionData>::heapSort(root);

    // 輸出排序結果 while removeMax
    while (root != nullptr) {
        std::cout << root->data.tradePrice << std::endl;
        root = HeapSort<OptionData>::removeMax(root);
    }
    

    return 0;
}