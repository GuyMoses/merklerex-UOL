#pragma once

#include <vector>
#include <map>
#include <functional>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class AdvisorBotMain
{
    public:
        AdvisorBotMain();
        void init();
    private: 
        void printTitle();
        
        // commands

        // help
        void handleHelp(std::vector<std::string> input);
        void handleHelpSpecific(std::string command);
        void printHelp();

        // prod
        void handleProd();
        void printProd();

        // min
        void handleMin(std::vector<std::string> input);
        void printMin();

        // max
        void handleMax(std::vector<std::string> input);
        void printMax();

        // avg
        void handleAvg(std::vector<std::string> input);
        void printAvg();

        // predict
        void printPredict();

        // time
        void handleTime();
        void printTime();

        // step
        void handleStep();
        void printStep();

        // common
        void printInvalidCommand();

        // user input
        std::vector<std::string> getUserInput();
        void processUserInput(std::vector<std::string> input);


        // properties
        std::string currentTime;
        OrderBook orderBook{"20200317.csv"};
};
