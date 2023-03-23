#ifndef ORDER_H_
#define ORDER_H_

#include <iostream>
#include <memory>
#include <string>

class Order {
    public:
        Order();

    private:
        std::string orderID;
        std::string orderType;
        std::string orderLimit;
        bool orderDivisble;

};

class MatchOrder {

};

#endif