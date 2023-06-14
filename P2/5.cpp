#include <mingw.mutex.h>
#include <mingw.thread.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#include "HeapSort.h"
#include "hashTable.h"
#include "timer.h"
#include "vector.h"

int n = 0;
std::mutex mtx;
std::string simplifyDecimalToString(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    std::string result = oss.str();
    size_t pos = result.find('.');
    if (pos != std::string::npos) {
        std::string decimalPart = result.substr(pos + 1);
        if (decimalPart == "000000") {
            result = result.substr(0, pos);
        }
    }
    return result;
}

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

    printf("Thread started, Reading %s\n", filename.c_str());
    Timer timer;  // 計時開始
    timer.start();

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

    timer.stop();  // 計時結束
    std::cout << "Read " << nn << " lines - Using " << timer.duration<std::chrono::milliseconds>() << " ms\n|File:" << filename << "\n\n";

    file.close();
}

int main() {
    Timer totalTimer;  // 計時開始
    totalTimer.start();

    std::vector<std::string> filenames = {
        "OptionsDaily_2017_05_15.csv",
        "OptionsDaily_2017_05_16.csv",
        "OptionsDaily_2017_05_17.csv",
        "OptionsDaily_2017_05_18.csv",
        "OptionsDaily_2017_05_19.csv",
    };

    OptionData* arr = new OptionData[2000000];

    Timer timer;  // 計時開始
    timer.start();

    // 使用多執行序讀取並處理 CSV 檔案
    std::vector<std::thread> threads;
    for (const std::string& filename : filenames) {
        std::cout << "Creating thread for " << filename << "\n";
        threads.emplace_back([&arr, filename]() {
            processCSV("data/" + filename, arr);
        });
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    timer.stop();  // 計時結束
    std::cout << "===All threads reading finished!===" << std::endl;
    std::cout << " >>> Load data : " << timer.duration<std::chrono::milliseconds>() << " ms\n\n";

    // === 排序 ===
    Timer timer1;  // 計時開始
    timer1.start();
    HeapSort hp = HeapSort();
    hp.heapSort(arr, n, true);
    timer1.stop();  // 計時結束
    std::cout << " >>> HeapSort : " << timer1.duration<std::chrono::milliseconds>() << " ms\n\n";
    // === === ===




    // 1
    std::cout << "=========   1   =========" << std::endl;
    // === 去重複 ===
    HashTable<std::string, int> ht = HashTable<std::string, int>(n * 2);
    Timer timer2;  // 計時開始
    timer2.start();
    int count = 0;
    for (int i = 0; i < n; ++i) {
        std::string key = arr[i].productCode + '_' + simplifyDecimalToString(arr[i].strikePrice) + '_' + arr[i].expirationDate + '_' + arr[i].callPut;
        int index = -1;
        // std::cout << key << std::endl;
        ht.find(key, index);
        if (index == -1) {
            count++;
            ht.insert(key, i);
        }
    }
    timer2.stop();  // 計時結束
    std::cout << " >>> HashTable and de-Duplicate : " << timer2.duration<std::chrono::milliseconds>() << " ms\n\n";
    // === === ===


    std::cout << "Total : " << n << std::endl;
    std::cout << "non-Duplicate : " << count << std::endl;
    std::cout << "=========================" << std::endl<< std::endl;
    // 2 - 3 - 4
    std::cout << "========= 2,3,4 =========" << std::endl;
    Timer timer3;  // 計時開始
    timer3.start();
    std::string search[] = {"TXO_1000_201706_P", "TXO_9500_201706_C", "GIO_5500_201706_C"};

    for (int i = 0; i <= 2; i++) {
        int index = -1;
        ht.find(search[i], index);
        if (index == -1)
            std::cout << search[i] << " not found" << std::endl;
        else
            std::cout << search[i] << " Found at index : " << index << std::endl;
    }
    timer3.stop();  // 計時結束
    std::cout << " >>> Search : " << timer3.duration<std::chrono::milliseconds>() << " ms\n\n";
    std::cout << "=========================" << std::endl<< std::endl;


    totalTimer.stop();  // 計時結束
    std::cout << " >>> Total time : " << totalTimer.duration<std::chrono::milliseconds>() << " ms\n\n";
    return 0;
}