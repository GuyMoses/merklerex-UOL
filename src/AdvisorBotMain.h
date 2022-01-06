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
        /** init function */
        void init();
    private: 
        /** Print title*/
        void printTitle();
        
        // commands

        // help
        /** handle help command*/
        void handleHelp(std::vector<std::string> input);
        /** handle help command with arguments*/
        void handleHelpSpecific(std::string command);
        /** print help */
        void printHelp();

        // prod
        /** lists all known products */
        void handleProd();
        /** print products command help*/
        void printProd();

        // min
        /** print minimum price for product of specific type (bid/ask) */
        void handleMin(std::vector<std::string> input);
        /** print min command help*/
        void printMin();

        // max
        /** print maximum price for product of specific type (bid/ask) */
        void handleMax(std::vector<std::string> input);
        /** print max command help*/
        void printMax();

        // avg
        /** print avg price for product of specific type (bid/ask) in requested amount of timestemps*/
        void handleAvg(std::vector<std::string> input);
        /** print avg command help*/
        void printAvg();

        // predict
        /** prints a prediction for product & type price according to last 5 timesteps*/
        void handlePredict(std::vector<std::string> input);
        /** print prediction command help */
        void printPredict();

        // time
        /** print current time */
        void handleTime();
        /** print time command help */
        void printTime();

        // step
        /** goes to next time step */
        void handleStep();
        /** print step command help */
        void printStep();

        // stats
        /** print stats command help */
        void printStats();
        /** print stats regarding the AdvisorBot */
        void handleStats();

        // common
        /** print invalid command text */
        void printInvalidCommand();

        // user input
        /** gets input from user, splits them by spaces using tokenizer */
        std::vector<std::string> getUserInput();
        /** process user input, calls the right command */
        void processUserInput(std::vector<std::string> input);


        // properties
        // stores current time
        std::string currentTime;
        // stores order book
        OrderBook orderBook{"20200317.csv"};
        // stores commands counter
        std::map<std::string, int> commandsCounter;
};
