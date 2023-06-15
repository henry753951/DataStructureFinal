// P2_1 ~ P2_4 先讀完全部檔案，再使用兩個for loop抓重複


#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

int n = 0;

struct OptionData {
    OptionData(){ };
    OptionData(string p, double st, string exp, string ca)
    {
        productCode = p;
        strikePrice = st;
        expirationDate = exp;
        callPut = ca;
    }
    void show()
    {
        std::cout << tradeDate << " / " << productCode << " / " << strikePrice << " / " << expirationDate << " / " << 
        callPut << " / " << tradeTime << " / " << tradePrice << " / " << tradeQuantity << " / " << openingPrice << endl;
    }
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

int processCSV(const std::string& filename, OptionData*& arr, int startIndex = 0) {
    int nn = 0;
    int n=0;
    std::ifstream file("data/" + filename);
    if (!file.is_open()) {
        std::cout << "無法開啟檔案：" << filename << std::endl;
        return 1;
    }

    std::cout << "Reading " << filename << "...\n";

    std::string line;
    std::getline(file, line);  // 略過標題列
    std::getline(file, line);  // 略過---
    while (std::getline(file, line)) {
        nn++;
        if (file.eof()) break;
        // cerr << "in while " << nn << endl;
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

        
        arr[startIndex + n] = option;
        n++;
    }
    // cerr << "out of while" << endl;
    // std::cout << "Read " << nn << " lines\n";

    file.close();
    return nn;
}

int main()
{
    OptionData* arr = new OptionData[4000000];
    // processCSV(fileName, arr);

    std::string filenames[5] = {        
        "OptionsDaily_2017_05_15.csv",
        "OptionsDaily_2017_05_16.csv",
        "OptionsDaily_2017_05_17.csv",
        "OptionsDaily_2017_05_18.csv",
        "OptionsDaily_2017_05_19.csv"
    };

    int total = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<5; i++)
    {
        total += processCSV(filenames[i], arr, total);
        // cerr << "total: " << total << endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

//-----------------P2-1(use For loop)----------------------------------------------------------------
    OptionData *unique = new OptionData[2000000];
    int count = 0;
    start = std::chrono::high_resolution_clock::now();
    for (int i=0; i<total; i++)
    {
        bool fail = 0;
        OptionData OD_1 = arr[i];
        // cout << OD_1.productCode << " " << OD_1.strikePrice << endl;
        // if (OD_1.strikePrice == 0) break;    // 跳過無資料部分

        for (int j=0; j<count; j++)
        {
            OptionData OD_2 = unique[j];
            if ((OD_1.productCode == OD_2.productCode) && (OD_1.strikePrice == OD_2.strikePrice) 
            && (OD_1.expirationDate == OD_2.expirationDate) && (OD_1.callPut == OD_2.callPut))
            {
                fail = 1;
                break;
            }
        }
        if (!fail)
        {
            unique[count++] = OD_1;
        }
    }
    std::cout << "Unique products: " << count << endl;
    end = std::chrono::high_resolution_clock::now();
    auto duration_2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
// ---------------Problem2-4---------------------------------------------------------------------------------- 
    int i=0;
    OptionData sample1("TXO", 1000, "201706", "P"), sample2("TXO", 9500, "201706", "C"), sample3("GIO", 5500, "201706", "C");
    bool check1, check2, check3;
    int index_1, index_2, index_3;
    for (int i=0; i<count; i++)
    {
        OptionData obj = unique[i];
        if (obj.strikePrice == 0) break;

        if (obj.productCode == sample1.productCode && obj.strikePrice == sample1.strikePrice && 
        obj.expirationDate == sample1.expirationDate && obj.callPut == sample1.callPut)
        {
            index_1 = i;
            check1 = true;
        }
        
        if (obj.productCode == sample2.productCode && obj.strikePrice == sample2.strikePrice && 
        obj.expirationDate == sample2.expirationDate && obj.callPut == sample2.callPut)
        {
            index_2 = i;
            check2 = true;
        }

        if (obj.productCode == sample3.productCode && obj.strikePrice == sample3.strikePrice && 
        obj.expirationDate == sample3.expirationDate && obj.callPut == sample3.callPut)
        {
            index_3 = i;
            check3 = true;
        }
    }
    cout << endl;
    cout << "TXO_1000_201706_P: ";
    if (check1) 
    {
        cout << "exist. -> ";
        unique[index_1].show();
    }
    else cout << "not found." << endl;
    cout << "TXO_9500_201706_C: ";
    if (check2) 
    {
        cout << "exist. -> ";
        unique[index_2].show();
    }
    else cout << "not found." << endl;
    cout << "GIO_5500_201706_C: ";
    if (check3) 
    {
        cout << "exist. -> ";
        unique[index_3].show();
    }
    else cout << "not found." << endl;

    cout << endl;
    cout << "Read file: " << duration_1.count() << "ms" << endl;
    cout << "Find unique: " << duration_2.count() << "ms" << endl;
    cout << "Total time: " << duration_1.count()+duration_2.count() << "ms" << endl;


// ------------------------------------------------------------------------------------------------------------
}
