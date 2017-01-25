//
// Created by calamer on 1/13/17.
//

#include "SimulationHedger.h"

void hedging(int H, int i, MonteCarloPricer *monteCarloPricer, PnlMat *path,
             PnlMat *pathPayoff, PnlVect *previousDeltas, PnlVect *deltas, PnlVect *deltasIC,
             PnlVect *assetsPrices, double &currentDate, double timeToNextDate, double &freePortfolioValue,
             double &riskPortfolioValue, bool isParallel)
{
    double capitalization;
    pnl_mat_get_row(assetsPrices, path, i);

    if (i == 0 || (i - 1) % (H / monteCarloPricer->prod->nbTimeSteps) != 0) {
        pnl_mat_set_row(pathPayoff, assetsPrices, pathPayoff->m - 1);
    } else {
        pnl_mat_add_row(pathPayoff, pathPayoff->m, assetsPrices);
    }

    monteCarloPricer->delta(pathPayoff, currentDate, deltas, deltasIC, isParallel);

    capitalization = exp(monteCarloPricer->mod->getSumForwardRates(currentDate, currentDate + timeToNextDate));

    if (i == 0) {

        double priceProduct, ic;
        monteCarloPricer->price(pathPayoff,currentDate,priceProduct,ic,isParallel);

        riskPortfolioValue = pnl_vect_scalar_prod(deltas, assetsPrices);
        freePortfolioValue = priceProduct - riskPortfolioValue;

    } else {

        freePortfolioValue = freePortfolioValue * capitalization - pnl_vect_scalar_prod(deltas, assetsPrices) +
                             pnl_vect_scalar_prod(previousDeltas, assetsPrices);
        riskPortfolioValue = pnl_vect_scalar_prod(deltas, assetsPrices);
    }

    pnl_vect_clone(previousDeltas, deltas);
    currentDate += timeToNextDate;
}

void SimulationHedger::hedging_PL_Prices(MonteCarloPricer *monteCarloPricer, int H, char* fileProductName, char* filePortfolioName, char* fileTimeName, bool isParallel) {

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

    double currentDate = 0;
    double timeToNextDate = (monteCarloPricer->prod->maturity)/((double)H);

    double price;
    double ic;

    double freePortfolioValue;
    double riskPortfolioValue;

    ofstream fileProduct(fileProductName,ios::out|ios::trunc);
    ofstream filePortfolio(filePortfolioName,ios::out|ios::trunc);
    ofstream fileTime(fileTimeName,ios::out|ios::trunc);

    if (fileProduct && filePortfolio && fileTime) {

        monteCarloPricer->mod->simulateUnderHistoricalProba(path,monteCarloPricer->prod->maturity,0, H,(*monteCarloPricer->poolRng)(),NULL);

        if (monteCarloPricer->prod->getName() == "Actigo") {

            ofstream fileESX50("ESX50.txt",ios::out|ios::trunc);
            ofstream fileSSP500("SSP500.txt",ios::out|ios::trunc);
            ofstream fileSSP200("SSP200.txt",ios::out|ios::trunc);
            ofstream fileXED("XED.txt",ios::out|ios::trunc);
            ofstream fileXEA("XEA.txt",ios::out|ios::trunc);

            double esx50_0 = MGET(path, 0, 0);
            double ssp500_0 = MGET(path, 0, 1);
            double ssp200_0 = MGET(path, 0, 2);
            double xed_0 = MGET(path, 0, 3);
            double xea_0 = MGET(path, 0, 4);
            for (int i=0; i <= H; i++) {
                fileESX50 << MGET(path,i,0)/esx50_0 << "\n";
                fileSSP500 << MGET(path,i,1)/ssp500_0 << "\n";
                fileSSP200 << MGET(path,i,2)/ssp200_0 << "\n";
                fileXED << MGET(path,i,3)/xed_0 << "\n";
                fileXEA << MGET(path,i,4)/xea_0 << "\n";
            }
            fileESX50.close();
            fileSSP500.close();
            fileSSP200.close();
            fileXEA.close();
            fileXED.close();

        }


        for (int i = 0; i <= H; i++) {
            hedging(H,i,monteCarloPricer,path,pathPayoff,previousDeltas,deltas,deltasIC,assetsPrices,currentDate,timeToNextDate,freePortfolioValue,riskPortfolioValue,isParallel);
            monteCarloPricer->price(pathPayoff,currentDate-timeToNextDate,price,ic,isParallel);
            filePortfolio << freePortfolioValue + riskPortfolioValue << "\n";
            fileProduct << price << "\n";
        }

        for (int i=0; i <= H; i++) {
            double t = ((double)i)*(monteCarloPricer->prod->maturity)/(double)H;
            fileTime << t << "\n";
        }

        fileProduct.close();
        filePortfolio.close();
        fileTime.close();

        std::cout << "Prix du produit à échéance = " << price << " €\n";
        std::cout << "Valeur du portefeuille de couverture = " << freePortfolioValue + riskPortfolioValue << " €\n";
        double PL = freePortfolioValue + riskPortfolioValue - price;
        std::cout << "P&L = " << PL << " €\n";

        std::cout << "\nPour visualiser les cours des actifs, du portefeuille et du produit, lancer les scripts R " << " \n";
    }

    pnl_mat_free(&pathPayoff);
    pnl_vect_free(&previousDeltas);
    pnl_vect_free(&assetsPrices);
    pnl_vect_free(&deltas);
    pnl_mat_free(&path);
    pnl_vect_free(&deltasIC);

}

double SimulationHedger::hedging_PL(MonteCarloPricer *monteCarloPricer, int H, bool isParallel)
{

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

    double currentDate = 0;
    double timeToNextDate = (monteCarloPricer->prod->maturity)/((double)H);

    double freePortfolioValue = 0;
    double riskPortfolioValue = 0;

    monteCarloPricer->mod->simulateUnderHistoricalProba(path,monteCarloPricer->prod->maturity,0, H,(*monteCarloPricer->poolRng)(),NULL);

    for (int i = 0; i <= H; i++) {
        hedging(H,i,monteCarloPricer,path,pathPayoff,previousDeltas,deltas,deltasIC,assetsPrices,currentDate,timeToNextDate,freePortfolioValue,riskPortfolioValue,isParallel);
    }

    double priceProduct = monteCarloPricer->prod->payoff(pathPayoff);

    pnl_mat_free(&pathPayoff);
    pnl_vect_free(&previousDeltas);
    pnl_vect_free(&assetsPrices);
    pnl_vect_free(&deltas);
    pnl_mat_free(&path);
    pnl_vect_free(&deltasIC);

    return freePortfolioValue + riskPortfolioValue - priceProduct;
}


