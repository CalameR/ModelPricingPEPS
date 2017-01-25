//
// Created by calamer on 1/10/17.
//

#include <iostream>
#include "BasketOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "SimulationHedger.h"
#include <cassert>

using namespace std;

int main(int argc, char **argv)
{
    /* 1st test : Vanilla Call Option */
    double T1 = 1;
    int size1 = 1;
    int nbTimeSteps1 = 10;
    double strike1 = 10;
    PnlVect* lambda1 = pnl_vect_create_from_scalar(size1,1);

    Product *O1 = new BasketOption(T1,size1,nbTimeSteps1,lambda1,strike1);

    double r1 = 0.05;
    double rho1 = 1;
    PnlVect *sigma1 = pnl_vect_create_from_scalar(size1,0.3);
    PnlVect *spot1 = pnl_vect_create_from_scalar(size1,10);
    PnlVect *trends = pnl_vect_create_from_double(size1,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size1,0.);
    PnlMat *rho = pnl_mat_create_from_double(size1,size1,rho1);

    PnlVect_Pool gaussianPool;
    gaussianPool.init(size1);

    PnlVect_Pool assetsPool;
    assetsPool.init(size1);

    BlackScholesModel *bSM1 = new BlackScholesModel(size1,r1,trends,dividends,sigma1,spot1,rho,&gaussianPool,&assetsPool);

    double fdStep1 = 0.01;
    int nbSamples1 = 100000;

    PnlRng_Pool poolRng;
    poolRng.init(0);

    MonteCarloPricer *MC = new MonteCarloPricer(bSM1, O1, fdStep1, nbSamples1,&poolRng);

    double prix;
    double ic;

    time_t before;
    time_t after;
    double computingTime;

    std::cout << "Call vanille :" <<"\n";
    std::cout <<  "Prix spot du sous jacent S0 = " << GET(spot1,0) << " €" <<"\n";
    std::cout <<  "Strike K = " << strike1 << " €" <<"\n";
    std::cout <<  "Echéance T = " << T1 << " an" <<"\n";
    std::cout <<  "Nombre de dates de constatation = " << nbTimeSteps1 <<"\n";
    std::cout <<  "Volatilité du sous-jacent = " << GET(sigma1,0) <<"\n";
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
    SimulationHedger::hedging_PL_Prices(MC,O1->nbTimeSteps,"ProductPrices.txt","PortfolioPrices.txt","time.txt",isParallel);
    time(&after);
    computingTime = difftime(after,before);
    std::cout << "Temps de calcul = " << computingTime << " secondes";

    /*
    PnlVect *delta = pnl_vect_create(O->nbAssets);
    PnlVect *deltaIC = pnl_vect_create(O->nbAssets);
>>>>>>> fa9ba4348acf948966221e160d325ebc0b4b2e7d

    std::cout << "prix = " << prix1 << "\n";

    assert((prix1 - ic1 < 1.423) && (prix1 + ic1 > 1.423));
    cout << "1st MonteCarlo Initial Price OK" << "\n";

    pnl_vect_free(&lambda1);
    pnl_vect_free(&sigma1);
    pnl_vect_free(&spot1);
     */
}

