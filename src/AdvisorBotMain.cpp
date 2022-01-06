#include "AdvisorBotMain.h"
#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include "OrderBookEntry.h"
#include "CSVReader.h"

AdvisorBotMain::AdvisorBotMain()
{

}

void AdvisorBotMain::init()
{
    currentTime = orderBook.getEarliestTime();
    printHelp();

    while(true)
    {
        printTitle();
        processUserInput(getUserInput());
    }
}


void AdvisorBotMain::printTitle()
{   
    std::cout << "Please enter a command, or help for a list of commands" << std::endl;
}

void AdvisorBotMain::handleHelp(std::vector<std::string> input)
{
    if (input.size() == 1) {
         // if no argument are passed
        printHelp();
    } else if (input.size() == 2) {
         // if second argument is passed (help <command>)
        handleHelpSpecific(input[1]);
    } else {
         // invalid number of arguments
        printInvalidCommand();
    }
}

void AdvisorBotMain::handleHelpSpecific(std::string command)
{
    if (command == "prod") {
        printProd();
    } else if (command == "min") {
        printMin();
    } else if (command == "max") {
        printMax();
    } else if (command == "avg") {
        printAvg();
    } else if (command == "predict") {
        printPredict();
    } else if (command == "time") {
        printTime();
    } else if (command == "step") {
        printStep();
    } else {
        // invalid command passed
        printInvalidCommand();
    }
}

void AdvisorBotMain::printHelp()
{
    std::cout << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << std::endl;

    std::cout << "Available commands are:" << std::endl;
    std::cout << std::endl;

    std::cout << "help" << "\t\t" << "Get help on the available commands" << std::endl;
    std::cout << "help <cmd>" << "\t" << "Get help on the specified <cmd> command" << std::endl;
    printProd();
    printMin();
    printMax();
    printAvg();
    printPredict();
    printTime();
    printStep();
    std::cout << std::endl;
}

void AdvisorBotMain::handleProd()
{
    std::cout << "Available products:" << std::endl;
    std::vector<std::string> products = orderBook.getKnownProducts();
    for (std::string& product : products)
    {
        std::cout << product << std::endl;
    }
    std::cout << std::endl;    
}

void AdvisorBotMain::printProd()
{
    std::cout << "prod" << "\t\t" << "List all available products" << std::endl;
}

void AdvisorBotMain::handleMin(std::vector<std::string> input)
{   
    if (input.size() == 3) {
        // handle bookType
        OrderBookType bookType = OrderBookEntry::stringToOrderBookType(input[2]);
        if (bookType == OrderBookType::unknown) {
            std::cout << "book type is invalid. valid types are: ask/ bid" << std::endl;
            return;
        }

        // handle product input
        std::string product = input[1];
        if (!orderBook.isProductInTimestamp(product, currentTime, bookType)) {
            std::cout << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " not found in the current timestamp: " << currentTime << std::endl;
            return;
        }

        // handle min calculation
        std::vector<OrderBookEntry> orders = orderBook.getOrders(bookType, product, currentTime);
        const double price = orderBook.getLowPrice(orders);

        std::cout << "The min " << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " is " << price << std::endl;
    } else {
        printInvalidCommand();
    }
}

void AdvisorBotMain::printMin()
{
    std::cout << "min" << "\t\t" << "Find minimum bid or ask for product in current time step" << std::endl;
}

void AdvisorBotMain::handleMax(std::vector<std::string> input)
{
    if (input.size() == 3) {
        // handle bookType
        OrderBookType bookType = OrderBookEntry::stringToOrderBookType(input[2]);
        if (bookType == OrderBookType::unknown) {
            std::cout << "book type is invalid. valid types are: ask/ bid" << std::endl;
            return;
        }

        // handle product input
        std::string product = input[1];
        if (!orderBook.isProductInTimestamp(product, currentTime, bookType)) {
            std::cout << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " not found in the current timestamp: " << currentTime << std::endl;
            return;
        }

        // handle max calculation
        std::vector<OrderBookEntry> orders = orderBook.getOrders(bookType, product, currentTime);
        const double price = orderBook.getHighPrice(orders);

        std::cout << "The max " << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " is " << price << std::endl;
    } else {
        printInvalidCommand();
    }
}

void AdvisorBotMain::printMax()
{   
    std::cout << "max" << "\t\t" << "Find maximum bid or ask for product in current time step" << std::endl;
}

void AdvisorBotMain::handleAvg(std::vector<std::string> input)
{
    if (input.size() == 4) {
        // handle bookType
        OrderBookType bookType = OrderBookEntry::stringToOrderBookType(input[2]);
        if (bookType == OrderBookType::unknown) {
            std::cout << "book type is invalid. valid types are: ask/ bid" << std::endl;
            return;
        }

        // handle timestamps input
        int timestamps = -1;
        try {
            timestamps = std::stoi(input[3]);
        } catch (const std::exception& e) {
            std::cout << "amount of timestamps is invalid. valid inputs are: numbers (1,2,3..)" << std::endl; 
            return;
        }

        // handle product input
        std::string product = input[1];
        if (!orderBook.isProductInLastTimestamps(product, currentTime, timestamps, bookType)) {
            std::cout << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " not found in the last " << timestamps << " timestamps" << std::endl;
            return;
        }

        // calc avg
        double avg = orderBook.calcProductInTimestampsAvg(product, currentTime, timestamps, bookType);

        std::cout << "The avg " << product << " " << OrderBookEntry::bookTypeToString(bookType) << " over the last " << timestamps << " was " << avg << std::endl;
    } else {
        printInvalidCommand();
    }
}

void AdvisorBotMain::printAvg()
{
    std::cout << "avg" << "\t\t" << "Compute Average ask/bid for product in the last timestamps" << std::endl;
}

void AdvisorBotMain::handlePredict(std::vector<std::string> input)
{
    if (input.size() == 4) {
        int timesteps = 5;

        // handle bookType
        OrderBookType bookType = OrderBookEntry::stringToOrderBookType(input[3]);
        if (bookType == OrderBookType::unknown) {
            std::cout << "book type is invalid. valid types are: ask/ bid" << std::endl;
            return;
        } else {
            if (bookType == OrderBookType::ask) {
                bookType = OrderBookType::bid;
            } else {
                bookType = OrderBookType::ask;
            }
        }

        // handle product input
        std::string product = input[2];
        if (!orderBook.isProductInLastTimestamps(product, currentTime, timesteps, bookType)) { // did the product appear in the last 10 timesteps
            std::cout << OrderBookEntry::bookTypeToString(bookType) << " for " << product << " not found in the last " << timesteps << " timestamps" << std::endl;
            return;
        }

        // read max/min
        std::string requestedOperator = input[1];
        if (requestedOperator != "min" && requestedOperator != "max") {
            std::cout << "operator is invalid. valid types are: min/ max" << std::endl;
            return;
        }

        // calc prediction
        double prediction = orderBook.calcProductPrediction(product, currentTime, timesteps, bookType, requestedOperator);

        std::cout << "The predicted " << OrderBookEntry::bookTypeToString(bookType) << " price for " << product << " is " << prediction << std::endl;
    } else {
        printInvalidCommand();
    }
}

void AdvisorBotMain::printPredict()
{
    std::cout << "predict" << "\t\t" << "Predict max or min ask or bid for the sent product for the next time" << std::endl;
}

void AdvisorBotMain::handleTime()
{
    std::cout << "Current time is: " << currentTime << std::endl;
}

void AdvisorBotMain::printTime()
{
    std::cout << "time" << "\t\t" << "State current time in dataset, i.e. which timeframe are we looking at" << std::endl;
}

void AdvisorBotMain::printStep()
{
    std::cout << "step" << "\t\t" << "Move to next time step" << std::endl;
}

void AdvisorBotMain::printInvalidCommand()
{
    std::cout << "Invalid input. type help, to see available commands" << std::endl;
}
   
void AdvisorBotMain::handleStep()
{
    std::cout << "Going to next time frame. " << std::endl;
    currentTime = orderBook.getNextTime(currentTime);
    handleTime();
}
 
std::vector<std::string> AdvisorBotMain::getUserInput()
{
    std::string rawInput;
    std::getline(std::cin, rawInput);

    try {
        // split input by spaces
        return CSVReader::tokenise(rawInput, ' ');
    } catch (const std::exception& e) {
        printInvalidCommand();
    }
    // return default empty vector array
    std::vector<std::string> emptyArary;
    return emptyArary;
}

void AdvisorBotMain::processUserInput(std::vector<std::string> input)
{
    if (input.size() == 0) {
        printInvalidCommand();
        return;
    }

    std::cout << std::endl;
    if (input[0] == "help") {
        handleHelp(input);
    } else if (input[0] == "prod") {
        handleProd();
    } else if (input[0] == "min") {
        handleMin(input);
    } else if (input[0] == "max") {
        handleMax(input);
    } else if (input[0] == "avg") {
        handleAvg(input);
    } else if (input[0] == "predict") {
        handlePredict(input);
    } else if (input[0] == "time") {
        handleTime();
    } else if (input[0] == "step") {
        handleStep();
    } else {
        printInvalidCommand();
    }
    std::cout << std::endl;
}
