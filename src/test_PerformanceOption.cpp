//
// Created by calamer on 1/23/17.
//

#include <iostream>
#include <cassert>
#include "BasketOption.h"
#include "PerformanceOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"

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

    PnlVect_Pool gaussianPool;
    gaussianPool.init(size4);

    PnlVect_Pool assetsPool;
    assetsPool.init(size4);

    BlackScholesModel *bSM4 = new BlackScholesModel(size4,r4,trends,dividends,sigma4,spot4,rho,&gaussianPool,&assetsPool);

    double fdStep4 = 0.01;
    int nbSamples4 = 500000;

    PnlRng_Pool poolRng;
    poolRng.init(0);

    MonteCarloPricer *MC4 = new MonteCarloPricer(bSM4, Opt4, fdStep4, nbSamples4,&poolRng);
    double prix4;
    double ic4;

    MC4->price(NULL,0,prix4,ic4,false);

    std::cout << "prix = " << prix4 << "\n";

    assert((prix4 - ic4 < 1.257353) && (prix4 + ic4 > 1.257353));
    cout << "4rd MonteCarlo Initial Price OK" << "\n";

    pnl_vect_free(&lambda4);
    pnl_vect_free(&sigma4);
    pnl_vect_free(&spot4);
}