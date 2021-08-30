//
//  RCR.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef myoRC_hpp
#define myoRC_hpp

#include <stdio.h>
#include "submodule.hpp"
#include <math.h>
#include <map>
#include <vector>

/*
 This class implements a submodule for Timothy Hunters simulator application.
 It is a multiple rc compartment dependent on the myogenic module to calculate resistance and capacitance, with 1 inlet and outlet.
 
 The requirements for this module are following:
 - All shared values are outlet flows
 - All but the last input are upstream pressures
 - last input is this compartments pressure
 - Algebraic values are flows from upstream compartments to this compartment
 */

class myoRC: public submodule {
private:
    bool hasValve = false;
    int nInlets = 1;
    int nOutlets = 1;
    int nSegments = 1;
//    int nInputs = 2; 
//    int nAlgebraic = 1;
//    int nP = 2;
public:
    myoRC(std::string name, std::vector<std::string> sharedNames, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : submodule(name, sharedNames, inputNames, algebraicNames, outputNames, parameters)
    {
        nInlets = (int) inputNames.size() - 1;
        nOutlets = (int) sharedNames.size();
    }
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
    void setHasValve(bool value) { hasValve = value; }
};

#endif /* myoRC_hpp */
