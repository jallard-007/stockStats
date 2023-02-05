#include <string>
#include <stdio.h>
#include <map>
#include <iostream>
#include <queue>
#include "data.h"

Data::Data():
  totalGain{}, grossValue{}, avgPrice{}, quantity{}, commission{}, div{},
  accountType{}, currency{}, action{}, symbol{}, date{}, numTrades{}
{}

double dataAveragePrice(Data &data) {
  return data.grossValue / (data.quantity == 0 ? 1 : data.quantity);
}

void Data::dataDiv() {
  printf("%s Dividend %s currency $%.2lf\n", date.c_str(), currency.c_str(), div);
  //std::cout << date << " Dividend " << currency << " $" << div << '\n';
}

double Data::dataSell(std::map<std::string, Data> &map) {
  auto search = map.find(symbol + accountType + currency);
  if (search == map.end()) {
    return 0;
  }
  Data &inMapData = search->second;
  const double avg = dataAveragePrice(inMapData) * -1;
  const double gain = (avgPrice - avg) * quantity * -1;
  inMapData.quantity += quantity;
  inMapData.grossValue = inMapData.quantity * avg * -1;
  inMapData.totalGain += gain;
  printf("%s Sold   %-5d of %-7s @ %s $%-10.4lf | Gross: %s $%.2lf\n", date.c_str(), quantity * -1, symbol.c_str(), currency.c_str(), avgPrice, currency.c_str(), gain);
  //std::cout << date << " Sold " << quantity * -1 << " of " << symbol << " @ " << currency << " $" << avgPrice << " | Gross: " << currency << " $" << gain << '\n';
  return gain;
}

void Data::dataBuy(std::map<std::string, Data> &map) {
  if (auto search = map.find(symbol + accountType + currency); search != map.end()) {
    Data &inMapData = search->second;
    inMapData.quantity += quantity;
    inMapData.grossValue += grossValue;
  } else {
    map.insert({symbol + accountType + currency, *this});
  }
  printf("%s Bought %-5d of %-7s @ %s $%-10.4lf\n", date.c_str(), quantity, symbol.c_str(), currency.c_str(), avgPrice);
  //std::cout << date << " Bought " << quantity << " of " << symbol << " @ " << currency << " $" << avgPrice << '\n';
}