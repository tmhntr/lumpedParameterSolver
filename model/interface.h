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
private:
    std::string _name;
public:
    model() { _name = "name";}
    model(std::string name) { _name = name; }
    std::string getName() { return _name; }

    virtual void updateAlgebraic(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;

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
