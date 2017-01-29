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
    double r1 = 0.05;
    PnlVect *rates = pnl_vect_create_from_scalar(1,r1);

    RatesMarkets *ratesMarkets = new ConstantRatesMarkets(r1,rates);

    double T1 = 1;
    int size1 = 1;
    int nbTimeSteps1 = 1;
    PnlVect* lambda1 = pnl_vect_create_from_scalar(size1,1);

    double rho1 = 1;
    double sigma = 0.3;
    PnlVect *sigma1 = pnl_vect_create_from_scalar(size1,sigma);
    double spot = 100;
    PnlVect *spot1 = pnl_vect_create_from_scalar(size1,spot);
    PnlVect *trends = pnl_vect_create_from_double(size1,0.);
    double divid = 0.;
    PnlVect *dividends = pnl_vect_create_from_double(size1,divid);
    PnlMat *rho = pnl_mat_create_from_double(size1,size1,rho1);

    BlackScholesModel *bSM1 = new BlackScholesModel(size1,r1,ratesMarkets,trends,dividends,sigma1,spot1,rho);

    double fdStep1 = 0.0001;
    int nbSamples1 = 5000000;
    double realPrix;
    double realDelta;

    double prix;
    double ic;


    PnlMat *data = pnl_mat_create(20,3);
    for (int i = 1; i <= 20; i++) {
        double strike1 = spot * (double)(i) / 10.;
        std::cout << strike1 << std::endl;
        pnl_cf_call_bs(spot,strike1,T1,r1,divid,sigma,&realPrix,&realDelta);
        Product *O1 = new BasketOption(T1,size1,nbTimeSteps1,lambda1,strike1);
        MonteCarloPricer *MC = new MonteCarloPricer(bSM1, O1, fdStep1, nbSamples1);
        MC->price(NULL,0,prix,ic,true);
        MLET(data,i-1,0) = strike1;
        MLET(data,i-1,1) = prix;
        MLET(data,i-1,2) = realPrix;
    }

    FILE* dataf;
    dataf = fopen("callStrikeData.txt","w");
    if (dataf == NULL) return 1;
    else {
        pnl_mat_fprint(dataf,data);
    }
    fclose(dataf);

    pnl_vect_free(&lambda1);
    pnl_vect_free(&sigma1);
    pnl_vect_free(&spot1);

}

