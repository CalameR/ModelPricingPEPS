#pragma once
#include "Pricer.h"
#include "Model.h"
#include "pool_pattern.h"
#include "pnl/pnl_random.h"

class MonteCarloPricer : public Pricer {
public:
	int nbSamples;
	Model *mod;
	PnlRng_Pool *poolRng;
	double fdStep;

	MonteCarloPricer(Model *mod, Product *prod, double fdStep, int nbSamples, PnlRng_Pool *poolRng);

	void price(const PnlMat *past, double t, double &price, double &ic, bool parallel);
	void delta(const PnlMat *past, double t, PnlVect *delta, PnlVect *ic, bool parallel);

};
