//
// Created by calamer on 12/1/1.
//

#include "Product.h"

class Actigo : public Product {
public:
	Actigo(double initialInvestment);
	double payoff(const PnlMat *path, PnlVect *spot) const;
    string getName() const { return "Actigo"; }

private:
	double initialInvestment;
	double customerPerformance;
    ~Actigo();
};
