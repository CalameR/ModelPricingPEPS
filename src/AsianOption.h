#include "Product.h"

class AsianOption : public Product {
public:

    /**
     * Constructeur d'Asian Option
     * @param T Maturité de l'option
     * @param size Nombre de sous-jacents
     * @param nbTimeSteps Nombre de pas entre la date de départ et la maturité
     * @param lambda Coefficients des sous-jacents pour le payoff de l'option
     * @param strike Prix d'exercice de l'option
     */
    AsianOption(double maturity,int size,int nbTimeSteps,PnlVect *lambda,double strike);


     /**
      * Payoff de l'Asian Option
      * @param path Matrice représentant les valeurs des sous-jacents entre la date de départ et la maturité pour chaque aléa
      * @return payoff de l'option
      */
    double payoff(const PnlMat *path) const;

    virtual ~AsianOption();

    string getName() const { return "AsianOption"; }

private:
    PnlVect *spot;
    PnlVect *lambda;
    double strike;
};
