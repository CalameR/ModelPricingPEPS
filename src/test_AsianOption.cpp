//
// Created by calamer on 1/23/17.
//

#include <iostream>
#include <cassert>
#include "AsianOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "SimulationHedger.h"

int main(int argc, char **argv)
{
    /* 3rd test : Asian Option */

    double T3 = 1.5;
    int size3 = 2;
    int nbTimeSteps3 = 52+26;
    PnlVect* lambda3 = pnl_vect_create_from_scalar(size3,0.5);

    double strike3 = 100;

    Product *Opt3 = new AsianOption(T3,size3,nbTimeSteps3,lambda3,strike3);

    double r3 = 0.02;
    double rho3 = 0.0;
    PnlVect *sigma3 = pnl_vect_create_from_scalar(size3,0.2);
    PnlVect *spot3 = pnl_vect_create_from_scalar(size3,100);

    PnlVect *trends = pnl_vect_create_from_double(size3,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size3,0.);
    PnlMat *rho = pnl_mat_create_from_double(size3,size3,rho3);
    pnl_mat_set_diag(rho,1.,0);

    BlackScholesModel *bSM3 = new BlackScholesModel(size3,r3,trends,dividends,sigma3,spot3,rho);

    double fdStep3 = 0.01;
    int nbSamples3 = 50000;

    MonteCarloPricer *MC3 = new MonteCarloPricer(bSM3, Opt3, fdStep3, nbSamples3);
    //double prix3;
    //double ic3;

    //MC3->price(NULL,0,prix3,ic3,false);

    //std::cout << "prix = " << prix3 << "\n";

    //assert((prix3 - ic3 < 4.67) && (prix3 + ic3 > 4.67));
    //cout << "3rd MonteCarlo Initial Price OK" << "\n";

    time_t before;
    time_t after;
    double computingTime;

    std::cout << "Simulation de la couverture : " << " \n" ;
    time(&before);
    SimulationHedger::hedging_PL_Prices(MC3,Opt3->nbTimeSteps*7,"AsianPrices.txt","PortfolioPricesAsian.txt","timeAsian.txt",false);
    time(&after);
    computingTime = difftime(after,before);

    //std::cout << "PL = " << PL << " €\n";
    std::cout << "Temps de calcul = " << computingTime << " secondes";

    pnl_vect_free(&lambda3);
    pnl_vect_free(&sigma3);
    pnl_vect_free(&spot3);
}

