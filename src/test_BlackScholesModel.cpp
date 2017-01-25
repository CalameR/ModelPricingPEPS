//
// Created by courtot on 25/01/17.
//

#include <iostream>
#include "BlackScholesModel.h"
#include <cassert>

using namespace std;

int main(int argc, char **argv)
{
    double T = 3;
    int size = 4;
    int nbTimeSteps = 1;
    PnlVect* lambda = pnl_vect_create_from_scalar(size,1/((double) size));
    double r = 0.1;
    PnlVect *sigma = pnl_vect_create_from_scalar(size,0.0);
    PnlVect *spot = pnl_vect_create_from_scalar(size,100);

    PnlVect *trends = pnl_vect_create_from_double(size,0.);
    PnlVect *dividends = pnl_vect_create_from_double(size,0.);
    PnlMat *rho = pnl_mat_create_from_double(size,size,0.0);
    pnl_mat_set_diag(rho,1.,0);

    PnlVect_Pool gaussianPool;
    gaussianPool.init(size);

    PnlVect_Pool assetsPool;
    assetsPool.init(size);

    BlackScholesModel *bsm = new BlackScholesModel(size,r,trends,dividends,sigma,spot,rho,&gaussianPool,&assetsPool);

    PnlRng *rng = pnl_rng_create(PNL_RNG_MERSENNE);
    pnl_rng_sseed(rng,time(NULL));
    PnlMat *path = pnl_mat_create(nbTimeSteps+1,size);

    bsm->simulateUnderRiskNeutralProba(path,T,0.,nbTimeSteps,rng,NULL);

    pnl_mat_print(path);
    PnlMat *path2 = pnl_mat_create(nbTimeSteps+2,size);

    std::cout << std::endl;

    bsm->simulateUnderRiskNeutralProba(path2,T+T/nbTimeSteps,T,nbTimeSteps+1,rng,path);
    pnl_mat_print(path2);

    return 0;
}