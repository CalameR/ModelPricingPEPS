#include "Product.h"

class PerformanceOption : public Product {
public:

    /**
     * Constructeur du Performance Option
     * @param T Maturité de l'option
     * @param size Nombre de sous-jacents
     * @param nbTimeSteps Nombre de pas entre la date de départ et la maturité
     * @param lambda Coefficients des sous-jacents pour le payoff de l'option
     * @param strike Prix d'exercice de l'option
     */
    PerformanceOption(double maturity, int size, int nbTimeSteps, PnlVect *lambda);

    /**
     * Payoff du Performance Option
     * @param path Matrice représentant les valeurs des sous-jacents entre la date de départ et la maturité pour chaque aléa
     * @return payoff de l'option
     */
    double payoff(const PnlMat *path, PnlVect *spot) const;

    virtual ~PerformanceOption();

private:
    PnlVect *lambda;
};
