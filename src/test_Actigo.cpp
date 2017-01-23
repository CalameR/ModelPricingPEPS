//
// Created by calamer on 12/1/16.
//

#include <iostream>
#include <ctime>
#include <omp.h>
#include "Actigo.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "pool_pattern.h"
#include "SimulationHedger.h"

using namespace std;

int main(int argc, char **argv) {
	double i0 = 100;

	Product *actigo = new Actigo(i0);

	/*char *actigoData = "actigoData.dat";
	PnlMat *actigoData_from_file = pnl_mat_create_from_file(actigoData);

	double payoff = actigo->payoff(actigoData_from_file);
	std::cout << "payoff = " << payoff << std::endl;
	*/

	double rEUR = 0.004;
	double rDOL = 0.007;
	double rAUD = 0.02;
	PnlVect *sigma = pnl_vect_create(actigo->nbAssets);
	LET(sigma, 0) = 0.16;
	LET(sigma, 1) = 0.12;
	LET(sigma, 2) = 0.16;
	LET(sigma, 3) = 0.15;
	LET(sigma, 4) = 0.15;

	PnlVect *spot = pnl_vect_create(actigo->nbAssets);
	LET(spot, 0) = 3300;
	LET(spot, 1) = 2200 * 0.95;
	LET(spot, 2) = 5700 * 0.70;
	LET(spot, 3) = 0.95;
	LET(spot, 4) = 0.70;

	PnlMat *rho = pnl_mat_create_from_scalar(actigo->nbAssets, actigo->nbAssets, 0);
	for (int i = 0; i < actigo->nbAssets; i++) {
		pnl_mat_set(rho, i, i, 1.0);
	}

	PnlVect *trends = pnl_vect_create_from_scalar(actigo->nbAssets, 0);
	PnlVect *dividends = pnl_vect_create_from_scalar(actigo->nbAssets, 0);
	LET(dividends, 3) += rDOL;
	LET(dividends, 4) += rAUD;

    PnlVect_Pool gaussianPool;
    gaussianPool.init(actigo->nbAssets);

    PnlVect_Pool assetsPool;
    assetsPool.init(actigo->nbAssets);

	BlackScholesModel *bSM = new BlackScholesModel(actigo->nbAssets, rEUR, trends, dividends, sigma, spot, rho, &gaussianPool,&assetsPool);

	ulong seed = 0;
	int count;
    int maxId = omp_get_max_threads()+1;
	PnlRng **rngTab = pnl_rng_dcmt_create_array_id(0, maxId, seed, &count);

	for (int i = 0; i < maxId; i++) {
		pnl_rng_sseed(rngTab[i], time(NULL));
	}

	double fdStep = 0.00001;
	int nbSamples = 50000;

	PnlMat *past = pnl_mat_create(1, actigo->nbAssets);
	MLET(past, 0, 0) = 3300;
	MLET(past, 0, 1) = 2200 * 0.95;
	MLET(past, 0, 2) = 5700 * 0.70;
	MLET(past, 0, 3) = 0.95;
	MLET(past, 0, 4) = 0.70;

    PnlRng_Pool poolRng;
    poolRng.init(0);

	MonteCarloPricer *MC = new MonteCarloPricer(bSM, actigo, fdStep, nbSamples, &poolRng);

    double prix;
	double ic;

    time_t before;
    time_t after;
    double computingTime;

/*
    time(&before);
	MC->price(past, 0,prix,ic,true);

	std::cout << "Price = " << prix << " €\n";
    std::cout << "Capitalization Price = " << prix*exp(bSM->getSumForwardRates(0,actigo->maturity))  << " €\n";

	PnlVect *delta = pnl_vect_create(actigo->nbAssets);
	PnlVect *deltaIC = pnl_vect_create(actigo->nbAssets);

	MC->delta(NULL, 0, delta,deltaIC,false);
    time(&after);
    computingTime = difftime(after,before);

	for (int i = 0; i < actigo->nbAssets; i++) {
		std::cout << "delta[" << i << "] = " << GET(delta, i) << "\n";
	}
    std::cout << "Computing Time = " << computingTime << " seconds";
*/

	// A changer si nécessaire !!!
	bool isParallel = true;
	std::cout << "Produit Actigo :"  << "\n\n";
	std::cout << "Investissement initial : "  << i0 << " €\n\n";
	std::cout <<  "Nombre de dates de constatation = " << actigo->nbTimeSteps <<"\n";

	std::cout << "Taux d'intérêts : "  << "\n";
	std::cout << "rEUR = " << rEUR << "\n";
	std::cout << "rUSD = " << rDOL << "\n";
	std::cout << "rAUD = " << rAUD << "\n\n";

	std::cout << "Volatilités des sous-jacents : "  << "\n";
	std::cout << "vol SX5E = " << GET(sigma, 0) << "\n";
	std::cout << "vol S&P500 = " << GET(sigma, 1)  << "\n";
	std::cout << "vol S&P200 = " << GET(sigma, 2) << "\n";
	std::cout << "vol EUR/USD = " << GET(sigma, 3) << "\n";
	std::cout << "vol EUR/AUD = " << GET(sigma, 4)  << "\n\n";


	std::cout << "Simulation de la couverture : " << " \n" ;
    time(&before);
    SimulationHedger::hedging(MC,actigo->nbTimeSteps*3,"ProductPrices.txt","PortfolioPrices.txt","time.txt",isParallel);
    time(&after);
    computingTime = difftime(after,before);

	std::cout << "Temps de calcul = " << computingTime << " secondes";
	delete MC;
	delete bSM;

}