#include "Order.h"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <string>

//Define the orders private memebers
Order::Order(){}

Order::Order(std::string orderID, std::string type, std::string stockType, float limitPrice, int targetQuantity, int arrivalNum, bool match){
     this->orderID_ = orderID;
     this->type_ = type;
     this->stockType_ = stockType;
     this->limitPrice_ = limitPrice;
     this->targetQuantity_ = targetQuantity;
     this->arrivalNumber_ = arrivalNum;  
     this->matched_ = match;
}

std::string Order:: getOrderID() const{
     return this->orderID_;
}

std::string Order:: getType() const{
     return this->type_;
}

std::string Order::getStockType() const{
     return this->stockType_;
}

float Order::getLimitPrice() const{
    return this->limitPrice_;
}

int Order::getTargetQuantity() const{
    return this->targetQuantity_;
}

int Order::getArrivalNumber() const{
    return this->arrivalNumber_;
}

bool Order::getMatched() const{
    return this->matched_;
}

void Order::setMatch(bool match){
    this->matched_ = match;
}

bool Order::operator==(Order &rhs){
    if(rhs.getOrderID() == this->orderID_){
        return true;
    }
    return false;      
}

PiorityOrder::PiorityOrder(){}

//Loops over each order, compares the limit price to the new order
//If it is larger then, insert at that position
//position_ is set to the position variable
void PiorityOrder::sortBuyingOrders(std::vector<Order> &orders, Order newOrder){
    
    int position = 0;
    for(auto ob : orders){ 
        if(newOrder.getLimitPrice() > ob.getLimitPrice()){
            orders.insert(orders.begin() + position, newOrder);
            this->position_ = position;
            break;
        }
        position++;
            
    }
    if(position == orders.size()){
        this->position_ = position;
        orders.push_back(newOrder);
        
    }
        

    

};


//Same as sortBuyingOrders but checks if
//Limit price is smaller
void PiorityOrder::sortSellingOrders(std::vector<Order> &orders, Order newOrder){
   
    int position = 0;
    for(auto os : orders){
        if(newOrder.getLimitPrice() < os.getLimitPrice()){
            orders.insert(orders.begin() + position, newOrder);
            this->position_ = position;
            break;
        }
        position++;
            
    }
    if(position == orders.size()){
        this->position_ = position;
        orders.push_back(newOrder);
        
    }
   

};


int PiorityOrder::getPosition(){
    return this->position_;

};



MatchOrder::MatchOrder(){}

MatchOrder::MatchOrder(Order &order, int p){
    this->orderToMatch_ = order;
    this->position_ = p;
}


//Using the c++ function to make a matched buying and selling pair      
void MatchOrder::createMatch(Order buy, Order sell){
    this->matchedOrder_ = std::make_pair(buy,sell);
}

std::pair<Order,Order> MatchOrder::getMatch(){
    return matchedOrder_;

}


//Order to be match is a selling order
//then this is called to match it to buying orders        
bool MatchOrder::matchToBuyingOrders(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders){
   

    //Incase there is more than on match
    Order currentMatch;

    //Counters to delete a matched order
    int currentMatchPostion = 0;
    int counter = 0;
    //Loop through the buying orders
    for(auto ob : buyingOrders){

        //First condition
        //Check if the limit price of the buy order
        //is higher than or equal to the selling order
        if(ob.getLimitPrice() >= this->orderToMatch_.getLimitPrice()){

            //Second condition
            //Check if the target quantity are the same
            //Currently assuming all orders are indivisible
            if(ob.getTargetQuantity() == this->orderToMatch_.getTargetQuantity()){
                        
                //Third condition
                //check if the current buying order has already a
                //match
                if(this->orderToMatch_.getMatched()){
                    //Check the match
                    //Return true if the match has been changed,
                    //change the currentMatch and position
                    //False if not, continue searching for another
                    //Order
                    if(checkMatch(ob, currentMatch)){
                        currentMatch = ob;
                        currentMatchPostion = counter;
                    }
                }

                //If third conditon is false
                //set order to match to true
                //set currentMatch and position
                else{
                    currentMatch = ob;
                    currentMatchPostion = counter;
                    this->orderToMatch_.setMatch(true);
                   
                }
                        
            }
        }
        counter++;

    }

    //If order matched
    //create a match pair
    //delete the matched orders from the vectors
    if(this->orderToMatch_.getMatched()){
        createMatch(currentMatch, this->orderToMatch_);
        buyingOrders.erase(buyingOrders.begin()+currentMatchPostion);
        sellingOrders.erase(sellingOrders.begin()+this->position_);
        return true;

    }

    return false;  

}

//Same as matchToBuyingOrders
//Differnce is matching a pending buying order to pending
//selling orders
bool MatchOrder::matchToSellingOrders(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders){
    bool matchFound = false;
    Order currentMatch;
    int currentMatchPostion = 0;
    int counter = 0;

    for(auto os : sellingOrders){
                
        if(os.getLimitPrice() <= this->orderToMatch_.getLimitPrice()){
            if(os.getTargetQuantity() == this->orderToMatch_.getTargetQuantity()){  
                if(this->orderToMatch_.getMatched()){
                    if(checkMatch(os, currentMatch)){
                        currentMatch = os;
                        currentMatchPostion = counter;
                    }
                }
                else{
                            
                    currentMatch = os;
                    this->orderToMatch_.setMatch(true);
                    currentMatchPostion = counter;
                
                }
                        
            }
        }

        counter++;

    }


     if(this->orderToMatch_.getMatched()){
        createMatch(this->orderToMatch_, currentMatch);
        buyingOrders.erase(buyingOrders.begin()+currentMatchPostion);
        sellingOrders.erase(sellingOrders.begin()+this->position_);
        return true;

    }

    return false;

}

std::vector<Order> MatchOrder::getBuyingOrders(){
    return this->buyingOrders_;
}

std::vector<Order> MatchOrder::getSellingOrders(){
    return this->sellingOrders_;

}

//If there is a current match in a matchTo_ function
//Check if there is a better match
bool MatchOrder::checkMatch(Order &checkOrder, Order &currentMatch){
    if(checkOrder.getType() == "B"){
        if(checkOrder.getLimitPrice() > currentMatch.getLimitPrice()){
            return true;
        }
        if(checkOrder.getLimitPrice() == currentMatch.getLimitPrice()){
            if(checkOrder.getArrivalNumber() < currentMatch.getArrivalNumber()){
                return true;
            }
        } 
    }

    else{
        if(checkOrder.getLimitPrice() < currentMatch.getLimitPrice()){
            return true;
        }
        if(checkOrder.getLimitPrice() == currentMatch.getLimitPrice()){
            if(checkOrder.getArrivalNumber() < currentMatch.getArrivalNumber()){
                return true;
            }
        } 

    }

    return false;
           
}


ExecuteOrder::ExecuteOrder(){}

//Gets the last traded price from the last matched order executed
float ExecuteOrder::getTradedPrice(std::pair<Order,Order> currentMatch){
    float tradedPrice = 0;

    //Compares their arrival time
    if(currentMatch.first.getArrivalNumber() < currentMatch.second.getArrivalNumber()){
        tradedPrice = currentMatch.first.getLimitPrice();
    }
    else{
        tradedPrice =  currentMatch.second.getLimitPrice();
    }

    std::cout<<"Buying Order: "<<currentMatch.first.getOrderID() <<" has been matched with Selling Order: " << currentMatch.second.getOrderID()<<std::endl;

    return tradedPrice;
}

//Writes the matched orders to a file
void ExecuteOrder::executeToFile(std::vector<std::pair<Order,Order>> &matchedOrders, std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders){
    std::ofstream outputFile ("executions.txt");
    if(outputFile.is_open()){
        for(auto mo : matchedOrders){
            float executionPrice = 0.0;

            outputFile << std::setprecision(2) << std::fixed;
            if(mo.first.getArrivalNumber() < mo.second.getArrivalNumber()){
                executionPrice = mo.first.getLimitPrice();
            }
            else{
                executionPrice =  mo.second.getLimitPrice();
            }

            outputFile << "order " << mo.first.getOrderID() <<" "<< mo.first.getTargetQuantity() << " shares purchased at price " << executionPrice << std::endl;
            outputFile << "order " << mo.second.getOrderID() <<" "<<mo.second.getTargetQuantity() << " shares sold at price " << executionPrice << std::endl;
  
        }

        if(!(buyingOrders.empty())){
            for(auto bo : buyingOrders){
                outputFile << "order " << bo.getOrderID() <<" "<<bo.getTargetQuantity() <<" shares unexecuted" << std::endl;

            }
        }

        if(!(sellingOrders.empty())){
            for(auto so : sellingOrders){
                outputFile << "order " << so.getOrderID() <<" "<<so.getTargetQuantity() <<" shares unexecuted" << std::endl;

            }

        }
        
    }

}

//Outputs the pending orders to the screen
void ExecuteOrder::screenOutput(std::vector<Order> &buyingOrders, std::vector<Order> &sellingOrders, float tradingPrice){
    std::cout<<"Updated pending orders: "<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;
    std::cout << std::setprecision(2) << std::fixed;

    std::cout<<"Last Trading price: "<<tradingPrice<<std::endl;
    int buySize = buyingOrders.size();
    int sellingSize = sellingOrders.size();
    int counter = 0;
    int width = 30;
    std::string n = " ";
    std::string dashline(width, '-');

    std::cout<<std::left<< std::setw(width)<<"Buy"<<"Sell" <<std::endl;
    std::cout<<std::left<< std::setw(width)<<dashline<<dashline<<std::endl;
    if(sellingOrders.empty() && !(buyingOrders.empty())){
        for(auto ob : buyingOrders){
            std::cout<<std::left<<std::setw(width)<<ob.getOrderID()+" "+floatToString(ob.getLimitPrice())+" "+std::to_string(ob.getTargetQuantity())<<n<<std::endl;
        }    
    }

    else if(buyingOrders.empty() && !(sellingOrders.empty())){
        for(auto os : sellingOrders){
            std::cout<<std::left<<std::setw(width)<<n<<os.getOrderID()+" "+floatToString(os.getLimitPrice())+" "+std::to_string(os.getTargetQuantity())<<std::endl;
        }    
    }

    else{
        for(auto ob : buyingOrders){
            if(sellingSize == 0){
                std::cout<<std::left<<std::setw(width)<<ob.getOrderID()+" "+floatToString(ob.getLimitPrice())+" "+std::to_string(ob.getTargetQuantity())<<n<<std::endl;
            }
            else{
                std::cout<<std::left<<std::setw(width)<<ob.getOrderID()+" "+floatToString(ob.getLimitPrice())+" "+std::to_string(ob.getTargetQuantity())<<sellingOrders[counter].getOrderID()+" "+floatToString(sellingOrders[counter].getLimitPrice())+" "+std::to_string(sellingOrders[counter].getTargetQuantity())<<std::endl;
            }

            counter++;
            buySize--;
            sellingSize--;
    
        }

        if(sellingSize != 0){
            for(sellingSize; sellingSize > 0; --sellingSize){
                std::cout<<std::left<<std::setw(width)<<n<<sellingOrders[counter].getOrderID()+" "+floatToString(sellingOrders[counter].getLimitPrice())+" "+std::to_string(sellingOrders[counter].getTargetQuantity())<<std::endl;
                counter++;
            }
        }

    }

    std::cout<<std::left<< std::setw(width)<<dashline<<dashline<<std::endl;
    std::cout<<std::endl;
    std::cout<<std::endl;

}

//Converts the float to a string
std::string ExecuteOrder::floatToString(float num){
    std::stringstream sstream;
    sstream << std::fixed << std::setprecision(2) << num;
    std::string numStr = sstream.str();
    return numStr;
 }