#include <mingw.mutex.h>
#include <mingw.thread.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "heap.h"
#include "vector.h"


int n = 0;
std::mutex mtx;

struct CompareByTradePrice {
    bool operator()(const OptionData& a, const OptionData& b) {
        return a.tradePrice > b.tradePrice;
    }
};

void processCSV(const std::string& filename, OptionData*& arr) {
    int nn = 0;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "無法開啟檔案：" << filename << std::endl;
        return;
    }

    std::cout << "Reading " << filename << "...\n";

    std::string line;
    std::getline(file, line);  // 略過標題列
    std::getline(file, line);  // 略過---
    while (std::getline(file, line)) {
        nn++;
        OptionData option;
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
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
        option.openingPrice = value;

        
        std::lock_guard<std::mutex> lockGuard(mtx);
        arr[n++] = option;


    }
    std::cout << "Read " << nn << " lines\n";

    file.close();
}

int main() {
    std::vector<std::string> filenames = {
        "OptionsDaily_2017_05_15.csv",
        "OptionsDaily_2017_05_16.csv",
        "OptionsDaily_2017_05_17.csv",
        "OptionsDaily_2017_05_18.csv",
        "OptionsDaily_2017_05_19.csv",
    };

    OptionData* arr = new OptionData[2000000];

    // 使用多執行序讀取並處理 CSV 檔案
    std::vector<std::thread> threads;
    for (const std::string& filename : filenames) {
        std::cout << "Creating thread for " << filename << std::endl;
        threads.emplace_back([&arr, filename]() {
            processCSV("data/" + filename, arr);
        });
    }
    // 等待所有執行緒完成
    for (std::thread& thread : threads) {
        thread.join();
    }
    std::cout << "All threads finished\n";
    // 使用成交價格進行排序
    // HeapSort<OptionData, CompareByTradePrice>::heapSort(root);

    // // 輸出排序結果 while removeMax
    // while (root != nullptr) {
    //     std::cout << root->data.tradePrice << std::endl;
    //     root = HeapSort<OptionData, CompareByTradePrice>::removeMax(root);
    // }

    return 0;
}