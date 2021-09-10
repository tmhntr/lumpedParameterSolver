//
//  heart.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef heart_hpp
#define heart_hpp

#include <stdio.h>
#include "component.hpp"
#include <math.h>
#include <vector>


class heart: public component {
private:
    int nInlets = 1;
    int nOutlets = 1;
    double DELTAT;

public:
    heart(std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component("heart", inputNames, algebraicNames, outputNames, parameters) {  }
    // int init(model * parent)
    // {
    //     DELTAT = slvr->getDeltaT();
    //     return component::init(parent);
    // }
    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double * DY);
};
#endif /* heart_hpp */
