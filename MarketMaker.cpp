#include "MarketMaker.h"

#include<string> 
MarketMaker::MarketMaker(int argc, char**argv): filename("input.txt"), delimeter(" ") {
    
}

void MarketMaker::start() {
    Order order;
    std::string orderInfo;

    if(!randomorder) {
        std::ifstream myfile(filename);

        if(myfile.is_open()) {
            getline(myfile, orderInfo);

            while(getline(myfile, orderInfo)) {
                orderFlow(orderInfo, delimeter, order);
                processTime(order);
                orderMatch(order);
            }
            myfile.close();
        }
    }

    print();
}

void MarketMaker::orderMatch(Order& order) {
    auto symbol = order.tickerSymbol;
    auto expiration = order.expiration;

    if(order.side == 0) {

    }
    else {
        
    }
}

void MarketMaker::processTime(const Order& order) {

}

void MarketMaker::orderFlow(std::string orderInfo, const std::string& delimeter, Order& order) {
    
}

void MarketMaker::print() {

}