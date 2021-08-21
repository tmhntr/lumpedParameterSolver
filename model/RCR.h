//
//  RCR.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef RCR_h
#define RCR_h

#include <stdio.h>
#include "modelBlock.h"
#include <math.h>
#include <map>
#include <vector>


class RCR: public modelBlock {
protected:
    bool hasValve;
    int nInputs = 2; 
    int nAlgebraic = 1;
    int nP = 2;
public:
    RCR(std::string n, int neq) : modelBlock(n, neq) {}
    RCR(std::string n, int neq, int II[], double p[]) : modelBlock(n, neq, II, p) {}
    
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
};

#endif /* RCR_h */
