#ifndef DATA_H
#define DATA_H

#include <string>

class Data {
  public:
    double totalGain;
    double grossValue;
    double avgPrice;
    int quantity;
    double commission;
    double div;
    std::string accountType;
    std::string currency;
    std::string action;
    std::string symbol;
    std::string date;
    int numTrades;
  
  public:
    Data();
    void dataDiv(void);
    double dataSell(std::map<std::string, Data> &);
    void dataBuy(std::map<std::string, Data> &);
};

#endif