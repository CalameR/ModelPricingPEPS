//
// Created by calamer on 12/1/1.
//
#include "pnl/pnl_mathtools.h"
#include "Product.h"

class Actigo : public Product {
public:
	Actigo(double initialInvestment, double rDol, double rAUD);
	double payoff(const PnlMat *path, PnlVect *spot) const;
    string getName() const { return "Actigo"; }

private:
	double initialInvestment;
	double customerPerformance;
	double rDOL;
	double rAUD;
	double getValueCashUS(double t) const { return exp(rDOL*t); }
	double getValueCashAU(double t) const { return exp(rAUD*t); }
    ~Actigo();
};
