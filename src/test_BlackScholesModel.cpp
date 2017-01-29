//
// Created by courtot on 25/01/17.
//

#include <iostream>
#include "BlackScholesModel.h"
#include <cassert>

using namespace std;

int main(int argc, char **argv)
{

    double r = 0.1;
    PnlVect *rates = pnl_vect_create_from_scalar(1,r);

    RatesMarkets *ratesMarkets = new ConstantRatesMarkets(r,rates);

    double T = 3;
    int size = 4;
    int nbTimeSteps = 10;
    PnlVect* lambda = pnl_vect_create_from_scalar(size,1/((double) size));
    PnlVect *sigma = pnl_vect_create_from_scalar(size,0.0);
    PnlVect *spot = pnl_vect_create_from_scalar(size,100);

    PnlVect *trends = pnl_vect_create_from_double(size,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size,0.);
    PnlMat *rho = pnl_mat_create_from_double(size,size,0.0);
    pnl_mat_set_diag(rho,1.,0);

    BlackScholesModel *bsm = new BlackScholesModel(size,r,ratesMarkets,trends,dividends,sigma,spot,rho);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng,time(NULL));
    PnlMat *path = pnl_mat_create(nbTimeSteps+1,size);
/*
    bsm->simulateUnderRiskNeutralProba(path,T,0.,nbTimeSteps,rng,NULL);

    pnl_mat_print(path);
    PnlMat *path2 = pnl_mat_create(nbTimeSteps+2,size);

    std::cout << std::endl;

    bsm->simulateUnderRiskNeutralProba(path2,T+T/nbTimeSteps,T,nbTimeSteps+1,rng,path);
    pnl_mat_print(path2);
*/
    cout << "testing isRecognition date" << endl;


    for (int i = 0; i <= nbTimeSteps; i++) {
        double timestep = T / (double)nbTimeSteps;
        double t = timestep * ((double) i);
        double mod = fmod(t, timestep);
        cout << t << endl;
    }

    cout << endl;

    return 0;
}