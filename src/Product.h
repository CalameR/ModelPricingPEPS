#include <iostream>
#include <string.h>
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>


#ifndef PRODUCT_H
#define PRODUCT_H

using namespace std;

class Product {
public:
	double maturity;
	int nbTimeSteps;
	int nbAssets;
	string name;
	virtual double payoff(const PnlMat* path, PnlVect *spot) const = 0;
	virtual string getName() const = 0;
};

#endif
