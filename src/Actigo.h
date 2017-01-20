//
// Created by calamer on 12/1/1.
//

#include "Product.h"

class Actigo : public Product {
public:
	Actigo(double initialInvestment);
	double payoff(const PnlMat *path) const;

private:
	double initialInvestment;
	double customerPerformance;
    ~Actigo();
};
