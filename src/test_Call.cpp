//
// Created by calamer on 1/10/17.
//

#include <iostream>
#include <string>
#include "BasketOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include <ctime>
#include <cassert>
#include <omp.h>
#include <fstream>
#include "SimulationHedger.h"

using namespace std;

int main(int argc, char **argv) {

    double T = 1;
    int size = 1;
    int nbTimeSteps = 12;
    double strike = 15;
    PnlVect *lambda = pnl_vect_create_from_scalar(size, 1);

    Product *O = new BasketOption(T, size, nbTimeSteps, lambda, strike);

    double r = 0.05;
    PnlMat *rho = pnl_mat_create_from_scalar(1,1,1);
    PnlVect *sigma = pnl_vect_create_from_scalar(size, 0.3);
    PnlVect *spot = pnl_vect_create_from_scalar(size, 10);

    int nbMarkets = 1;
    PnlVect *trends = pnl_vect_create_from_scalar(size, 0.25);
    PnlVect *dividends = pnl_vect_create_from_double(size, 0.0);

    PnlVect_Pool gaussianPool;
    gaussianPool.init(O->nbAssets);

    PnlVect_Pool assetsPool;
    assetsPool.init(O->nbAssets);

    BlackScholesModel *bSM = new BlackScholesModel(O->nbAssets, r, trends, dividends, sigma, spot, rho, &gaussianPool,&assetsPool);

    double fdStep = 0.01;
    int nbSamples = 100000;

    PnlRng_Pool poolRng;
    poolRng.init(0);

    MonteCarloPricer *MC = new MonteCarloPricer(bSM, O, fdStep, nbSamples,&poolRng);

    double prix;
    double ic;

    time_t before;
    time_t after;
    double computingTime;

    std::cout << "Call vanille :" <<"\n";
    std::cout <<  "Prix spot du sous jacent S0 = " << GET(spot,0) << " €" <<"\n";
    std::cout <<  "Strike K = " << strike << " €" <<"\n";
    std::cout <<  "Echéance T = " << T << " an" <<"\n";
    std::cout <<  "Nombre de dates de constatation = " << nbTimeSteps <<"\n";
    std::cout <<  "Volatilité du sous-jacent = " << GET(sigma,0) <<"\n";
    std::cout <<  "Tendance = " << GET(trends,0) <<"\n";

    time(&before);
	MC->price(NULL, 0,prix,ic,true);
    time(&after);
    computingTime = difftime(after,before);

    std::cout << "\nCalcul du prix du Call " << " \n" ;
	std::cout << "Prix du Call en t = 0 :  " << prix << " €\n";
    std::cout << "Temps de calcul = " << computingTime << " secondes\n\n";

    std::cout << "Simulation de la couverture : " << " \n" ;
    bool isParallel = true;
    time(&before);
    SimulationHedger::hedging(MC,O->nbTimeSteps*4*3,"ProductPrices.txt","PortfolioPrices.txt","time.txt",isParallel);
    time(&after);
    computingTime = difftime(after,before);
    std::cout << "Temps de calcul = " << computingTime << " secondes";

    /*
    PnlVect *delta = pnl_vect_create(O->nbAssets);
    PnlVect *deltaIC = pnl_vect_create(O->nbAssets);

    time(&before);
    MC->delta(NULL, 0, delta,deltaIC,true);
    time(&after);
    computingTime = difftime(after,before);

    for (int i = 0; i < O->nbAssets; i++) {
        std::cout << "delta[" << i << "] = " << GET(delta, i) << "\n";
        std::cout << "ic[" << i << "] = " << GET(deltaIC, i) << "\n";
    }
    std::cout << "Computing Time = " << computingTime << " seconds";
    */

    pnl_vect_free(&lambda);
    delete bSM;
    delete MC;
}

