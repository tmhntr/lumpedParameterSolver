//
//  dialysis.hpp
//  model
//
//  Created by Tim  Hunter on 2021-09-03.
//

#ifndef dialysis_hpp
#define dialysis_hpp

#include <stdio.h>
#include "component.hpp"

class dialysis : public component {
public:
    dialysis(std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component("heart", inputNames, algebraicNames, outputNames, parameters) {  }
//    int init(std::vector<model *> modlist, std::vector<std::string> stateVars, solver * slvr)
//    {
//        DELTAT = slvr->getDeltaT();
//        return component::init(modlist, stateVars, slvr);
//    }
    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double * DY);
}

#endif /* dialysis_hpp */