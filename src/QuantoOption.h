//
// Created by courtocy on 1/25/17.
//

#ifndef MC_PRICER_QUANTOOPTION_H
#define MC_PRICER_QUANTOOPTION_H
#include "Product.h"
#include "RatesMarkets.h"

class QuantoOption : public Product {
public:
    QuantoOption(double maturity,int size,int nbTimeSteps,PnlVect *lambda,double strike, currencyType foreignCurrency, RatesMarkets *ratesMarkets) :
            strike(strike),
            lambda(lambda)
    {
        this->maturity = maturity;
        this->nbAssets = size;
        this->nbTimeSteps = nbTimeSteps;
        this->ratesMarkets = ratesMarkets;
        this->type = QUANTO;
    };
    double payoff(const PnlMat* path, PnlVect *spot) const;
private:
    double strike;
    PnlVect *lambda;
    currencyType foreignCurrency;
    RatesMarkets *ratesMarkets;
};


#endif //MC_PRICER_QUANTOOPTION_H
