#pragma once
#include "Pricer.h"
#include "Model.h"
#include "pool_pattern.h"
#include "pnl/pnl_random.h"

class MonteCarloPricer : public Pricer {
public:
	int nbSamples;
	Model *mod;
	double fdStep;
    PnlRng_Pool *poolRng;

	MonteCarloPricer(Model *mod, Product *prod, double fdStep, int nbSamples);

	void price(const PnlMat *past, double t, double &price, double &ic, bool parallel);
	void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, bool parallel);
private:
    PnlMat_Pool *path;
    PnlMat_Pool *shiftedPath;
	PnlVect_Pool *spotsPool;
    PnlVect *st;
    PnlVect *st2;
    PnlVect *sum2;

};
