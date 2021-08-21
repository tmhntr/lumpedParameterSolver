//
//  interface.h
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#ifndef interface_h
#define interface_h

#include <string>
#include <vector>

class model {
protected:
    int NEQ ;
    std::string name;
public:
    /* This function is the main functionality of the model. It takes in t (time) and y[] as arguments and populates array DY with the dydt values */
    virtual void updateAlgebraic(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;
    int getNEQ() { return NEQ; }
    std::string getName() { return name; }

};

class solver {
public:
    virtual void solveStep(double t) = 0;
};

class printer {
public:
    virtual void print(double t, double * y) = 0;
};
class config {
    double abstol, reltol, t_init, t_stop, print_interval, deltat;
    
};


#endif /* interface_h */
