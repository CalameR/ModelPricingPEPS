//
// Created by calamer on 1/10/17.
//

#include <stdexcept>
#include "pnl/pnl_mathtools.h"
#include "BasketOption.h"

BasketOption::BasketOption(double maturity, int nbAssets, int nbTimeSteps, PnlVect *lambda, double strike) {
    this->maturity = maturity;
    this->nbTimeSteps = nbTimeSteps;
    this->nbAssets = nbAssets;
    this->lambda = lambda;
    this->strike = strike;
    this->type = BASKET;
}

double BasketOption::payoff(const PnlMat *path, PnlVect *spot) const {
    double basketPrice = 0;
    for(int d = 0; d < nbAssets; d++) {
        basketPrice += MGET(path,nbTimeSteps,d) * GET(lambda,d);
    }
    return MAX(basketPrice - strike, 0);
}

BasketOption::~BasketOption()
{
}