#ifndef ORDER_H_
#define ORDER_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <algorithm> 

// ********************** Order class **********************
class Order{
    public:
        //Default constructor
        Order();
        
        //Constructor to initialise a order with the
        //required data
        Order(std::string orderID, std::string type, std::string stockType, float limitPrice, int targetQuantity, int arrivalNum, bool match);

        //Returns the orderID
        std::string getOrderID() const;

        //Returns the type,
        //Buying or Selling
        std::string getType() const;

        //Returns the stock type
        std::string getStockType() const;

        //Returns the limit price
        float getLimitPrice() const;

        //Returns the target quantity
        int getTargetQuantity() const;

        //Returns the arrival number
        int getArrivalNumber() const;
        
        //Returns if the order is matched
        bool getMatched() const;

        //Set if the order has been matched
        void setMatch(bool match);

        //Compare the orders
        //Return true if equal
        //otherwise false
        bool operator==(Order &rhs);



    
    private:
        
        std::string orderID_;
        std::string type_;
        std::string stockType_;
        float limitPrice_;
        int targetQuantity_;
        int arrivalNumber_;
        bool matched_;
};

// ********************** PiorityOrder class **********************

class PiorityOrder{
    public:
        //constructor
        PiorityOrder();

        //Method to sort a new buying order
        //into a vector of buying orders
        void sortBuyingOrders(std::vector<Order> &orders, Order newOrder);

        //Method to sort a new selling order
        //into a vector of selling orders
        void sortSellingOrders(std::vector<Order> &orders, Order newOrder);

        //Get the position of where the new order is
        int getPosition();
    private:

        int position_ = 0;




};

// ********************** MatchOrder class **********************
class MatchOrder {
    public:
        //Default constructor
        MatchOrder();

        //Constructor to create a match order with order
        //to be matched and its positon in the vector
        MatchOrder(Order &order, int p);
        
        //Creates a pair of the buying 
        //and selling order
        void createMatch(Order buy, Order sell);

        //Returns a pair of the matched order
        std::pair<Order,Order> getMatch();

        //Order to be match is a selling order
        //then this is called to match it to buying orders        
        bool matchToBuyingOrders(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders);

        //Order to be match is a selling order
        //then this is called to match it to buying orders  
        bool matchToSellingOrders(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders);

        //Another match is found
        //compare the 2 matches to check which one
        //is better
        bool checkMatch(Order &checkOrder, Order &currentMatch);

        std::vector<Order> getBuyingOrders();

        std::vector<Order> getSellingOrders();
    
    private:
        Order orderToMatch_;
        std::vector<Order> buyingOrders_;
        std::vector<Order> sellingOrders_;
        std::pair <Order,Order> matchedOrder_;
        std::vector<std::pair<Order,Order>> matchedOrdersVector_;
        int position_;

};

// ********************** ExecuteOrder class **********************
class ExecuteOrder{
    public:
        //Constructor
        ExecuteOrder();

        //Work out the execution price
        float getTradedPrice(std::pair<Order,Order> currentMatch);

        //Executes the orders
        //Saves the orders executed to a txt file called
        //execution.txt
        void executeToFile(std::vector<std::pair<Order,Order>> &matchedOrders, std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders);
        
        //Outputs the pending orders to the screen
        void screenOutput(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders, float tradingPrice);

        //Converts a float to a string
        std::string floatToString(float price);

};

#endif