//
//  solver.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#ifndef solver_hpp
#define solver_hpp

#include <iostream>
#include <string>
#include <vector>
#include "component.hpp"
#include "printer.hpp"

class solver {
public:
    solver() {}
    solver(component * m): solver() { setModel(m); }
    virtual ~solver() {}

    void setModel(component * m) { mdl = m; }
    component * getModel() { return mdl; }

    void setDeltaT(double *deltat) { _deltat = deltat; }
    double getDeltaT() { return *_deltat; }

    void setPrinter(printer * p)
    {
        _printer = p;
        _printer->setSolver(this);
    }
    printer * getPrinter() { return _printer; }

    virtual double getY(int index) = 0;
    virtual double getT() = 0;

    void print() { _printer->print(); }

    virtual void solveStep(double t) = 0;
protected:
    component * mdl;
    printer * _printer;
    double * _deltat;
};



#endif /* solver_hpp */
