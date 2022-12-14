#include "inputData.h"
#include <string> 

Order::Order(long long time_stamp, std::string client_name, std::string ticker_symbol, long long _price, long long _quantity, 
             long long _expiration, Side _side): timeStamp(time_stamp), clientName(client_name), tickerSymbol(ticker_symbol), price(_price), 
             quantity(_quantity), expiration(_expiration), side(_side)
{ID++;}

long long Order::ID = 0;