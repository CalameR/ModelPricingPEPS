//
// Created by courtocy on 1/26/17.
//

#include <iostream>
#include <cfloat>
//#include <tgmath.h>
#include <cmath>

inline bool isRecognitionDate(double t, double T, int nbTimeSteps) {
    double timestep = ((double)T) / ((double) nbTimeSteps);
    return (fabs(fmod(t,timestep)) < DBL_EPSILON);
}

int main() {
//#define P
    double T = 8.;
    int nbTimeSteps = 16;
    int nbHedgingSteps = nbTimeSteps * 1000;
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
        if (isRecognitionDate(t + DBL_EPSILON,T,nbTimeSteps)) {
            err3++;
#ifdef P

            std::cout << "3 :" << "\t" << t * timestep << '\t' << t << std::endl;
#endif
        }
    }

    std::cout << "ERR3 : " << err3 << std::endl;

}