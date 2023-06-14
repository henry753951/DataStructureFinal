#include <iostream>
#include <chrono>


class Timer {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    void stop() {
        end_time = std::chrono::high_resolution_clock::now();
    }

    template<typename T>
    long long duration() {
        auto duration = std::chrono::duration_cast<T>(end_time - start_time);
        return duration.count();
    }
};