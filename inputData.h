#ifndef INPUTDATA_H
#define INPUTDATA_H
#include <queue>
#include <string>

struct clientsInfo {

};

struct equityInfo {

};

typedef enum {BUY, SELL} SIDE;
class Order {
    public:
    Order();
    Order(long long time_stamp, std::string client_name, std::string ticker_symbol, long long _price,
            long long _quantity, long long _expiration);


    private:
    long long timeStamp;
    std::string clientName;
    std::string tickerSymbol;
    long long price;
    long long quantity;
    long long expiration;
    long long ID;
    SIDE side;
    friend class BuyOrder;
    friend class SellOrder;
    friend class MarketMaker;
};

#endif