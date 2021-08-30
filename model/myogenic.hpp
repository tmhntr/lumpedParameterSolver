//
//  myogenic.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#ifndef myogenic_hpp
#define myogenic_hpp

#include <stdio.h>
#include "submodule.hpp"

class myogenic: public submodule {
private:
    int nSegments = 1;
public:
    myogenic(std::string name, std::vector<std::string> sharedNames, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : submodule(name, sharedNames, inputNames, algebraicNames, outputNames, parameters)
    {
//        nSegments = (int) inputNames.size() - 1;
    }
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
    void setSegNum(int value) { nSegments = value; }
};

#endif /* myogenic_hpp */
