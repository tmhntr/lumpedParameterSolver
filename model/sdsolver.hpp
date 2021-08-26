//
//  sdsolver.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#ifndef sdsolver_hpp
#define sdsolver_hpp

#include <stdio.h>
#include "interface.hpp"

class sdsolver: public solver {
    double * DY;
    double * y;
    double _deltat = 0.01;
public:
//    sdsolver() : solver() {}
    sdsolver(model * m);
    
    void setDeltaT(double deltat) { _deltat = deltat; }
    double getDeltaT() { return _deltat; }
    
    virtual void solveStep(double t) = 0;
    
};

#endif /* sdsolver_hpp */
