#ifndef INPUTDATA_H
#define INPUTDATA_H

#include <queue>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <string>
#include <functional>

enum class Side{SELL, BUY};
struct clientsInfo {
    long long quantityBought;
    long long quantitySold;
    long long netTransfer;
};

struct equityInfo {
    std::vector<std::pair<long long, long long> > priceVolume;
};

class Order {
    public:
    Order(){};
    Order(long long time_stamp, std::string client_name, std::string ticker_symbol, long long _price,
            long long _quantity, long long _expiration, Side _side);

    private:
    long long timeStamp;
    std::string clientName;
    std::string tickerSymbol;
    long long price;
    long long quantity;
    long long expiration;
    static long long ID;
    Side side;
    friend struct BuyComparator;
    friend struct SellComparator;
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


using maxHeap = std::priority_queue<long long, std::vector<long long>, std::greater<long long> >;
using minHeap = std::priority_queue<long long, std::vector<long long>, std::less<long long> > ;
using maxHeaps = std::unordered_map<std::string, maxHeap>;
using minHeaps = std::unordered_map<std::string, minHeap>;
using ClientsInfo = std::unordered_map<std::string, clientsInfo>;
using EquitysInfo = std::unordered_map<std::string, equityInfo>;
using sellBook =  std::priority_queue<Order, std::vector<Order>, SellComparator>;
using buyBook = std::priority_queue<Order, std::vector<Order>, BuyComparator>;
using sellBooks = std::unordered_map<std::string, sellBook>;
using buyBooks = std::unordered_map<std::string, buyBook>; 
#endif