//
//  RCR.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef RCR_hpp
#define RCR_hpp

#include <stdio.h>
#include "submodule.hpp"
#include <math.h>
#include <map>
#include <vector>


class RCR: public submodule {
private:
    bool hasValve = false;
    int nInlets = 1;
    int nOutlets = 1;
//    int nInputs = 2; 
//    int nAlgebraic = 1;
//    int nP = 2;
public:
    RCR(std::string name, std::vector<std::string> sharedNames, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : submodule(name, sharedNames, inputNames, algebraicNames, outputNames, parameters) {}
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
};

#endif /* RCR_hpp */
