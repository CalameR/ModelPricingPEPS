#include "DefinitionTypes.h"
#include "RatesMarkets.h"
#include <stdexcept>
#include "pool_pattern.h"
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_random.h"
#include <cmath>

#ifndef MODEL_H
#define MODEL_H

class Model {
public:
    RatesMarkets *ratesMarkets;
	int dim;
	int nbRiskAssets;
	PnlVect *spots;

	virtual void simulateUnderHistoricalProba(PnlMat *path, double T, double t, int nbTimeStep, PnlRng *rng, const PnlMat *past = NULL) = 0;
	virtual void simulateUnderRiskNeutralProba(PnlMat *path, double T, double t, int nbTimeStep, PnlRng *rng, const PnlMat *past = NULL) = 0;

	virtual ~Model() {};

	void shiftSimulation(PnlMat *shift_path, const PnlMat *path, int d, double h, double t, double timestep)
    {
        double ceil = std::ceil(t/timestep);
        int row = (int) ceil;
        if ((fabs(t/timestep - ceil) <= FLT_EPSILON)
            and (t/timestep <= ceil)){
            row++;
        }

        pnl_mat_set_subblock(shift_path, path, 0, 0);

        for (int i = row; i < path->m; i++) {
            MLET(shift_path,i,d) *= (1.0 + h);
        }
    }

    virtual modelType getModelType() const { return type; };

protected:
    modelType type;
};

#endif