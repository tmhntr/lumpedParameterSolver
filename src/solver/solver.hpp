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
#include "model/model.hpp"
#include "printer/printer.hpp"

class solver {
    model * mdl;
    printer * _printer;
    double _deltat;
public:
    solver() {}
    solver(model * m): solver() { setModel(m); }
    virtual ~solver() {}

    void setModel(model * m) { mdl = m; }
    model * getModel() { return mdl; }

    void setDeltaT(double deltat) { _deltat = deltat; }
    double getDeltaT() { return _deltat; }

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

};



#endif /* solver_hpp */
