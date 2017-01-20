#pragma once

#include "Product.h"
#include <iostream>
#include <string.h>
#include <pnl/pnl_matrix.h>
#include <pnl/pnl_vector.h>

class Pricer {
public:
	Product *prod;

	Pricer(Product *prod) {
		this->prod = prod;
	}

	virtual void price(const PnlMat *past, double t, double &price, double &ic, bool parallel) = 0;
	virtual void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, bool parallel) = 0;
	//virtual void gamma(const PnlMat *past, double t, PnlMat *gamma);
	//virtual void vega(const PnlMat *past, double t, PnlVect *vega);

};
