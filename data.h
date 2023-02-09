#ifndef DATA_H
#define DATA_H

#include <string>

typedef struct {
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
} Data;

void dataDiv(Data &);
double dataSell(Data &, std::map<std::string, Data> &);
void dataBuy(Data &, std::map<std::string, Data> &);

#endif