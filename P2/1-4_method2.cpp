#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

void removeSpaces(string &s)
{
    size_t pos = 0;
    while ((pos = s.find(' ', pos)) != string::npos)
    {
        s.erase(pos, 1);
    }
}
class Data
{
public:
    Data()
    {
        c1 = "";
        c2 = "";
        c3 = "";
        c4 = "";
        c5 = "";
        c6 = "";
        c7 = "";
        c8 = "";
        c9 = "";
    };
    string c1, c2, c3, c4, c5, c6, c7, c8, c9;
    int count = 0;
};
int main()
{
    int total_rows = 0;
    int row = 1500000;
    Data *ptr = new Data[row];
    string filename[] = {"OptionsDaily_2017_05_15.csv", "OptionsDaily_2017_05_16.csv", "OptionsDaily_2017_05_17.csv", "OptionsDaily_2017_05_18.csv", "OptionsDaily_2017_05_19.csv"};
    fstream file;
    double time = 0;
    int i = 0;
    for (int k = 0; k < 5; k++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        file.open("data/"+filename[k], ios::in);
        if (file.is_open())
        {
            cout << "Open File"<< k+1 << endl;
        }
        string line;
        getline(file, line, '\n');
        getline(file, line, '\n');
        string c1_temp = "", c2_temp = "", c3_temp = "", c4_temp = "", c5_temp = "", c6_temp = "", c7_temp = "", c8_temp = "", c9_temp = "";
        bool same = false;
        while (getline(file, line))
        {
            same = false;
            if (i == row || line == "")
            {
                break;
            }
            // cout << i << endl;
            // cout << line << endl;
            stringstream ss(line);
            string s;
            getline(ss, s, ',');
            removeSpaces(s);
            c1_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c2_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c3_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c4_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c5_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c6_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c7_temp = s;
            getline(ss, s, ',');
            removeSpaces(s);
            c8_temp = s;
            total_rows++;
            ss.str("");
            ss.clear();
            for (int k = 0; k < i; k++)
            {
                if (ptr[k].c2 == c2_temp && ptr[k].c3 == c3_temp && ptr[k].c4 == c4_temp && ptr[k].c5 == c5_temp)
                {
                    same = true;
                    break;
                }
            }
            if (same)
            {
                continue;
            }
            ptr[i].c1 = c1_temp;
            ptr[i].c2 = c2_temp;
            ptr[i].c3 = c3_temp;
            ptr[i].c4 = c4_temp;
            ptr[i].c5 = c5_temp;
            ptr[i].c6 = c6_temp;
            ptr[i].c7 = c7_temp;
            ptr[i].c8 = c8_temp;
            ptr->count++;
            i++;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cout<< "Load Data + De-Dulicate : " << duration.count()<<" ms" << endl;
        time += duration.count();
        file.close();
    }
    // for(int j=0;j < row;j++){
    //     if(ptr[j].c2 == ""){
    //         break;
    //     }
    //     cout << ptr[j].c2 << ptr[j].c3 << ptr[j].c4 << ptr[j].c5 << endl;
    // }
    cout << "Total Data : " << total_rows << endl;
    cout << "De-Dulicate Data : " << ptr->count << endl;
    string need_found[][4]={
        {"TXO","1000","201706","P"},
        {"TXO","9500","201706","C"},
        {"GIO","5500","201706","C"}
    };
    int result[] = {0,0,0};
    bool found[] = {false, false, false};
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < row; j++)
    {
        if (ptr[j].c2 == "")
        {
            break;
        }
        if (need_found[0][0] == ptr[j].c2 && need_found[0][1] == ptr[j].c3 && need_found[0][2] == ptr[j].c4 && need_found[0][3] == ptr[j].c5)
        {
            found[0] = true;
            result[0] = j;
        }
        if(need_found[1][0] == ptr[j].c2 && need_found[1][1] == ptr[j].c3 && need_found[1][2] == ptr[j].c4 && need_found[1][3] == ptr[j].c5){
            found[1] = true;
            result[1] = j;
        }
        if(need_found[2][0] == ptr[j].c2 && need_found[2][1] == ptr[j].c3 && need_found[2][2] == ptr[j].c4 && need_found[2][3] == ptr[j].c5){
            found[2] = true;
            result[2] = j;
        }
    }
    if(found[0]){
        cout << "TXO_1000_201706_P found => "<< ptr[result[0]].c1 << ","<< ptr[result[0]].c2 << ","<< ptr[result[0]].c3 << ","<< ptr[result[0]].c4 << ","<< ptr[result[0]].c5 << ","<< ptr[result[0]].c6 << ","<< ptr[result[0]].c7 << ","<< ptr[result[0]].c8 << endl;
    }else{
        cout << "TXO_1000_201706_P not found" << endl;
    }
    if(found[1]){
        cout << "TXO_9500_201706_C found => "<< ptr[result[1]].c1 << ","<< ptr[result[1]].c2 << ","<< ptr[result[1]].c3 << ","<< ptr[result[1]].c4 << ","<< ptr[result[1]].c5 << ","<< ptr[result[1]].c6 << ","<< ptr[result[1]].c7 << ","<< ptr[result[1]].c8 << endl;
    }else{
        cout << "TXO_9500_201706_C not found" << endl;
    }
    if(found[2]){
        cout << "GIO_5500_201706_C found" << ptr[result[2]].c1 << ","<< ptr[result[2]].c2 << ","<< ptr[result[2]].c3 << ","<< ptr[result[2]].c4 << ","<< ptr[result[2]].c5 << ","<< ptr[result[2]].c6 << ","<< ptr[result[2]].c7 << ","<< ptr[result[2]].c8 << endl;
    }else{
        cout << "GIO_5500_201706_C not found" << endl;
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2);
    cout<< "Search : " << duration2.count()<<" ms" << endl;
    time += duration2.count();
    cout << "Total Time : " << time << " ms" << endl;
}