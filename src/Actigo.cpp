//
// Created by calamer on 12/1/16.
//

#include "Actigo.h"

Actigo::Actigo(double initialInvestment, RatesMarkets *ratesMarkets) {
	this->initialInvestment = initialInvestment;
	this->maturity = 8;
	this->nbAssets = 5;
	this->nbTimeSteps = 16;
	this->customerPerformance = 0.6;
	this->ratesMarkets = ratesMarkets;
    this->type = ACTIGO;
}

// !! path->m == nbTimeSteps + 1 !!
// !!  path->n == nbAssets
/*
double Actigo::payoff(const PnlMat *path, PnlVect *spot) const {

	double sumPerf = 0.0;
	double esx50_0 = MGET(path, 0, 0);
	double ssp500_0 = MGET(path, 0, 1);
	double ssp200_0 = MGET(path, 0, 2);
	double xed_0 = MGET(path, 0, 3);
	double xea_0 = MGET(path, 0, 4);
	for (int t = 1; t < nbTimeSteps + 1; t++) {
		sumPerf += MAX((MGET(path, t, 0) / esx50_0) + (MGET(path, t, 1)*xed_0) / (ssp500_0*MGET(path, t, 3)) +
							   (MGET(path, t, 2)*xea_0) / (ssp200_0*MGET(path, t, 4)) - 3.0, 0.0);
	}

	return initialInvestment*(1.0 + customerPerformance*(sumPerf/(3*nbTimeSteps)));
}
*/

double Actigo::payoff(const PnlMat *path, PnlVect *spot) const {

	double sumPerf = 0.0;
	double esx50_0 = MGET(path, 0, 0);
	double ssp500_0 = MGET(path, 0, 1);
	double ssp200_0 = MGET(path, 0, 2);
	double rxed_0 = MGET(path, 0, 3);
	double rxea_0 = MGET(path, 0, 4);
    double t;
	for (int i = 1; i < nbTimeSteps + 1; i++) {
        t = this->maturity/(double)i;
		sumPerf += MAX((MGET(path, i, 0) / esx50_0) + (MGET(path, i, 1)*rxed_0*exp(ratesMarkets->getSumForwardRates(USD,0,t))) / (ssp500_0*MGET(path, i, 3)) +
					   (MGET(path, i, 2)*rxea_0*exp(ratesMarkets->getSumForwardRates(AUD,0,t))) / (ssp200_0*MGET(path, i, 4)) - 3.0, 0.0);
	}
	return initialInvestment*(1.0 + customerPerformance*(sumPerf/(3*nbTimeSteps)));
}

/*
double Actigo::payoff(const PnlMat *path) const {

    double sumPerf = 0.0;
    double esx50_0 = MGET(path, 0, 0);
    double ssp500_0 = MGET(path, 0, 1);
    double ssp200_0 = MGET(path, 0, 2);
    double xed_0 = MGET(path, 0, 3);
    double xea_0 = MGET(path, 0, 4);
    double value_t;
    double xed_t, xea_t;
    for (int t = 1; t < nbTimeSteps + 1; t++) {
        xed_t = MGET(path, t, 3);
        xea_t = MGET(path, t, 4);
        value_t = MGET(path, t, 0)*ssp500_0*xed_t*ssp200_0*xea_t +
                  MGET(path, t, 1)*xed_0*esx50_0*ssp200_0*xea_t +
                  MGET(path, t, 2)*xea_0*esx50_0*ssp500_0*xed_t;
        value_t /= esx50_0*ssp500_0*xed_t*ssp200_0*xea_t;
        sumPerf += MAX(value_t - 3.0, 0.0);
    }
    return initialInvestment*(1.0 + customerPerformance*(sumPerf/(3*nbTimeSteps)));
}
 */

Actigo::~Actigo()
{
}

