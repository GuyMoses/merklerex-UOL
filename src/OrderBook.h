#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** construct, reading a csv data file */
        OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();
    /** return vector of all know products in the dataset that match the timestamp*/
        std::vector<std::string> getKnownProductsInTimestamp(std::string timestamp, OrderBookType type);
    /** return wether a product exists in timestamp or not*/
        bool isProductInTimestamp(std::string product, std::string timestamp, OrderBookType type);
    /** return wether a product exists in timestamps or not*/
        bool isProductInLastTimestamps(std::string product, std::string currentTime, int lastTimestamps, OrderBookType type);
    /** calcs avg for product in last timestamps */
    double calcProductInTimestampsAvg(std::string product, std::string currentTime, int lastTimestamps, OrderBookType type);
    /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();
        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);

        void insertOrder(OrderBookEntry& order);

        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

    private:
        std::vector<OrderBookEntry> orders;


};