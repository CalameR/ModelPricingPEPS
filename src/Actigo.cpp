//
// Created by calamer on 12/1/16.
//

#include "pnl/pnl_mathtools.h"
#include "Actigo.h"

Actigo::Actigo(double initialInvestment) {
	this->initialInvestment = initialInvestment;
	this->maturity = 8;
	this->nbAssets = 5;
	this->nbTimeSteps = 16;
	this->customerPerformance = 0.6;
}

// !! path->m == nbTimeSteps + 1 !!
// !!  path->n == nbAssets
double Actigo::payoff(const PnlMat *path) const {

	double sumPerf = 0.0;
	double esx50_0 = MGET(path, 0, 0);
	double ssp500_0 = MGET(path, 0, 1);
	double ssp200_0 = MGET(path, 0, 2);
	double xed_0 = MGET(path, 0, 3);
	double xea_0 = MGET(path, 0, 4);
	for (int i = 1; i < nbTimeSteps + 1; i++) {
		sumPerf += MAX((MGET(path, i, 0) / esx50_0) + (MGET(path, i, 1)*xed_0) / (ssp500_0*MGET(path, i, 3)) +
							   (MGET(path, i, 2)*xea_0) / (ssp200_0*MGET(path, i, 4)) - 3.0, 0.0);
	}

	return initialInvestment*(1.0 + customerPerformance*(sumPerf/(3*nbTimeSteps)));
}

Actigo::~Actigo()
{
}

