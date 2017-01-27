//
// Created by calamer on 1/10/17.
//

#include "Product.h"

#ifndef BASKETOPTION_H
#define BASKETOPTION_H

class BasketOption : public Product {
public:

    BasketOption(double maturity,int nbAssets,int nbTimeSteps,PnlVect *lambda,double strike);
    double payoff(const PnlMat *path, PnlVect *spot) const;
    ~BasketOption();
    string getName() const { return "BasketOption"; }
private:
    PnlVect *lambda;
    double strike;
};

#endif //BASKETOPTION_H
