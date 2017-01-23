//
// Created by calamer on 1/10/17.
//

#include <iostream>
#include "BasketOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
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

    MonteCarloPricer *MC1 = new MonteCarloPricer(bSM1, O1, fdStep1, nbSamples1, &poolRng);
    double prix1;
    double ic1;
    MC1->price(NULL,0,prix1,ic1,false);

    std::cout << "prix = " << prix1 << "\n";

    assert((prix1 - ic1 < 1.423) && (prix1 + ic1 > 1.423));
    cout << "1st MonteCarlo Initial Price OK" << "\n";

    pnl_vect_free(&lambda1);
    pnl_vect_free(&sigma1);
    pnl_vect_free(&spot1);
}

