#include "MarketMaker.h"
#include<string> 

MarketMaker::MarketMaker(int argc, char**argv): filename("test.txt"), delimeter(" "), randomorder(false), median(true) {
    currentStamp=0;
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
        while(!sellBooks[symbol].empty()) {
            auto bestSell = sellBooks[symbol].top();
            if(bestSell.expiration != -1 && !(currentStamp - bestSell.timeStamp < bestSell.expiration)) {
                //order expired remove it
                sellBooks[symbol].pop(); break;
            }
            if(order.price > bestSell.price || order.price == bestSell.price) {
                if(order.quantity < bestSell.quantity) {
                    // create a new order after the buyer has made the transaction and push it back into the sell book
                    auto nextOrder = sellBooks[symbol].top();
                    nextOrder.quantity = bestSell.quantity - order.quantity;
                    sellBooks[symbol].pop();
                    sellBooks[symbol].push(nextOrder);
                    if(verbose) std::cout << order.clientName << " purchased " << order.quantity <<  " share of " << symbol << " from " << bestSell.clientName << " for $ " << bestSell.price << "/share" << std::endl;
                    // gather end market stats
                    commissions += ((bestSell.price*order.quantity) / 100) * 2;
                    moneyTransfered += bestSell.price*order.quantity;
                    ++numTrades;
                    sharesTraded += order.quantity;
                    //clients stats
                    clientsInfo[order.clientName].quantityBought += order.quantity;
                    clientsInfo[order.clientName].netTransfer -= bestSell.price * order.quantity;
                    clientsInfo[bestSell.clientName].quantitySold += order.quantity;
                    clientsInfo[bestSell.clientName].netTransfer += bestSell.price * order.quantity;
                    //equity info
                    equityInfo[symbol].priceVolume.push_back({bestSell.price, order.quantity});

                    // median
                    calcMedian(symbol, bestSell.price);
                    return;
                }
                else if(order.quantity == bestSell.quantity) {
                    sellBooks[symbol].pop();
                    if(verbose) std::cout << order.clientName << " purchased " << order.quantity <<  " share of " << symbol << " from " << bestSell.clientName << " for $ " << bestSell.price << "/share" << std::endl;
                    commissions += ((bestSell.price*order.quantity / 100) * 2);
                    moneyTransfered += bestSell.price * order.quantity;
                    ++numTrades;
                    sharesTraded += order.quantity;
                    
                    // update info on clients
                    clientsInfo[order.clientName].quantityBought += order.quantity;
                    clientsInfo[order.clientName].netTransfer -= bestSell.price * order.quantity;
                    clientsInfo[bestSell.clientName].quantityBought += order.quantity;
                    clientsInfo[bestSell.clientName].netTransfer += bestSell.price * order.quantity;

                    //update equity price volume
                    equityInfo[symbol].priceVolume.push_back({bestSell.price, order.quantity});
                    calcMedian(symbol, bestSell.price);
                    return;
                }
                else //parital match
                {
                    order.quantity = order.quantity - bestSell.quantity;
                    sellBooks[symbol].pop();
                    if(verbose) std::cout << order.clientName << " purchased " << order.quantity <<  " share of " << symbol << " from " << bestSell.clientName << " for $ " << bestSell.price << "/share" << std::endl;
                    commissions += ((bestSell.price * bestSell.quantity) / 100) * 2;
                    moneyTransfered += bestSell.price*bestSell.quantity;
                    ++numTrades;
                    sharesTraded += bestSell.quantity;

                    //updating client info
                    clientsInfo[order.clientName].quantityBought += bestSell.quantity;
                    clientsInfo[order.clientName].netTransfer -= bestSell.price*bestSell.quantity;
                    clientsInfo[bestSell.clientName].quantityBought += order.quantity;
                    clientsInfo[bestSell.clientName].netTransfer += bestSell.price*bestSell.quantity;

                    equityInfo[symbol].priceVolume.push_back({bestSell.price, bestSell.quantity});
                    static bool flag = true;
                    if(flag) calcMedian(symbol, bestSell.price); flag = false;
                }
            }
            else { 
                // no match for price
                if(expiration != 0) buyBooks[symbol].push(order); return;
            }
        }
        // empty book no match
        if(expiration != 0) buyBooks[symbol].push(order); return;
    }

    else {
        while(!buyBooks[symbol].empty()) {
            auto bestBuy = buyBooks[symbol].top();
            if(bestBuy.expiration != -1 && !(currentStamp - bestBuy.timeStamp < bestBuy.expiration)) {
                // best buy has expired
                buyBooks[symbol].pop(); break;
            }
            if(order.price < bestBuy.price || order.price == bestBuy.price) {
                if(order.quantity < bestBuy.quantity) {
                    auto topOrder = buyBooks[symbol].top();
                    buyBooks[symbol].pop();
                    topOrder.quantity = bestBuy.quantity - order.quantity;
                    buyBooks[symbol].push(topOrder);
                    if(verbose) std::cout << bestBuy.clientName << " purchased " << order.quantity <<  " share of " << symbol << " from " << order.clientName << " for $ " << bestBuy.price << "/share" << std::endl;
                    
                    commissions += ((bestBuy.price * order.quantity) / 100) *2;
                    moneyTransfered += bestBuy.price*order.quantity;
                    ++numTrades;
                    sharesTraded+=order.quantity;

                    //update client info
                    clientsInfo[bestBuy.clientName].quantityBought += order.quantity;
                    clientsInfo[bestBuy.clientName].netTransfer -= bestBuy.price - order.quantity;
                    
                }
            }
        }
    }
}

void MarketMaker::processTime(const Order& order) {
    if(order.timeStamp != currentStamp) {
        if(median) {
            for(auto medianPair : medians) {
                std::cout << "Median match price of " << medianPair.first << " at time " << currentStamp << " is " << medianPair.second << std::endl;
            }
        }
        if(midPoint) {
            for(auto equity : equities) {
                if(sellBooks[equity].size() == 0 || buyBooks[equity].size() == 0) {
                    std::cout << "Midpoint of " << equity << " at time " << currentStamp << " is not defined " << std::endl;
                }
                else {
                     std::cout << "Midpoint of " << equity << " at time " << currentStamp << " is $ " << ((sellBooks[equity].top().price + buyBooks[equity].top().price) >> 1) << std::endl;
                }
            }
        }
        currentStamp = order.timeStamp;
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

void MarketMaker::calcMedian(const std::string &symbol, long long price) {

}

void MarketMaker::print() {

}