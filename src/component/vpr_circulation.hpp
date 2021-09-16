//
//  ratHeart.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#ifndef ratHeart_hpp
#define ratHeart_hpp

#include <stdio.h>
#include "component_model.hpp"
#include <math.h>
#include <map>
#include <vector>

/*
 This clas implements a submodule for Timothy Hunters simulator application.
 It is a simple RCR compartment with 1 or multiple inlets and outlets.

 The requirements for this module are following:
 - All shared values are outlet flows
 - All but the last input are upstream pressures
 - last input is this compartments pressure
 - Algebraic values are flows from upstream compartments to this compartment
 */

class vpr_circulation: public component_model {
public:
    vpr_circulation(std::string name, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component_model(name, inputNames, algebraicNames, outputNames, parameters)
    {

    }
    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double * DY);

    void setHasDialysis(bool value) { hasDialysis = value; }
    
private:
    bool hasDialysis = false;
};

#endif /* ratHeart_hpp */
