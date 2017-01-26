//
// Created by calamer on 1/23/17.
//

#include <iostream>
#include <cassert>
#include "BasketOption.h"
#include "PerformanceOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "SimulationHedger.h"

int main(int argc, char **argv)
{
    /* 4th test: Performance Option */

    double T4 = 2;
    int size4 = 5;
    int nbTimeSteps4 = 12;
    PnlVect* lambda4 = pnl_vect_create_from_scalar(size4,0.2);

    Product *Opt4 = new PerformanceOption(T4,size4,nbTimeSteps4,lambda4);

    double r4 = 0.03;
    double rho4 = 0.5;
    PnlVect *sigma4 = pnl_vect_create_from_scalar(size4,0.2);
    PnlVect *spot4 = pnl_vect_create_from_scalar(size4,100);

    PnlVect *trends = pnl_vect_create_from_double(size4,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size4,0.);
    PnlMat *rho = pnl_mat_create_from_double(size4,size4,rho4);
    pnl_mat_set_diag(rho,1.,0);

    BlackScholesModel *bSM4 = new BlackScholesModel(size4,r4,trends,dividends,sigma4,spot4,rho);

    double fdStep4 = 0.01;
    int nbSamples4 = 1;

    MonteCarloPricer *MC4 = new MonteCarloPricer(bSM4, Opt4, fdStep4, nbSamples4);
    double prix4;
    double ic4;

    /*
    MC4->price(NULL,0,prix4,ic4,true);

    std::cout << "prix = " << prix4 << "\n";

    assert((prix4 - ic4 < 1.257353) && (prix4 + ic4 > 1.257353));
    cout << "4rd MonteCarlo Initial Price OK" << "\n";
    */

    time_t before;
    time_t after;
    double computingTime;


    std::cout << "Simulation de la couverture : " << " \n" ;
    time(&before);
    SimulationHedger::hedging_PL_Prices(MC4,Opt4->nbTimeSteps*2,"PerfPrices.txt","PortfolioPricesPerf.txt","timePerf.txt",true);
    time(&after);
    computingTime = difftime(after,before);
    std::cout << "Temps de calcul = " << computingTime << " secondes";

    pnl_vect_free(&lambda4);
    pnl_vect_free(&sigma4);
    pnl_vect_free(&spot4);
}