//
// Created by courtocy on 1/26/17.
//

#include <iostream>
#include <cfloat>
//#include <tgmath.h>
#include <cmath>

inline bool isRecognitionDate(double t, double T, int nbTimeSteps) {
    double timestep = ((double)T) / ((double) nbTimeSteps);
    int s = std::max((int) (t/timestep) - 1,0);
    int k = (int) (t/timestep + 0.5) + 1;
    for (; s <= k; s++) {
        if (fabs(s*timestep - t) <= 1e4 * DBL_EPSILON)
            return true;
    }
    return false;
}

int main() {
//#define P
    std::cout << FLT_EPSILON/DBL_EPSILON << std::endl;
    double T = 1.;
    int nbTimeSteps = 100;
    int nbHedgingSteps = nbTimeSteps*99999;
    double timestep = T/(double)(nbTimeSteps);
    double htimestep = T/(double)(nbHedgingSteps);

    int err1 = 0;
    int err2 = 0;
    for (int h = 0; h <= nbHedgingSteps; h++) {
        double t = (double)(h) * htimestep;
        if ((h % (nbHedgingSteps/nbTimeSteps) == 0)) {
            if (!isRecognitionDate(t,T,nbTimeSteps)) {
                err1++;
#ifdef P
                std::cout << "1 :" << "\t" << h * htimestep << '\t' << t << std::endl;
#endif
            }
        } else {
            if (isRecognitionDate(t,T,nbTimeSteps)) {
                err2++;
#ifdef P

                std::cout << "2 :" << "\t" << h * htimestep << '\t' << t << std::endl;
#endif
            }
        }
    }
    std::cout << "ERR1 : " << err1 << std::endl;
    std::cout << "ERR2 : " << err2 << std::endl;

    int err3 = 0;
    for (int i = 0; i <= nbTimeSteps ; i++) {
        double t = (double)(i) * timestep;
        if (!isRecognitionDate(t,T,nbTimeSteps)) {
            err3++;
#ifdef P

            std::cout << "3 :" << "\t" << t * timestep << '\t' << t << std::endl;
#endif
        }
    }

    std::cout << "ERR3 : " << err3 << std::endl;


    int err4 = 0;
    int err5 = 0;
    for (int i = 0; i <= nbTimeSteps; i++) {
        for (int j = 0; j < nbHedgingSteps/nbTimeSteps; j++) {
            double t = (double)(i) * timestep + (double)(j)*htimestep;
            int row = std::ceil(t/timestep);
            if ((fabs(t/timestep - std::ceil(t/timestep)) <= FLT_EPSILON)
                and (t/timestep <= std::ceil(t/timestep))){
                row++;
            }
            if (row - i != 1)
                err4++;
        }

    }
    std::cout << "ERR4 : " << err4 << std::endl;

}