//
// Created by calamer on 1/13/17.
//

#include "MonteCarloPricer.h"
#include "pnl/pnl_random.h"
#include <iostream>
#include <fstream>

#ifndef MC_PRICER_SIMULATIONHEDGER_H
#define MC_PRICER_SIMULATIONHEDGER_H

class SimulationHedger {
public:
    static void hedging_PL_Prices(MonteCarloPricer *monteCarloPricer, int H, char* fileProductName, char* filePortfolioName, char* fileTimeName, bool isParallel);
    static double hedging_PL(MonteCarloPricer *monteCarloPricer, int H, bool isParallel);
    static void prices(MonteCarloPricer *monteCarloPricer, int H, char* fileProductName, char* fileTimeName, bool isParallel);
};

#endif //MC_PRICER_SIMULATIONHEDGER_H