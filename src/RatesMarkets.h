//
// Created by calamer on 1/29/17.
//

#include "DefinitionTypes.h"

#ifndef RATESMARKETS_H
#define RATESMARKETS_H

class RatesMarkets {
public:
    virtual double getSumForwardRates(double t, double T) = 0;
    virtual double getSumForwardRates(currencyType currency, double t, double T) = 0;
    virtual ratesMarketsType getRatesMarketsType() { return type; }
protected:
    ratesMarketsType type;
};


#endif //RATESMARKETS_H
