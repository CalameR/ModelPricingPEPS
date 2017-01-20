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
    static void hedging(MonteCarloPricer *monteCarloPricer, int H, char* fileProductName, char* filePortfolioName, char* fileTimeName, bool isParallel);
};

#endif //MC_PRICER_SIMULATIONHEDGER_H