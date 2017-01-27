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
	double rEUR = 0.004;
	double rDOL = 0.007;
	double rAUD = 0.015;

	Product *actigo = new Actigo(i0,rDOL,rAUD);

	/*char *actigoData = "actigoData.dat";
	PnlMat *actigoData_from_file = pnl_mat_create_from_file(actigoData);

	double payoff = actigo->payoff(actigoData_from_file);
	std::cout << "payoff = " << payoff << std::endl;
	*/

	PnlVect *sigma = pnl_vect_create(actigo->nbAssets);
	LET(sigma, 0) = 0.16;
	LET(sigma, 1) = 0.12 + 0.01;
	LET(sigma, 2) = 0.16 + 0.01;
	LET(sigma, 3) = 0.01;
	LET(sigma, 4) = 0.01;

	PnlVect *spot = pnl_vect_create(actigo->nbAssets);
	LET(spot, 0) = 3300;
	LET(spot, 1) = 2200 * 0.95;
	LET(spot, 2) = 5700 * 0.70;
	LET(spot, 3) = 0.95;
	LET(spot, 4) = 0.70;

	PnlMat *rho = pnl_mat_create_from_scalar(actigo->nbAssets, actigo->nbAssets, 0);
	MLET(rho,0,1) = 0.35;
	MLET(rho,1,0) = 0.35;
	MLET(rho,0,2) = 0.71;
	MLET(rho,2,0) = 0.71;
	MLET(rho,0,3) = 0.66;
	MLET(rho,3,0) = 0.66;

	MLET(rho,1,2) = 0.24;
	MLET(rho,2,1) = 0.24;
	MLET(rho,1,3) = 0.53;
	MLET(rho,3,1) = 0.53;

	MLET(rho,2,3) = 0.52;
	MLET(rho,3,2) = 0.52;

	MLET(rho,3,4) = 0.50;
	MLET(rho,4,3) = 0.50;
	for (int i = 0; i < actigo->nbAssets; i++) {
		pnl_mat_set(rho, i, i, 1.0);
	}

	PnlVect *trends = pnl_vect_create_from_scalar(actigo->nbAssets, 0);
	LET(trends,0) = 0.15;
	LET(trends,1) = 0.20;
	LET(trends,2) = 0.18;
	LET(trends,3) = rDOL + 0.01;
	LET(trends,4) = rAUD + 0.01;
	PnlVect *dividends = pnl_vect_create_from_scalar(actigo->nbAssets, 0);
	//LET(dividends, 3) = rDOL;
	//LET(dividends, 3) = 0;
	//LET(dividends, 4) = rAUD;
	//LET(dividends, 4) = 0;

	BlackScholesModel *bSM = new BlackScholesModel(actigo->nbAssets, rEUR, trends, dividends, sigma, spot, rho);

	double fdStep = 1e-5;
	int nbSamples = 150000;

	PnlMat *past = pnl_mat_create(1, actigo->nbAssets);
	MLET(past, 0, 0) = 3300;
	MLET(past, 0, 1) = 2200 * 0.95;
	MLET(past, 0, 2) = 5700 * 0.70;
	MLET(past, 0, 3) = 0.95;
	MLET(past, 0, 4) = 0.70;

	MonteCarloPricer *MC = new MonteCarloPricer(bSM, actigo, fdStep, nbSamples);

    double prix;
	double ic;

    time_t before;
    time_t after;
    double computingTime;


    time(&before);
	MC->price(past, 0,prix,ic,true);

	std::cout << "Price = " << prix << " €\n";
    std::cout << "Capitalization Price = " << prix*exp(bSM->getSumForwardRates(0,actigo->maturity))  << " €\n";

	PnlVect *delta = pnl_vect_create(actigo->nbAssets);
	PnlVect *deltaIC = pnl_vect_create(actigo->nbAssets);

	MC->delta(NULL, 0, delta,deltaIC,true);
    time(&after);
    computingTime = difftime(after,before);

	for (int i = 0; i < actigo->nbAssets; i++) {
		std::cout << "delta[" << i << "] = " << GET(delta, i) << "\n";
	}
    //std::cout << "Computing Time = " << computingTime << " seconds";

	//std::cout << GET(delta,3)/exp(rDOL) << "\n";
	//std::cout << GET(delta,4)/exp(rAUD*8) << "\n";


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
	//SimulationHedger::prices(MC,actigo->nbTimeSteps*6*4,"ProductPrices.txt","time.txt",isParallel);
	SimulationHedger::hedging_PL_Prices(MC,actigo->nbTimeSteps*6*22,"ProductPrices.txt","PortfolioPrices.txt","time.txt",isParallel);
    //double PL = SimulationHedger::hedging_PL(MC,actigo->nbTimeSteps*2,isParallel);
    time(&after);
    computingTime = difftime(after,before);

    //std::cout << "PL = " << PL << " €\n";
	//std::cout << "Temps de calcul = " << computingTime << " secondes";

}