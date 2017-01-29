#include "PerformanceOption.h"
#include "pnl/pnl_mathtools.h"

PerformanceOption::PerformanceOption(double T, int size, int nbTimeSteps, PnlVect *lambda) {
    this->maturity = T;
    this->nbTimeSteps = nbTimeSteps;
    this->nbAssets = size;
    this->lambda = lambda;
    this->type = PERFORMANCE;
}

double PerformanceOption::payoff(const PnlMat *path, PnlVect *spot) const {
    double res = 1;
    if (spot->size != path->m) pnl_vect_resize(spot,path->m);
    pnl_mat_mult_vect_inplace(spot,path,lambda);
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
    pnl_vect_free(&lambda);
}
