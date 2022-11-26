#include "MarketMaker.h"
#include<string> 
#include <sstream>

MarketMaker::MarketMaker(int argc, char**argv): filename("test.txt"), delimeter(" "), randomorder(false), median(true) {
    currentStamp = 0;
    commissions = 0;
    moneyTransfered = 0;
    numTrades = 0;
    sharesTraded = 0;
}

void MarketMaker::start() {
    Order order;
    std::string orderInfo;

    if(!randomorder) {
        std::ifstream myfile(filename);
        if(myfile.is_open()) {
            getline(myfile, orderInfo);

            while(getline(myfile, orderInfo)) {
                parseOrders(orderInfo, delimeter, order);
                processTime(order);
                orderMatch(order);
            }
            myfile.close();
        }
    }

    //print();
}

void MarketMaker::orderMatch(Order& order) {
    auto symbol = order.tickerSymbol;
    auto expiration = order.expiration;
    
    if(order.side == Side::BUY) {

    }
    else {
        
    }
}

void MarketMaker::processTime(const Order& order) {
    if(order.timeStamp != currentStamp) {
        if(median) {
            
        }
    }
}  

void MarketMaker::parseOrders(std::string orderInfo, const std::string& delimeter, Order& order) {
    long long timestamp, price, quantity, expiration;
    Side side;
    std::string clientName, tickerSymbol;

    //parse timestamp
    auto leftPos = orderInfo.find_first_not_of(delimeter);
    auto rightPos = orderInfo.find_first_of(delimeter, leftPos);
    auto info = orderInfo.substr(leftPos, rightPos - leftPos);
    timestamp = stoll(info);

    //client name
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos, rightPos - leftPos);
    clientName = info;

    //determining side
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos, rightPos - leftPos);
    if(info == "BUY") side = Side::BUY;
    else if(info == "SELL") side = Side::SELL;
    else {std::cout << "Side not suppoted" << '\n'; exit(-1);}

    //parse ticker info
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos, rightPos - leftPos);
    tickerSymbol = info;

    //price 
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos+1, rightPos - leftPos);
    price = stoi(info);

    //quantity
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos+1, rightPos - leftPos);
    quantity = stoi(info);

    //expiration time
    leftPos = orderInfo.find_first_not_of(delimeter, rightPos);
    rightPos = orderInfo.find_first_of(delimeter, leftPos);
    info = orderInfo.substr(leftPos, rightPos - leftPos);
    expiration = stoi(info);

    equities.insert(tickerSymbol);
    Order tempOrder(timestamp, clientName, tickerSymbol, price, quantity, expiration);
    order = tempOrder;
}

void MarketMaker::print() {

}