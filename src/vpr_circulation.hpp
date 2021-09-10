//
//  ratHeart.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#ifndef ratHeart_hpp
#define ratHeart_hpp

#include <stdio.h>
#include "component.hpp"
#include <math.h>
#include <map>
#include <vector>

/*
 This clas implements a submodule for Timothy Hunters simulator application.
 It is a model of rat circulation and baroreflex heavily inspired by the vpr model composite.

 The requirements for this module are following:
 - All shared values are outlet flows
 - All but the last input are upstream pressures
 - last input is this compartments pressure
 - Algebraic values are flows from upstream compartments to this compartment
 */

class vpr_circulation: public component {
private:
    // bool hasValve = false;
    bool hasDialysis = false;

public:
    vpr_circulation(std::string name, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component(name, inputNames, algebraicNames, outputNames, parameters)
    {

    }
    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double * DY);

    void setHasDialysis(bool value) { hasDialysis = value; }
    // void setHasValve(bool value) { hasValve = value; }
};

#endif /* ratHeart_hpp */
