#include <iostream>
#include <string.h>
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>
#include "DefinitionTypes.h"


#ifndef PRODUCT_H
#define PRODUCT_H

using namespace std;

class Product {
public:
	double maturity;
	int nbTimeSteps;
	int nbAssets;
	virtual double payoff(const PnlMat* path, PnlVect *spot) const = 0;
	virtual productType getProductType() const { return type; }
protected:
	productType type;
};

#endif
