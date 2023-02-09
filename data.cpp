#include <map>
#include <string>
#include <stdio.h>
#include "data.h"

double dataAveragePrice(Data &data) {
  return data.grossValue / (data.quantity == 0 ? 1 : data.quantity);
}

void dataDiv(Data &data) {
  printf("%s Dividend %s currency $%.2lf\n", data.date.c_str(), data.currency.c_str(), data.div);
}

double dataSell(Data &data, std::map<std::string, Data> &map) {
  auto search = map.find(data.symbol + data.accountType + data.currency);
  if (search == map.end()) {
    return 0;
  }
  Data &inMapData = search->second;
  const double avg = dataAveragePrice(inMapData) * -1;
  const double gain = (data.avgPrice - avg) * data.quantity * -1;
  inMapData.quantity += data.quantity;
  inMapData.grossValue = inMapData.quantity * avg * -1;
  inMapData.totalGain += gain;
  printf("%s Sold   %-5d of %-7s @ %s $%-10.4lf | Gross: %s $%.2lf %s\n", data.date.c_str(), data.quantity * -1, data.symbol.c_str(), data.currency.c_str(), data.avgPrice, data.currency.c_str(), gain, data.accountType.c_str());
  return gain;
}

void dataBuy(Data &data, std::map<std::string, Data> &map) {
  if (auto search = map.find(data.symbol + data.accountType + data.currency); search != map.end()) {
    Data &inMapData = search->second;
    inMapData.quantity += data.quantity;
    inMapData.grossValue += data.grossValue;
  } else {
    map.insert({data.symbol + data.accountType + data.currency, data});
  }
  printf("%s Bought %-5d of %-7s @ %s $%-10.4lf %s\n", data.date.c_str(), data.quantity, data.symbol.c_str(), data.currency.c_str(), data.avgPrice, data.accountType.c_str());
}