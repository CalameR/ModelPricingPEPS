#include "MonteCarloPricer.h"
#include <fstream>
#include "omp.h"

using namespace std;

MonteCarloPricer::MonteCarloPricer(Model *mod, Product *prod, double fdStep, int nbSamples) : Pricer(prod)
{
    this->nbSamples = nbSamples;
    this->mod = mod;

    this->poolRng = new PnlRng_Pool();
    this->poolRng->init(0,0);

    this->path = new PnlMat_Pool();
    this->path->init(prod->nbTimeSteps + 1, prod->nbAssets);

    this->shiftedPath = new PnlMat_Pool();
    this->shiftedPath->init(prod->nbTimeSteps + 1, prod->nbAssets);

    this->st = pnl_vect_copy(mod->spots);
    this->st2 = pnl_vect_copy(this->st);
    this->sum2 = pnl_vect_create(prod->nbTimeSteps + 1);
    pnl_vect_mult_vect_term(this->st2,this->st);

    this->fdStep = fdStep;
}

void MonteCarloPricer::price(const PnlMat *past, double t, double &price, double &ic, bool parallel)
{
    double sum = 0.;
    double sum2 = 0.;

    if (parallel) {
#pragma omp parallel shared(sum,sum2,t,past)
        {
#pragma omp for reduction(+:sum,sum2)
            for (int i = 0; i < nbSamples; i++) {
                mod->simulateUnderRiskNeutralProba((*path)(), prod->maturity, t, prod->nbTimeSteps, (*poolRng)(), past);
                double payoff = prod->payoff((*path)());
                sum += payoff;
                sum2 += pow(payoff, 2);
            }
        };
    } else {
        for (int i = 0; i < nbSamples; i++) {
            mod->simulateUnderRiskNeutralProba((*path)(), prod->maturity, t, prod->nbTimeSteps, (*poolRng)(), past);
            double payoff = prod->payoff((*path)());
            sum += payoff;
            sum2 += pow(payoff, 2);
        }
    }
    price = (exp(-mod->getSumForwardRates(t, prod->maturity))*sum)/nbSamples;

    double ksi_carre = exp(-2. * mod->getSumForwardRates(t, prod->maturity));
    ksi_carre *= (sum2 / nbSamples - pow(sum / nbSamples, 2));
    ic = 2 * 1.96 * sqrt(ksi_carre / nbSamples);
    return;
}

// ///!!!\\\
// delta->size == prod->nbAssets
// ic->size == prod->nbAssets
void MonteCarloPricer::delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, bool parallel)
{
    pnl_vect_set_all(delta, 0.);
    pnl_vect_set_all(ic, 0.);
    pnl_vect_set_all(sum2,0.);

    if (t != 0) {
        if (past == NULL) {
            throw std::invalid_argument("Can not compute deltas at t > 0 with past matrix equals to NULL pointer");
        }
        pnl_mat_get_row(st,past,past->m - 1);
        pnl_mat_get_row(st2,past,past->m - 1);
        pnl_vect_mult_vect_term(st2, st);
    }

    double ksi_carre = 0;
    double timeStep = prod->maturity / ((double) prod->nbTimeSteps);

    if (parallel) {
#pragma omp parallel
        {
            int j;
#pragma omp for private(j)
            for (int i = 0; i < nbSamples; i++) {
                mod->simulateUnderRiskNeutralProba((*path)(), prod->maturity, t, prod->nbTimeSteps, (*poolRng)(), past);
                for (j = 0; j < st->size; j++) {
                    mod->shiftSimulation((*shiftedPath)(), (*path)(), j, -fdStep, t, timeStep);
                    double payoffMoins = prod->payoff((*shiftedPath)());

                    mod->shiftSimulation((*shiftedPath)(), (*path)(), j, fdStep, t, timeStep);
                    double payoffPlus = prod->payoff((*shiftedPath)());

#pragma omp atomic
                    LET(delta, j) += payoffPlus - payoffMoins;
#pragma omp atomic
                    LET(sum2, j) += pow(payoffPlus - payoffMoins, 2);
                }
            }
            // delta <- sum(j=1 à nbSamples) (payoff(shiftSimulation(S,1+fdStep))-payoff(shiftSimulation(S,1-fdStep))
        };
    }
    else {
        int j;
        for (int i = 0; i < nbSamples; i++) {
            mod->simulateUnderRiskNeutralProba((*path)(), prod->maturity, t, prod->nbTimeSteps, (*poolRng)(), past);
            for (j = 0; j < st->size; j++) {
                mod->shiftSimulation((*shiftedPath)(), (*path)(), j, -fdStep, t, timeStep);
                double payoffMoins = prod->payoff((*shiftedPath)());

                mod->shiftSimulation((*shiftedPath)(), (*path)(), j, fdStep, t, timeStep);
                double payoffPlus = prod->payoff((*shiftedPath)());
                LET(delta, j) += payoffPlus - payoffMoins;
                LET(sum2, j) += pow(payoffPlus - payoffMoins, 2);
            }
        }
        // delta <- sum(j=1 à nbSamples) (payoff(shiftSimulation(S,1+fdStep))-payoff(shiftSimulation(S,1-fdStep))
    }

    for (int j = 0; j < st->size; j++) {
        ksi_carre = (exp(-2. * mod->getSumForwardRates(t, prod->maturity)) / (4. * fdStep * fdStep * GET(st2, j))) *
                    (LET(sum2, j) / nbSamples - pow(LET(delta, j) / nbSamples, 2));
        LET(ic, j) = 2. * 1.96 * sqrt(ksi_carre / nbSamples);
    }

    // delta <- delta*exp(-integrale(t,T,r(s)ds)/(2*nbSamples*fdStep)
    pnl_vect_mult_scalar(delta, exp(-mod->getSumForwardRates(t, prod->maturity)) / (2. * nbSamples * fdStep));

    // delta <- delta/st
    pnl_vect_div_vect_term(delta, st);

    return;
}
