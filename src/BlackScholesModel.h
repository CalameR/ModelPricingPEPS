#pragma once

#include "Model.h"
#include "Utils.h"


/// \brief Mod�le de Black Scholes
/// \brief Modele de Black Scholes
class BlackScholesModel : public Model {
private:
	PnlMat *cholesky;

    PnlVect_Pool *gaussianPool;
    PnlVect_Pool *assetsPool;
    PnlVect_Pool *spotsPool;


    typedef double (BlackScholesModel::*TrendFunc)(int);


public:
	double interestRate;

	PnlVect *trends;
	PnlVect *dividends;

	PnlVect *volatilities;

	PnlMat *correlations;

	BlackScholesModel(int dim, double interestRate, PnlVect *trends, PnlVect *dividends, PnlVect *volatilities,
					  PnlVect *spots, PnlMat *correlations);

	void simulateUnderHistoricalProba(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng,
											  const PnlMat *past = NULL);

	void simulateUnderRiskNeutralProba(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng,
											   const PnlMat *past = NULL);

	double getSumForwardRates(double t, double T) { return interestRate*(T - t); }

    modelType getModelType() { return BLACK_SCHOLES_MODEL; }

	virtual ~BlackScholesModel();

private:
	void simulate(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng,
				  const PnlMat *past, TrendFunc trendFunc);
	void  nextIteration(PnlMat *path,const PnlVect *spot,int row,double timeStep,PnlRng *rng,TrendFunc trendFunc);
	bool isRecognitionDate(double t, double T, int nbTimeSteps);
	double getRiskNeutralTrend(int d) { return interestRate - GET(dividends, d); }
	double getTrend(int d) { return GET(trends, d) - GET(dividends, d); }
	void checkingInput(int dim, double interestRate, PnlVect *trends, PnlVect *dividends, PnlVect *volatilities,
					   PnlVect *spots, PnlMat *correlations);

	// To delete once the model is finished
	void simulateDebug(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng, const PnlMat *past,
					   TrendFunc trendFunc);

};
