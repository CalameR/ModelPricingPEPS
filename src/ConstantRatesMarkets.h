//
// Created by calamer on 1/29/17.
//

#include "RatesMarkets.h"
#include "pnl/pnl_vector.h"

#ifndef CONSTANTRATESMARKETS_H
#define CONSTANTRATESMARKETS_H

class ConstantRatesMarkets: public RatesMarkets {
public:
    ConstantRatesMarkets(double domesticRate, PnlVect *rates)
    {
        this->domesticRate = domesticRate;
        this->rates = pnl_vect_copy(rates);
        this->type = CONSTANT_RATES;
    }
    double getSumForwardRates(double t, double T) { return domesticRate*(T-t); }
    double getSumForwardRates(currencyType currency, double t, double T) { return GET(rates,currency)*(T-t); }

    double getDomesticRate() { return domesticRate; }
    double getRate(currencyType currency) { return GET(rates,currency); }
private:
    double domesticRate;
    PnlVect *rates;
};

#endif //CONSTANTRATESMARKETS_H
