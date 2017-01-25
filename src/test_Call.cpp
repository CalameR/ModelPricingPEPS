//
// Created by calamer on 1/10/17.
//

#include <iostream>
#include "BasketOption.h"
#include "BlackScholesModel.h"
#include "MonteCarloPricer.h"
#include "pnl/pnl_finance.h"
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
    double sigma = 0.3;
    PnlVect *sigma1 = pnl_vect_create_from_scalar(size1,sigma);
    double spot = 10;
    PnlVect *spot1 = pnl_vect_create_from_scalar(size1,spot);
    PnlVect *trends = pnl_vect_create_from_double(size1,0.);
    double divid = 0.;
    PnlVect *dividends = pnl_vect_create_from_double(size1,divid);
    PnlMat *rho = pnl_mat_create_from_double(size1,size1,rho1);

    PnlVect_Pool gaussianPool;
    gaussianPool.init(size1);

    PnlVect_Pool assetsPool;
    assetsPool.init(size1);

    BlackScholesModel *bSM1 = new BlackScholesModel(size1,r1,trends,dividends,sigma1,spot1,rho,&gaussianPool,&assetsPool);

    double fdStep1 = 0.01;
    int nbSamples1 = 1000000;

    PnlRng_Pool poolRng;
    poolRng.init(0);

    MonteCarloPricer *MC = new MonteCarloPricer(bSM1, O1, fdStep1, nbSamples1,&poolRng);

    double prix;
    double ic;
    PnlVect *delta = pnl_vect_create(O1->nbAssets);
    PnlVect *deltaIC = pnl_vect_create(O1->nbAssets);

    double realPrix;
    double realDelta;


	MC->price(NULL,0,prix,ic,true);
    MC->delta(NULL,0,delta,deltaIC,true);
    pnl_cf_call_bs(spot,strike1,T1,r1,divid,sigma,&realPrix,&realDelta);

	std::cout << "Prix du Call en t = 0 :  " << prix << " €\n";
    std::cout << "Vrai prix du Call en t = 0 :  " << realPrix << " €\n";

    assert((prix - ic < realPrix) && (prix + ic > realPrix));
    cout << "MonteCarlo Initial Price OK" << "\n";

    std::cout << "Delta du Call en t = 0 :  " << GET(delta,0) << "\n";
    std::cout << "Vrai delta du Call en t = 0 :  " << realDelta << "\n";

    assert((GET(delta,0) - GET(deltaIC,0) < realDelta) && (GET(delta,0) + GET(deltaIC,0) > realDelta));
    cout << "MonteCarlo Initial Delta OK" << "\n";

    pnl_vect_free(&lambda1);
    pnl_vect_free(&sigma1);
    pnl_vect_free(&spot1);
}

