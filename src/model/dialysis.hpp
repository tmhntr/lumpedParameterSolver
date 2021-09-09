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
    dialysis(std::vector<std::string> inputNames, std::vector<std::string> derivedNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component("dialysis", inputNames, derivedNames, outputNames, parameters) {  }

    void updateDerived(double t, double y[]);
    void getDY(double t, double y[], double * DY);

    void setNCompartments(int num) { nCompartments = num; }

private:
    int nCompartments;
};


#endif /* dialysis_hpp */
