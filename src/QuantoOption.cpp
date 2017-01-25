//
// Created by courtocy on 1/25/17.
//

#include <pnl/pnl_mathtools.h>
#include "QuantoOption.h"

double QuantoOption::payoff(const PnlMat* path) const {
    double optionPrice = 0;
    for(int d = 0; d < nbAssets - 1; d++) {
        optionPrice += MGET(path,nbTimeSteps,d) * GET(lambda,d);
    }
    return std::max(optionPrice / MGET(path,nbTimeSteps,nbAssets - 1) - strike, 0.);
}