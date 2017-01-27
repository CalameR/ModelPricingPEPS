//
// Created by courtocy on 1/25/17.
//

#ifndef MC_PRICER_QUANTOOPTION_H
#define MC_PRICER_QUANTOOPTION_H
#include "Product.h"

class QuantoOption : public Product {
public:
    double strike;
    PnlVect *lambda;
    QuantoOption(double maturity,int size,int nbTimeSteps,PnlVect *lambda,double strike) :
            strike(strike),
            lambda(lambda)
    {
        this->maturity = maturity;
        this->nbAssets = size;
        this->nbTimeSteps = nbTimeSteps;
        this->name = "Quanto";
    };
    double payoff(const PnlMat* path, PnlVect *spot) const;
    string getName() const {return this->name;};
};


#endif //MC_PRICER_QUANTOOPTION_H
