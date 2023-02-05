#include <string>
#include <stdio.h>
#include <sstream>
#include <queue>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>
#include "data.h"

double roundTo2(double value) {
  return std::ceil(value * 100) / 100;
}

void yearInReview(const int year, const int numTrades, const double commissionUSD, const double divUSD, const double gainWOutCommissionUSD,
  const double commissionCAD, const double divCAD, const double gainWOutCommissionCAD) {
  printf("\n###\n\n%d in review\n\n", year);
  printf("Number of trades: %d\n", numTrades);
  printf("Total commission USD: $%.2lf\n", roundTo2(commissionUSD * -1));
  printf("Dividends USD: $%.2lf\n", roundTo2(divUSD));
  printf("Net gain USD: $%.2lf\n", roundTo2(gainWOutCommissionUSD + commissionUSD + divUSD));
  printf("Total commission CAD: $%.2lf\n", roundTo2(commissionCAD * -1));
  printf("Dividends CAD: $%.2lf\n", roundTo2(divCAD));
  printf("Net gain CAD: $%.2lf\n", roundTo2(gainWOutCommissionCAD + commissionCAD + divCAD));

  printf("\n###\n\n\n");
}

std::string parseDate(std::string &str) {
  // format: year-month-day 12:00:00 AM
  return str.substr(0, 10);
}

bool parseLine(std::stringstream &ss, Data &data) {
  int column = 0;
  std::vector<std::string> split(12);
  for (size_t i = 0; ss.good(); ++i) {
    std::getline(ss, split[i], ',');
    ++column;
  }
  if (split[1].compare("Sell") != 0 && split[1].compare("Buy") != 0 && split[1].compare("DIV") != 0) {
    return false;
  }

  if (split[1].compare("DIV") == 0) {
    data.div = std::stod(split[7]);
  } else {
    data.grossValue = std::stod(split[5]);
  }
  data.avgPrice = std::stod(split[4]);
  data.quantity = std::stoi(split[3]);
  data.commission = std::stod(split[6]);
  data.accountType = split[9];
  data.currency = split[8];
  data.action = split[1];
  data.symbol = split[2];
  data.date = parseDate(split[0]);
  return true;
}

int main() {
  std::map<std::string, Data> map;
  std::ifstream infile("./trades.csv");
  std::string line;
  if (!std::getline(infile, line)) {
    return 1;
  }
  std::cout << "\"Gross\" does not include commission\n\n";
  int year = 0;
  double commissionUSD = 0, divUSD = 0, gainWOutCommissionUSD = 0;
  double commissionCAD = 0, divCAD = 0, gainWOutCommissionCAD = 0;
  for (int i = 0; std::getline(infile, line, '\n');) {
    std::stringstream ss(line);
    Data data;
    if (!parseLine(ss, data)) {
      continue;
    }
    if (data.action.compare("Sell") == 0) {
      if (data.currency.compare("USD") == 0) {
        gainWOutCommissionUSD += data.dataSell(map);
        commissionUSD += data.commission;
      } else {
        gainWOutCommissionCAD += data.dataSell(map);
        commissionCAD += data.commission;
      }
      ++i;
    } else if (data.action.compare("Buy") == 0) {
      data.dataBuy(map);
      if (data.currency.compare("USD") == 0) {
        commissionUSD += data.commission;
      } else {
        commissionCAD += data.commission;
      }
      ++i;
    } else if (data.action.compare("DIV") == 0) {
      data.dataDiv();
      if (data.currency.compare("USD") == 0) {
        divUSD += data.div;
      } else {
        divCAD += data.div;
      }
    } else {
      continue;
    }
    if (year == 0) {
      year = std::stoi(data.date.substr(0,4));
    } else if (int nextYear = std::stoi(data.date.substr(0,4)); year < nextYear) {
      // reset stats;
      for (auto &mapEntry : map) {
        mapEntry.second.totalGain = 0;
      }
      yearInReview(year, i,
        commissionUSD, divUSD, gainWOutCommissionUSD,
        commissionCAD, divCAD, gainWOutCommissionCAD
      );
      commissionUSD = 0; divUSD = 0; gainWOutCommissionUSD = 0;
      commissionCAD = 0; divCAD = 0; gainWOutCommissionCAD = 0;
      year = nextYear;
      i = 0;
    }
  }
  return 0;
}