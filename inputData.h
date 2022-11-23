#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <functional>

struct clientsInfo {
    long long boughtPrice;
    long long soldPrice;
    long long netTransfer;
};

struct equityInfo {
    std::vector<std::pair<long long, long long> > priceVolume;
};

class Order {
    public:
    Order(){};
    Order(long long time_stamp, std::string client_name, std::string ticker_symbol, long long _price,
            long long _quantity, long long _expiration);

    private:
    long long timeStamp;
    std::string clientName;
    std::string tickerSymbol;
    long long price;
    long long quantity;
    long long expiration;
    static long long ID;
    int side;
    friend class BuyComparator;
    friend class SellComparator;
    friend class MarketMaker;
};

struct SellComparator {
    bool operator () (const Order& left, const Order& right) {
        if(right.price < left.price) return true;
        if(right.price == left.price && left.timeStamp > right.timeStamp) return true;
        return false;
    }
};

struct BuyComparator {
    bool operator() (const Order& left, const Order& right) {
        if(left.price < right.price) return true;
        if(left.price == right.price && left.timeStamp > right.timeStamp) return true;
        return false;
    }
};


typedef std::priority_queue<long long, std::vector<long long>, std::greater<long long> > maxHeap;
typedef std::priority_queue<long long, std::vector<long long>, std::less<long long> > minHeap;
typedef std::unordered_map<std::string, maxHeap> maxHeaps;
typedef std::unordered_map<std::string, minHeap> minHeaps;
typedef std::unordered_map<std::string, clientsInfo> ClientsInfo;
typedef std::unordered_map<std::string, equityInfo> EquitysInfo;
typedef std::priority_queue<Order, std::vector<Order>, SellComparator> sellBook;
typedef std::priority_queue<Order, std::vector<Order>, BuyComparator> buyBook;
typedef std::unordered_map<std::string, sellBook> sellBooks;
typedef std::unordered_map<std::string, buyBook> buyBooks; 
#endif