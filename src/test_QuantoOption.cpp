//
// Created by courtocy on 1/25/17.
//


#include <iostream>
#include <ctime>
#include <cassert>
#include "QuantoOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "SimulationHedger.h"

int main(int argc, char **argv)
{
    /* 2nd test : Basket Option */
    double T2 = 1;
    int size2 = 2;
    int nbTimeSteps2 = 1;
    double strike2 = 100;
    PnlVect* lambda2 = pnl_vect_create_from_scalar(size2,1./((double)size2 - 1.));

    double rEUR = 0.01;
    double rUSD = 0.02;

    PnlVect *rates = pnl_vect_create(2);
    LET(rates,EUR) = rEUR;
    LET(rates,USD) = rUSD;

    RatesMarkets *ratesMarkets = new ConstantRatesMarkets(rEUR,rates);

    Product *Opt2 = new QuantoOption(T2,size2,nbTimeSteps2,lambda2,strike2,USD,ratesMarkets);

    double r2 = rEUR;
    double rho2 = 0.0;
    PnlVect *sigma2 = pnl_vect_create(size2);
    LET(sigma2,0) = 0.2;
    LET(sigma2,1) = 0.01;
    PnlVect *spot2 = pnl_vect_create(size2);
    LET(spot2,0) = 100;
    LET(spot2,1) = 1.07525;

    PnlVect *trends = pnl_vect_create_from_double(size2,0.1);
    PnlVect *dividends = pnl_vect_create_from_double(size2,0.);
    PnlMat *rho = pnl_mat_create_from_double(size2,size2,rho2);
    pnl_mat_set_diag(rho,1.,0);


    BlackScholesModel *bSM2 = new BlackScholesModel(size2,r2,ratesMarkets,trends,dividends,sigma2,spot2,rho);

    double fdStep2 = 10.* FLT_EPSILON;
    int nbSamples2 = 10000;


    MonteCarloPricer *MC2 = new MonteCarloPricer(bSM2, Opt2, fdStep2, nbSamples2);
    double prix2;
    double ic2;
    MC2->price(NULL,0,prix2,ic2,false);

    std::cout << "prix = " << prix2 << "\n";
    std::cout << "ic = " << ic2 << "\n";
    cout << "2nd MonteCarlo Initial Price OK" << "\n";


    std::cout << "Simulation de la couverture : " << " \n" ;

    SimulationHedger::hedging_PL_Prices(MC2,Opt2->nbTimeSteps*365,"QuantoPrices.txt","PortfolioPricesQuanto.txt","timeQuanto.txt",true);
    pnl_vect_free(&lambda2);
    pnl_vect_free(&sigma2);
    pnl_vect_free(&spot2);

}

