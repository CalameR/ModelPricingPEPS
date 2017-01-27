//
// Created by courtocy on 1/27/17.
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
    int nbTimeSteps1 = 1;
    double strike1 = 14;
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

    BlackScholesModel *bSM1 = new BlackScholesModel(size1,r1,trends,dividends,sigma1,spot1,rho);

    double fdStep1 = 0.0001;
    int nbSamples1 = 500000;

    MonteCarloPricer *MC = new MonteCarloPricer(bSM1, O1, fdStep1, nbSamples1);


    PnlVect *delta = pnl_vect_create(O1->nbAssets);
    PnlVect *deltaIC = pnl_vect_create(O1->nbAssets);

    double realPrix;
    double realDelta;
    pnl_cf_call_bs(spot,strike1,T1,r1,divid,sigma,&realPrix,&realDelta);

    double nbPrices = 10000;
    double prix;
    double ic;
    PnlVect *prices = pnl_vect_create(nbPrices);
    for (int i = 0; i < nbPrices; i++) {
        std::cout << i << '/' << nbPrices << std::endl;
        MC->price(NULL,0,prix,ic,true);
        LET(prices,i) = prix-realPrix;
    }

    FILE* dataf;
    dataf = fopen("callPriceData.txt","w");
    if (dataf == NULL) return 1;
    else {
        pnl_vect_fprint(dataf,prices);
    }
    fclose(dataf);

    pnl_vect_free(&prices);
    pnl_vect_free(&lambda1);
    pnl_vect_free(&sigma1);
    pnl_vect_free(&spot1);

}

