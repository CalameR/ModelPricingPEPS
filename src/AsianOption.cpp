#include "AsianOption.h"
#include "pnl/pnl_mathtools.h"

AsianOption::AsianOption(double T, int size, int nbTimeSteps, PnlVect *lambda, double strike) {
    this->maturity = T;
    this->nbTimeSteps = nbTimeSteps;
    this->nbAssets = size;
    this->lambda = lambda;
    this->strike = strike;
    this->type = ASIAN;
}

double AsianOption::payoff(const PnlMat *path, PnlVect *spot) const {
    if (spot->size != path->n) pnl_vect_resize(spot,path->n);
    pnl_mat_sum_vect(spot, path, 'r');
    double basketPrice = pnl_vect_scalar_prod(lambda, spot);
    return MAX(basketPrice / ((double) nbTimeSteps + 1) - strike, 0);
}

AsianOption::~AsianOption(){
    pnl_vect_free(&lambda);
}
