#include <stdexcept>
#include "pool_pattern.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"

#ifndef MODEL_H
#define MODEL_H


typedef enum modelType {
    BLACK_SCHOLES_MODEL
} modelType;

class Model {
public:
	int dim;
	int nbRiskAssets;
	PnlVect *spots;
	PnlMat_Pool *modelLine;

	virtual void simulateUnderHistoricalProba(PnlMat *path, double T, double t, int nbTimeStep, PnlRng *rng, const PnlMat *past = NULL) = 0;
	virtual void simulateUnderRiskNeutralProba(PnlMat *path, double T, double t, int nbTimeStep, PnlRng *rng, const PnlMat *past = NULL) = 0;

	virtual ~Model() {};

	virtual double getSumForwardRates(double t, double T) = 0;

	void shiftSimulation(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);

    virtual modelType getModelType() = 0;

private:
	// To delete once the debugging is finished
	void shiftSimulationDebug(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep);
};

#endif