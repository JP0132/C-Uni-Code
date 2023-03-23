#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <utility>  
#include "Order.h"
#include "Order.cpp"
using namespace std;

//Group of 1: Assuming all orders are limit orders

void checkList( std::vector<Order> buyingOrders){ 
    for(auto o : buyingOrders){ 
        std::cout << o.getOrderID() <<" "<< o.getType()<<" " << o.getStockType()<<" " << o.getLimitPrice()<<" " << o.getTargetQuantity()<<" " << o.getArrivalNumber() << std::endl; 
    } 
    
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

    //vectors to separte the types of orders;
    std::vector<Order> buyingOrders;
    std::vector<Order> sellingOrders;

    //Vector storing each matched pair
    std::vector<std::pair<Order,Order>> matchedOrders;
    
    //Opens the file and reads each line
    std::ifstream myfile (filename);
    if(myfile.is_open()){

        while(getline (myfile, line)){
            int counter = 1; 

            //First line stores the traded price
            if(lineNo == 0){
                tradedPrice = std::stof(line); 
            }

            else{
                //Convert the line to string stream
                std::istringstream iss(line);
                //Store the string stream elements into a vector
                std::vector<std::string> v(std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{});
                //Create a new order
                Order newOrder(v[0],v[1], v[2],std::stof(v[4]),std::stoi(v[5]), lineNo, false);
                //Declare executeorder class
                ExecuteOrder scrOut;

                //If new order is a buying order 
                if(newOrder.getType() == "B"){
                    int p = 0;
                    //Place into vector if empty
                    if(buyingOrders.empty()){
                        buyingOrders.push_back(newOrder);
                        //Output to screen
                        scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                    }
                    //If vector not empty
                    else{
                        
                        //Declare piority order
                        PiorityOrder po;
                        //Sort the new order into the vector
                        po.sortBuyingOrders(buyingOrders, newOrder);
                        //Store its position
                        p = po.getPosition();

                        //Output to screen
                        scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                    }

                    //Match the buying order to pending selling orders
                    if(!(sellingOrders.empty())){
                        MatchOrder newMatch(newOrder, p);
                        //Returns true if matched
                        if(newMatch.matchToSellingOrders(buyingOrders, sellingOrders)){
                            matchedOrders.push_back(newMatch.getMatch());
                            //Output the new list of pending orders
                            tradedPrice =  scrOut.getTradedPrice(newMatch.getMatch());
                            scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                         
                        }

                    }
                    
                    

                    

                }
                //Same as above but for selling orders
                else{
                    int p = 0;
                    if(sellingOrders.empty()){
                        sellingOrders.push_back(newOrder);
                        scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                    }
                    else{
                        PiorityOrder po;
                        po.sortSellingOrders(sellingOrders, newOrder);
                        p = po.getPosition();
                        scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                    }
                    if(!(buyingOrders.empty())){
                          MatchOrder newMatch(newOrder, p);
                        if(newMatch.matchToBuyingOrders(buyingOrders, sellingOrders)){
                            matchedOrders.push_back(newMatch.getMatch());
                            tradedPrice =  scrOut.getTradedPrice(newMatch.getMatch());
                            scrOut.screenOutput(buyingOrders, sellingOrders, tradedPrice);
                        }

                    }
                  
                }
  
            }

            lineNo++;
            
        }
        myfile.close();
    }

    else{
        std::cout << "Unable to open file";
    }

    //Writes the matched orders to the text file
    ExecuteOrder exeOrder;
    exeOrder.executeToFile(matchedOrders, buyingOrders, sellingOrders);

    return 0;

}