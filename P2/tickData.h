#include <string>

struct TickData {
    double price;
    std::string timestamp;
};

double calculateTickReturn(double currentPrice, double previousPrice) {
    return (currentPrice - previousPrice) / previousPrice * 100.0;
}
