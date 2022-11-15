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
};


#endif