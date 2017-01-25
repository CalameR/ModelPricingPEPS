//
// Created by calamer on 1/23/17.
//


#include <iostream>
#include <ctime>
#include <cassert>
#include "BasketOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"

int main(int argc, char **argv)
{
    /* 2nd test : Basket Option */
    double T2 = 3;
    int size2 = 40;
    int nbTimeSteps2 = 1;
    double strike2 = 100;
    PnlVect* lambda2 = pnl_vect_create_from_scalar(size2,0.025);

    Product *Opt2 = new BasketOption(T2,size2,nbTimeSteps2,lambda2,strike2);

    double r2 = 0.04879;
    double rho2 = 0.0;
    PnlVect *sigma2 = pnl_vect_create_from_scalar(size2,0.2);
    PnlVect *spot2 = pnl_vect_create_from_scalar(size2,100);

    PnlVect *trends = pnl_vect_create_from_double(size2,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size2,0.);
    PnlMat *rho = pnl_mat_create_from_double(size2,size2,rho2);
    pnl_mat_set_diag(rho,1.,0);

    PnlVect_Pool gaussianPool;
    gaussianPool.init(size2);

    PnlVect_Pool assetsPool;
    assetsPool.init(size2);

    BlackScholesModel *bSM2 = new BlackScholesModel(size2,r2,trends,dividends,sigma2,spot2,rho,&gaussianPool,&assetsPool);

    double fdStep2 = 0.01;
    int nbSamples2 = 50000;

    PnlRng_Pool poolRng;
    poolRng.init(0);

    MonteCarloPricer *MC2 = new MonteCarloPricer(bSM2, Opt2, fdStep2, nbSamples2,&poolRng);
    double prix2;
    double ic2;
    MC2->price(NULL,0,prix2,ic2,false);

    std::cout << "prix = " << prix2 << "\n";

    assert((prix2 - ic2 < 13.627) && (prix2 + ic2 > 13.627));
    cout << "2nd MonteCarlo Initial Price OK" << "\n";

    pnl_vect_free(&lambda2);
    pnl_vect_free(&sigma2);
    pnl_vect_free(&spot2);

}

