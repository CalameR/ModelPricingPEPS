//
// Created by courtocy on 1/26/17.
//

#include <iostream>
#include <cmath>
#include <cfloat>

int main() {

    std::cout << "Hello World!" << std::endl;

    double T = 1. + DBL_EPSILON;
    int nbTimeSteps = 5;
    int nbHedgingSteps = nbTimeSteps * 10;
    double timestep = T/nbTimeSteps;
    double htimestep = T/nbHedgingSteps;


    for (int h = 0; h <= nbHedgingSteps; h++) {
        double t = (double)(h) * htimestep;
        std::cout << h << '\t' << (int) std::ceil((t+DBL_EPSILON)/timestep) << std::endl;
    }
}