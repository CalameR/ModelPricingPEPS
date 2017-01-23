#include "PerformanceOption.h"
#include "pnl/pnl_mathtools.h"

PerformanceOption::PerformanceOption(double T, int size, int nbTimeSteps, PnlVect *lambda) {
    this->maturity = T;
    this->nbTimeSteps = nbTimeSteps;
    this->nbAssets = size;
    this->lambda = lambda;
    this->spot = pnl_vect_create(size);
}

double PerformanceOption::payoff(const PnlMat *path) const {
    double res = 1;
    PnlVect *spot = pnl_mat_mult_vect(path,lambda);
    double oBP = GET(spot,0);
    double nBP;
    for (int i = 1; i <= nbTimeSteps; i++) {
        nBP = GET(spot,i);
        res += MAX(nBP/oBP - 1.0,0.0);
        oBP = nBP;
    }
    return res;
}

PerformanceOption::~PerformanceOption(){
    pnl_vect_free(&spot);
}
