#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <utility>  

class MatchOrder;

class Order{
    public:
        Order(std::string orderID, std::string type, std::string stockType, float limitPrice, int targetQuantity, int arrivalNum, bool match){
            this->orderID_ = orderID;
            this->type_ = type;
            this->stockType_ = stockType;
            this->limitPrice_ = limitPrice;
            this->targetQuantity_ = targetQuantity;
            this->arrivalNumber_ = arrivalNum;  
            this->matched_ = match;
        }

        Order(const Order &copyOrder){
            orderID_ = copyOrder.orderID_;
            type_ = copyOrder.type_;
            stockType_ =  copyOrder.stockType_;
            limitPrice_ = copyOrder.limitPrice_;
            targetQuantity_ = copyOrder.targetQuantity_;
            arrivalNumber_ = copyOrder.arrivalNumber_;
            matched_ = copyOrder.matched_;

        }

        Order();

        std::string getOrderID() const{
            return this->orderID_;
        }

        std::string getType() const{
            return this->type_;
        }

        std::string getStockType() const{
            return this->stockType_;
        }

        float getLimitPrice() const{
            return this->limitPrice_;
        }

        int getTargetQuantity() const{
            return this->targetQuantity_;
        }

        int getArrivalNumber() const{
            return this->arrivalNumber_;
        }

        bool getMatched() const{
            return this->matched_;

        }

        void setMatch(bool match){
            this->matched_ = match;

        }

        bool operator==(Order &rhs){
            if(rhs.getOrderID() == this->orderID_){
                return true;
            }
            return false;
            
        }



    
    private:
        //float tradedPrice_ = 0;
        std::string orderID_;
        std::string type_;
        std::string stockType_;
        //bool divisible_;
        float limitPrice_;
        int targetQuantity_;
        int arrivalNumber_;
        bool matched_;
};


class Order;
class MatchOrder{
    public:
        MatchOrder(Order &order) : orderToMatch_(order){}
        

        void createMatch(Order buy, Order sell){
            this->matchedOrder_ = std::make_pair(buy,sell);
        }

        std::pair<Order,Order> getMatch(){
            return this->matchedOrder_;

        }


        //Order to be match is a selling order
        //then this is called to match it to buying orders        
        bool matchToBuyingOrders(){
            bool matchFound = false;
            //Order currentMatch;
            //Loop through the buying orders
            for(auto ob : this->buyingOrders_){

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
                        if(ob.getMatched()){
                            //Check the match
                            //Return true if the match has been changed
                            //False if not, continue searching for another
                            //Order
                            if(checkMatch(ob)){
                                matchFound = true;
                                break;
                            }
                        }
                        //If third conditon is false
                        //Create a match
                        else{
                            ob.setMatch(true);
                            this->orderToMatch_.setMatch(true);
                            createMatch(ob, this->orderToMatch_);
                            matchFound = true;
                            break;
                        }
                        
                    }
                }

            }
            return matchFound;  
        }

        bool matchToSellingOrders(){
             bool matchFound = false;

             for(auto os : this->sellingOrders_){
                
                if(this->orderToMatch_.getLimitPrice() >= os.getLimitPrice()){
                    if(os.getTargetQuantity() == this->orderToMatch_.getTargetQuantity()){
                        if(os.getMatched()){
                            if(checkMatch(os)){
                                matchFound = true;
                                break;
                            }
                        }
                        else{
                            os.setMatch(true);
                            this->orderToMatch_.setMatch(true);
                            createMatch(this->orderToMatch_, os);
                            matchFound = true;
                            break;
                        }
                        
                    }
                }

            }
            return matchFound;

        }

        bool checkMatch(Order checkOrder){
            bool change =  false;
            //Checking the type
            if(checkOrder.getType() == "B"){
                
                for(auto o : this->matchedOrdersVector_){
                    //Check if the 2 buying orders are the same
                    if(o.first == checkOrder){
                        //If the orderToMatch has a higher limit price
                        //Hence higher piority
                        //This order is changed
                        if(this->orderToMatch_.getLimitPrice() > o.second.getLimitPrice()){
                            o.second.setMatch(false);
                            o.second = this->orderToMatch_;
                            change = true;
                            break;
                        }
                        else{
                            break;
                        }
                    
                    }
                    

                }
            }

            else{
                 for(auto o : this->matchedOrdersVector_){
                    if(o.second == checkOrder){
                        if(this->orderToMatch_.getLimitPrice() < o.first.getLimitPrice()){
                            o.first.setMatch(false);
                            o.first = this->orderToMatch_;
                            change = true;
                            break;
                        }
                        else{
                            break;
                        }
                    
                    }
                    

                }

            }

            return change;

        }
    
    private:
        Order orderToMatch_;
        std::vector<Order> buyingOrders_;
        std::vector<Order> sellingOrders_;
        std::pair <Order,Order> matchedOrder_;
        std::vector<std::pair<Order,Order>> matchedOrdersVector_;

};

void sortOrderPiority(std::vector<Order>& orders, Order newOrder){
    Order currentHighest = orders[0];
    if(newOrder.getType() == "B"){
        int position = 0;
        for(auto ob : orders){
            if(newOrder.getLimitPrice() > ob.getLimitPrice()){
                orders.insert(orders.begin() + position, newOrder);
                break;
            }
            position++;
            
        }

        if(position == orders.size()){
            orders.push_back(newOrder);
        }
        

    }

    else{
        int position = 0;
        for(auto os : orders){
            if(newOrder.getLimitPrice() < os.getLimitPrice()){
                orders.insert(orders.begin() + position, newOrder);
                break;
            }
            position++;
            
        }
        if(position == orders.size()){
            orders.push_back(newOrder);
        }
    }

}

void checkMatch(){

}

int main(int argc, char *argv[]){
    //Store each line of the file
    std::string line;

    //Store the name of the file that is given
    //in the command line
    std::string filename;
    filename = argv[1];

    //current line number
    int lineNo = 0;

    //The previous traded price
    float tradedPrice = 0;

    std::vector<Order> orders;
    std::vector<Order> buyingOrders;
    std::vector<Order> sellingOrders;

    std::vector<std::pair<Order,Order>> matchedOrders;
    

    std::ifstream myfile (filename);

    if(myfile.is_open()){

        while(getline (myfile, line)){
            int counter = 1;
            
           

            if(lineNo == 0){
                tradedPrice = std::stof(line);
                 
            }

            else{
                
                std::istringstream iss(line);
                std::vector<std::string> v(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{});
                //std::cout << std::stoi(v[5]);
                Order newOrder(v[0],v[1], v[2],std::stof(v[4]),std::stoi(v[5]), lineNo, false);
                
                //orders.push_back(newOrder);  
                if(newOrder.getType() == "B"){
                    if(buyingOrders.empty()){
                        buyingOrders.push_back(newOrder);
                    }
                    else{
                        sortOrderPiority(buyingOrders, newOrder);
                    }
                    MatchOrder newMatch(newOrder);
                    // if(newMatch.matchToSellingOrders()){
                    //     matchedOrders.push_back(newMatch.getMatch());
                    // }
                    

                    

                }
                else{
                    if(sellingOrders.empty()){
                        sellingOrders.push_back(newOrder);
                    }
                    else{
                        sortOrderPiority(sellingOrders, newOrder);
                    }
                    // MatchOrder newMatch(buyingOrders, sellingOrders, newOrder, matchedOrders);
                    // if(newMatch.matchToBuyingOrders()){
                    //     matchedOrders.push_back(newMatch.getMatch());
                    // }
                }
  
            }

            lineNo++;
            
           
            //std::cout << line << '\n';
        }
        myfile.close();
    }

    else{
        std::cout << "Unable to open file";
    }

    // for(auto o : orders){
    //     std::cout << o.getOrderID() <<" "<< o.getType()<<" " << o.getStockType()<<" " << o.getLimitPrice()<<" " << o.getTargetQuantity()<<" " << o.getArrivalNumber() << std::endl;
    // }

    for(auto o : sellingOrders){
        std::cout << o.getOrderID() <<" "<< o.getType()<<" " << o.getStockType()<<" " << o.getLimitPrice()<<" " << o.getTargetQuantity()<<" " << o.getArrivalNumber() << std::endl;
    }

    std::cout << "Buying Orders:" << std::endl;

    for(auto o : buyingOrders){
        std::cout << o.getOrderID() <<" "<< o.getType()<<" " << o.getStockType()<<" " << o.getLimitPrice()<<" " << o.getTargetQuantity()<<" " << o.getArrivalNumber() << std::endl;
    }

    return 0;

}