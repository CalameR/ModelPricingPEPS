//
// Created by calamer on 1/13/17.
//

#include "SimulationHedger.h"

void SimulationHedger::hedging(MonteCarloPricer *monteCarloPricer, int H, char* fileProductName, char* filePortfolioName, char* fileTimeName) {
    if (H <= 0) {
        throw std::invalid_argument("Number of discretization steps must be positive!");
    }

    if ((H % monteCarloPricer->prod->nbTimeSteps) != 0) {
        throw std::invalid_argument("Number of discretization steps must be a multiple of time steps!");
    }

    PnlMat *path = pnl_mat_create(H + 1, monteCarloPricer->prod->nbAssets);
    PnlMat *pathPayoff = pnl_mat_create(1, monteCarloPricer->prod->nbAssets);

    PnlVect *deltas = pnl_vect_create(monteCarloPricer->prod->nbAssets);
    PnlVect *deltasIC = pnl_vect_create(monteCarloPricer->prod->nbAssets);
    PnlVect *previousDeltas = pnl_vect_create(monteCarloPricer->prod->nbAssets);

    PnlVect *assetsPrices = pnl_vect_create(monteCarloPricer->prod->nbAssets);

    //PnlVect *portfolioValue = pnl_vect_create(H+1);

    double currentDate = 0;
    double timeToNextDate = (monteCarloPricer->prod->maturity)/((double)H);
    double capitalization;

    double price;
    double ic;

    double portfolioValue;
    double freePortfolioValue;
    double riskPortfolioValue;

    fstream fileProduct(fileProductName,ios::out|ios::trunc);
    ofstream filePortfolio(filePortfolioName,ios::out|ios::trunc);
    ofstream fileTime(fileTimeName,ios::out|ios::trunc);

    if (fileProduct && filePortfolio && fileTime) {
        monteCarloPricer->mod->simulateUnderHistoricalProba(path,monteCarloPricer->prod->maturity,0, H,(*monteCarloPricer->poolRng)(),NULL);

        for (int i = 0; i <= H; i++) {

            pnl_mat_get_row(assetsPrices, path, i);

            if (i == 0 || (i-1) % (H / monteCarloPricer->prod->nbTimeSteps) != 0) {
                pnl_mat_set_row(pathPayoff, assetsPrices, pathPayoff->m - 1);
            } else {
                pnl_mat_add_row(pathPayoff, pathPayoff->m, assetsPrices);
            }

            monteCarloPricer->price(pathPayoff,currentDate,price,ic,true);
            monteCarloPricer->delta(pathPayoff,currentDate,deltas,deltasIC,true);

            capitalization = exp(monteCarloPricer->mod->getSumForwardRates(currentDate,currentDate+timeToNextDate));

            if (i == 0) {
                riskPortfolioValue = pnl_vect_scalar_prod(deltas, assetsPrices);
                freePortfolioValue = price - riskPortfolioValue;
            } else {
                freePortfolioValue = freePortfolioValue * capitalization - pnl_vect_scalar_prod(deltas, assetsPrices) +
                                     pnl_vect_scalar_prod(previousDeltas, assetsPrices);
                riskPortfolioValue = pnl_vect_scalar_prod(deltas, assetsPrices);
            }

            pnl_vect_clone(previousDeltas,deltas);

            currentDate += timeToNextDate;

            portfolioValue = riskPortfolioValue + freePortfolioValue;

            filePortfolio << portfolioValue << "\n";
            fileProduct << price << "\n";
        }
        for (int i=0; i <= H; i++) {
            double t = i*(monteCarloPricer->prod->maturity)/(double)H;
            fileTime << t << "\n";
        }
        fileProduct.close();
        filePortfolio.close();
        fileTime.close();

        std::cout << "Product Price = " << price << " €\n";
        std::cout << "Portfolio Price = " << portfolioValue << " €\n";
        double PL = portfolioValue - price;
        std::cout << "P&L = " << PL << " €\n";
    }

    pnl_mat_free(&pathPayoff);
    pnl_vect_free(&previousDeltas);
    pnl_vect_free(&assetsPrices);
    pnl_vect_free(&deltas);
    pnl_mat_free(&path);
    pnl_vect_free(&deltasIC);

}