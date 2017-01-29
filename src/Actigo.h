//
// Created by calamer on 12/1/1.
//
#include "pnl/pnl_mathtools.h"
#include "Product.h"
#include "RatesMarkets.h"

class Actigo : public Product {
public:
	Actigo(double initialInvestment, RatesMarkets *ratesMarkets);
	double payoff(const PnlMat *path, PnlVect *spot) const;
private:
	double initialInvestment;
	double customerPerformance;
	RatesMarkets *ratesMarkets;
    ~Actigo();
};
