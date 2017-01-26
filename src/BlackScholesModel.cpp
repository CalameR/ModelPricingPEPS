//
// Created by courtocy on 9/15/16.
//

#include "BlackScholesModel.h"
#include <tgmath.h>
#include <cmath>


BlackScholesModel::BlackScholesModel(int dim, double interestRate, PnlVect *trends, PnlVect *dividends, PnlVect *volatilities,
                                     PnlVect *spots, PnlMat *correlations)
{
	BlackScholesModel::checkingInput(dim, interestRate, trends, dividends, volatilities, spots, correlations);

	this->dim = dim;
	this->nbRiskAssets = dim;
	this->interestRate = interestRate;
	this->trends = pnl_vect_copy(trends);
	this->dividends = pnl_vect_copy(dividends);
	this->volatilities = pnl_vect_copy(volatilities);
	this->spots = pnl_vect_copy(spots);
	this->correlations = pnl_mat_copy(correlations);

    this->gaussianPool = new PnlVect_Pool();
    this->gaussianPool->init(this->nbRiskAssets,0);

	this->assetsPool = new PnlVect_Pool();
    this->assetsPool->init(this->nbRiskAssets,0);

    this->spotsPool = new PnlVect_Pool();
    this->spotsPool->init(this->nbRiskAssets,0);

	/// Correlations matrix is made positive and symetric if it is not the case
	correlations = Utils::higham(correlations);

	this->cholesky = pnl_mat_copy(correlations);
	pnl_mat_chol(this->cholesky);
}

inline void BlackScholesModel::nextIteration(PnlMat *path, const PnlVect *spots, int row, double timeStep, PnlRng *rng,
											 TrendFunc trendFunc)
{
	pnl_vect_rng_normal_d((*gaussianPool)(), dim, rng);

	pnl_mat_mult_vect_inplace((*assetsPool)(), cholesky, (*gaussianPool)());

	for (int d = 0; d < dim; d++) {
		double vol = GET(volatilities, d);
		double val = (*this.*(trendFunc))(d) - (vol*vol)/ 2.0;

		val *= timeStep;
		val += vol * sqrt(timeStep) * GET((*assetsPool)(), d);

		val = exp(val);

		pnl_vect_set((*assetsPool)(), d, val);
	}
	pnl_vect_mult_vect_term((*assetsPool)(), spots);
	pnl_mat_set_row(path, (*assetsPool)(), row);
}

inline void BlackScholesModel::simulate(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng,
										const PnlMat *past, TrendFunc trendFunc)
{
	// simulateDebug(path, T, t, nbTimeSteps, rng, past, trendFunc);
	// /*
	double timeStep = T / ((double)nbTimeSteps);


	if (t==0) {

        pnl_vect_clone((*spotsPool)(),spots);
		pnl_mat_set_row(path, (*spotsPool)(), 0);

		for (int row = 1; row <= nbTimeSteps; row++) {
			pnl_mat_get_row((*spotsPool)(), path, row - 1);
			nextIteration(path,(*spotsPool)(),row,timeStep,rng,trendFunc);
		}

	}
	else {

		if (t >= T) {
			pnl_mat_set_subblock(path, past, 0, 0);
			return;
		}

		int row;
		double fTimeStep;

		pnl_mat_set_subblock(path, past, 0, 0);

		pnl_mat_get_row((*spotsPool)(), past, past->m - 1);

		if (isRecognitionDate(t, T, nbTimeSteps)) {
			row = past->m;
			fTimeStep = timeStep;
		}
		else {
			row = past->m - 1;
			fTimeStep = ((double)row) * timeStep - t;
		}


		for (; row <= nbTimeSteps; row++) {
			nextIteration(path,(*spotsPool)(),row,fTimeStep,rng,trendFunc);
			pnl_mat_get_row((*spotsPool)(),path,row);
			fTimeStep = timeStep;
		}

	}
	// */
}

void BlackScholesModel::simulateUnderHistoricalProba(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
	BlackScholesModel::simulate(path, T, t, nbTimeSteps, rng, past, (TrendFunc)&BlackScholesModel::getTrend);
}

void BlackScholesModel::simulateUnderRiskNeutralProba(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng, const PnlMat *past) {
	BlackScholesModel::simulate(path, T, t, nbTimeSteps, rng, past, (TrendFunc)&BlackScholesModel::getRiskNeutralTrend);
}

inline bool BlackScholesModel::isRecognitionDate(double t, double T, int nbTimeSteps) {
    double timestep = ((double)T) / ((double) nbTimeSteps);
    int s = std::max((int) (t/timestep) - 1,0);
    int k = (int) (t/timestep + 0.5);
    for (; s <= k; s++) {
        if (fabs(s*timestep - t) <= FLT_EPSILON)
            return true;
    }
    return false;
}


BlackScholesModel::~BlackScholesModel() {

	pnl_mat_free(&cholesky);
	pnl_vect_free(&volatilities);
	pnl_vect_free(&spots);
	pnl_mat_free(&correlations);
	pnl_vect_free(&trends);
	pnl_vect_free(&dividends);

}

void BlackScholesModel::checkingInput(int dim, double interestRate, PnlVect *trends, PnlVect *dividends,
									  PnlVect *volatilities, PnlVect *spots, PnlMat *correlations)
{
	if (dim <= 0) {
		throw std::invalid_argument("Dimension of the model must be strictly above 0!");
	}

	if (interestRate <= 0.) {
		throw std::invalid_argument("Interest rate in this model has to be strictly positive");
	}

	if (trends == NULL) {
		throw std::invalid_argument("Vector of trends is NULL!");
	}

	if (trends->size != dim) {
		throw std::invalid_argument("Model dimension does not match with the size of the trends vector!");
	}

	if (dividends == NULL) {
		throw std::invalid_argument("Vector of dividends is NULL!");
	}

	if (trends->size != dim) {
		throw std::invalid_argument("Model dimension does not match with the size of the dividends vector!");
	}

	if (volatilities == NULL) {
		throw std::invalid_argument("Vector of volatility is NULL!");
	}

	if (volatilities->size != dim) {
		throw std::invalid_argument("Model dimension does not match with the size of the volatility vector!");
	}

	if (spots == NULL) {
		throw std::invalid_argument("Vector of spots is NULL!");
	}

	if (spots->size != dim) {
		throw std::invalid_argument("Model dimension does not match with the size of the spot vector!");
	}

	if (correlations == NULL) {
		throw std::invalid_argument("Correlation matrix is NULL!");
	}

	if (correlations->m != correlations->n) {
		throw std::invalid_argument("Correlation matrix is not squared!");
	}

	if (correlations->m != dim) {
		throw std::invalid_argument("Model dimension does not match with the size of the correlation matrix!");
	}
}

void BlackScholesModel::simulateDebug(PnlMat *path, double T, double t, int nbTimeSteps, PnlRng *rng,
									  const PnlMat *past, TrendFunc trendFunc)
{
	if (T <= 0.0) {
		throw std::domain_error("Maturity can't be negative!");
	}

	if (nbTimeSteps <= 0.0) {
		throw std::invalid_argument("Number of time steps must be positive!");
	}

	if (rng == NULL) {
		throw std::invalid_argument("PnlRng is NULL!");
	}

	if (path == NULL) {
		throw std::invalid_argument("path is NULL!");
	}

	if (path->m != nbTimeSteps + 1) {
		throw std::invalid_argument("Number of rows of path doesn't match the number of time steps!");
	}

	if (path->n != nbRiskAssets) {
		throw std::invalid_argument("Number of columns of path doesn't match the number of assets!");
	}

	double timeStep = T / ((double)nbTimeSteps);

	PnlVect *tempSpots;

	if (t == 0) {

		tempSpots = pnl_vect_copy(spots);
		pnl_mat_set_row(path, spots, 0);

		for (int t = 1; t <= nbTimeSteps; t++) {
			pnl_mat_get_row(tempSpots, path, t - 1);
			nextIteration(path, tempSpots, t, timeStep, rng, trendFunc);
		}

	}
	else {

		if (past == NULL) {
			throw std::invalid_argument("Can not compute the simulation at t != 0 without data in past matrix!");
		}

		if (t >= T) {
			pnl_mat_set_subblock(path, past, 0, 0);
			return;
		}

		if (past->n != nbRiskAssets) {
			throw std::invalid_argument("Past matrix number of column is incorrect!");
		}

		int row;
		double fTimeStep;

		tempSpots = pnl_vect_create(dim);
		pnl_mat_set_subblock(path, past, 0, 0);

		pnl_mat_get_row(tempSpots, past, past->m - 1);

		if (isRecognitionDate(t, T, nbTimeSteps)) {
			row = past->m;
			fTimeStep = timeStep;

			if (past->m != floor(t / timeStep + 0.5) + 1) {
				throw std::invalid_argument("Past matrix number of row is incorrect!");
			}

		}
		else {
			row = past->m - 1;
			fTimeStep = ((double)row) * timeStep - t;

			int val;

			if (fabs(t / timeStep - floor(t / timeStep)) < 0.00001) {
				val = floor(t / timeStep);
			}
			else {
				val = ceil(t / timeStep);
			}

			if (past->m != val + 1) {
				throw std::invalid_argument("Past matrix number of row is incorrect!");
			}
		}


		for (; row <= nbTimeSteps; row++) {
			nextIteration(path, tempSpots, row, fTimeStep, rng, trendFunc);
			pnl_mat_get_row(tempSpots, path, row);
			fTimeStep = timeStep;
		}

	}
	pnl_vect_free(&tempSpots);
}
