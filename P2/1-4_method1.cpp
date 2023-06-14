#include <mingw.mutex.h>
#include <mingw.thread.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "HeapSort.h"
#include "hashTable.h"
#include "tickData.h"
#include "timer.h"
#include "vector.h"

int n = 0;
std::mutex mtx;

struct CompareByTradePrice {
    bool operator()(const OptionData& data1, const OptionData& data2, bool ascending) const {
        return ascending ? data1.tradePrice < data2.tradePrice : data1.tradePrice > data2.tradePrice;
    }
};

struct CompareByTradeDateTimePrice {
    bool operator()(const OptionData& data1, const OptionData& data2, bool ascending) const {
        if (data1.tradeDate == data2.tradeDate) {
            if (data1.tradeTime == data2.tradeTime) {
                return ascending ? data1.tradePrice < data2.tradePrice : data1.tradePrice > data2.tradePrice;
            }
            return ascending ? data1.tradeTime < data2.tradeTime : data1.tradeTime > data2.tradeTime;
        }
        return ascending ? data1.tradeDate < data2.tradeDate : data1.tradeDate > data2.tradeDate;
    }
};

void removeSpaces(string& s) {
    size_t pos = 0;
    while ((pos = s.find(' ', pos)) != string::npos) {
        s.erase(pos, 1);
    }
}

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
        removeSpaces(line);
        std::stringstream ss(line);
        std::string value;
        std::getline(ss, value, ',');
        option.tradeDate = value;

        std::getline(ss, value, ',');
        option.productCode = value;

        std::getline(ss, value, ',');
        option.strikePrice = value;

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

    Vector<std::string> filenames = {
        "OptionsDaily_2017_05_15.csv",
        "OptionsDaily_2017_05_16.csv",
        "OptionsDaily_2017_05_17.csv",
        "OptionsDaily_2017_05_18.csv",
        "OptionsDaily_2017_05_19.csv",
    };

    OptionData* arr = new OptionData[1500000];

    Timer timer;  // 計時開始
    timer.start();

    // 使用多執行序讀取並處理 CSV 檔案
    std::thread* threads = new std::thread[filenames.getSize()];

    for (size_t i = 0; i < filenames.getSize(); ++i) {
        const std::string& filename = filenames[i];
        std::cout << "Creating thread for " << filename << "\n";
        threads[i] = std::thread([&arr, filename]() {
            processCSV("data/" + filename, arr);
        });
    }

    for (size_t i = 0; i < filenames.getSize(); ++i) {
        threads[i].join();
    }

    timer.stop();  // 計時結束
    std::cout << "===All threads reading finished!===" << std::endl;
    std::cout << " >>> Load data : " << timer.duration<std::chrono::milliseconds>() << " ms\n\n";
    // === 排序 ===
    Timer timer1;  // 計時開始
    timer1.start();
    HeapSort<OptionData, CompareByTradePrice> hs;
    hs.heapSort(arr, n, CompareByTradePrice());
    timer1.stop();  // 計時結束
    std::cout << " >>> HeapSort : " << timer1.duration<std::chrono::milliseconds>() << " ms\n\n";
    // === === ===

    // 1
    std::cout << "=========   1   =========" << std::endl;

    Vector<OptionData> q5;
    // === 去重複 ===
    HashTable<std::string, int> ht = HashTable<std::string, int>(n * 2);
    Timer timer2;  // 計時開始
    timer2.start();
    int count = 0;
    for (int i = 0; i < n; ++i) {
        std::string key = arr[i].productCode + '_' + arr[i].strikePrice + '_' + arr[i].expirationDate + '_' + arr[i].callPut;
        // 順便找第五題 TXO_9900_201705_C
        if (key == "TXO_9900_201705_C") {
            q5.emplace_back(arr[i]);
        }
        int index = -1;
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
    std::cout << "=========================" << std::endl
              << std::endl;

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
    std::cout << "=========================" << std::endl
              << std::endl;

    std::cout << "=========== 5 ===========" << std::endl;
    Timer timer4;  // 計時開始
    timer4.start();
    std::cout << "A:" << std::endl;
    for (size_t i = q5.getSize() - 1; i >= q5.getSize() - 10; i--) {
        std::cout << q5[i].tradeDate << " | " << q5[i].tradeTime << " | " << q5[i].tradePrice << std::endl;
    }
    std::cout << "B:" << std::endl;
    for (size_t i = 0; i < 10; ++i) {
        std::cout << q5[i].tradeDate << " | " << q5[i].tradeTime << " | " << q5[i].tradePrice << std::endl;
    }
    std::cout << "C (median) :" << std::endl;
    if (q5.getSize() % 2 == 0) {
        std::cout << q5[q5.getSize() / 2 - 1].tradeDate << " | " << q5[q5.getSize() / 2 - 1].tradeTime << " | " << q5[q5.getSize() / 2 - 1].tradePrice << std::endl;
        std::cout << q5[q5.getSize() / 2].tradeDate << " | " << q5[q5.getSize() / 2].tradeTime << " | " << q5[q5.getSize() / 2].tradePrice << std::endl;
    } else {
        std::cout << q5[q5.getSize() / 2].tradeDate << " | " << q5[q5.getSize() / 2].tradeTime << " | " << q5[q5.getSize() / 2].tradePrice << std::endl;
    }

    std::cout << "D :" << std::endl;
    OptionData* array = q5.toArray();
    HeapSort<OptionData, CompareByTradeDateTimePrice> hsbytime;
    hsbytime.heapSort(array, q5.getSize(), CompareByTradeDateTimePrice(), false);

    Vector<TickData> tickData;
    for (size_t i = 0; i < q5.getSize(); ++i) {
        TickData td;
        td.price = array[i].tradePrice;
        td.timestamp = array[i].tradeDate + " " + array[i].tradeTime;
        tickData.emplace_back(td);
    }


    Vector<double> returns;  // 用於儲存所有報酬率的向量
    // 計算所有 tick 的報酬率（除了第一個 tick）
    for (size_t i = 1; i < tickData.getSize(); ++i) {
        double currentPrice = tickData[i].price;
        double previousPrice = tickData[i - 1].price;
        double tickReturn = calculateTickReturn(currentPrice, previousPrice);
        returns.emplace_back(tickReturn);
    }


    // 找到最大和最小報酬率及其發生的時間點
    double minReturn = returns[0];
    double maxReturn = returns[0];
    std::size_t minReturnIndex = 0;
    std::size_t maxReturnIndex = 0;

    for (std::size_t i = 1; i < returns.getSize(); ++i) {
        if (returns[i] < minReturn) {
            minReturn = returns[i];
            minReturnIndex = i;
        }
        if (returns[i] > maxReturn) {
            maxReturn = returns[i];
            maxReturnIndex = i;
        }
    }
    std::string minReturnTimestamp1 = tickData[minReturnIndex].timestamp;
    std::string minReturnTimestamp2 = tickData[minReturnIndex + 1].timestamp;  // 因為排除了第一個 tick
    std::string maxReturnTimestamp1 = tickData[maxReturnIndex].timestamp;
    std::string maxReturnTimestamp2 = tickData[maxReturnIndex + 1].timestamp;  // 因為排除了第一個 tick

    // 輸出結果
    std::cout << "Maximum return: " << maxReturn << "% at time \n"
              << maxReturnTimestamp1 << " " << tickData[maxReturnIndex].price << "\n"
              << maxReturnTimestamp2 << " " << tickData[maxReturnIndex + 1].price << std::endl;
    std::cout << "Minimum return: " << minReturn << "% at time \n"
              << minReturnTimestamp1 << " " << tickData[minReturnIndex].price << "\n"
              << minReturnTimestamp2 << " " << tickData[minReturnIndex + 1].price << std::endl;

    timer4.stop();  // 計時結束
    std::cout << " >>> 5 : " << timer4.duration<std::chrono::milliseconds>() << " ms\n\n";
    std::cout << "=========================" << std::endl
              << std::endl;

    totalTimer.stop();  // 計時結束
    std::cout << " >>> Total time : " << totalTimer.duration<std::chrono::milliseconds>() << " ms\n\n";
    return 0;
}