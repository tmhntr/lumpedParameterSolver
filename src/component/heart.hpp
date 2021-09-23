//
//  heart.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef heart_hpp
#define heart_hpp

#include "component_model.hpp"
#include <math.h>
#include <stdio.h>
#include <vector>

class heart : public component_model {
private:
    int nInlets = 1;
    int nOutlets = 1;
    double DELTAT;

public:
    heart(std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters)
        : component_model("heart", inputNames, algebraicNames, outputNames, parameters)
    {
    }
    // int init(model * parent)
    // {
    //     DELTAT = slvr->getDeltaT();
    //     return component_model::init(parent);
    // }
    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double* DY);
};
#endif /* heart_hpp */
