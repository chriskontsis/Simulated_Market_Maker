#include "MarketMaker.h"
#include <iostream>
int main(int argc, char** argv) {
    MarketMaker marketMaker(argc, argv);
    std::cout << "starting" << '\n';
    marketMaker.start();
}