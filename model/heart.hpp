//
//  heart.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef heart_hpp
#define heart_hpp

#include <stdio.h>
#include "submodule.hpp"
#include <math.h>
#include <vector>


class heart: public submodule {
    
public:
    heart(std::vector<std::string> sharedNames, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : submodule("heart", sharedNames, inputNames, algebraicNames, outputNames, parameters) {}
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
};
#endif /* heart_hpp */
