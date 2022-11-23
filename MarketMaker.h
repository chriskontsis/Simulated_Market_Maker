#ifndef _MARKETMAKER_H_
#define _MARKETMAKER_H_
#include "inputData.h"
#include "time.h"
#include <iostream>
#include <random> 
#include <fstream>

class MarketMaker {
    public:
    MarketMaker(int argc, char** argv);
    void start();
    
    private:
    void orderGenerator(Order& order);
    void orderMatch(Order& order);
    void processTime(const Order& order);
    void parseOrders(std::string orderInfo, const std::string& delimeter, Order& order);
    void help();
    void print();
    void notEnd(size_t pos);
    void isNum(const std::string& symbol);
    void medi(const std::string& symbol, long long newprice);
    sellBooks sellBooks;
    buyBooks buyBooks;
    long long currentStamp;

    bool verbose;
    bool median;
    bool midPoint;
    bool transfers;
    bool VWAP;
    bool randomorder;
    std::string delimeter;
    std::string filename;

    long long commissions;
    long long moneyTransfered;
    long long numTrades;
    long long sharesTraded;
    ClientsInfo clientsInfo;
    EquitysInfo equityInfo;
    std::unordered_map<std::string, long long> medians;
    minHeaps minHeaps;
    maxHeaps maxHeaps;
    std::set<std::string> equities;
};


#endif