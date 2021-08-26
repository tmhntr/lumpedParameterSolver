//
//  interface.h
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#ifndef interface_h
#define interface_h

#include <iostream>
#include <string>
#include <vector>

class model {
private:
    int _neq;
    std::string _name;
public:
    std::string * stateNames;

    model()
    {
        _name = "name";
    }
    model(std::string name) { _name = name; }
    std::string getName() { return _name; }

    virtual void updateAlgebraic(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;
    
    void setNEQ(int neq){ _neq = neq; }
    void setStateName(int index, std::string stateName)
    {
        if (index < getNEQ())
            stateNames[index] = stateName;
        else
        {
            std::cout << index << " outside of stateName array range." << std::endl;
            throw (index);
        }
    }
    
    int getNEQ(){ return _neq; }
    std::string getStateName(int index)
    {
        if (index >= getNEQ())
        {
            std::cout << index << " outside of shared array range." << std::endl;
            throw (index);
        }
        else
            return stateNames[index];
    }

};

class solver {
    model * mdl;
public:
    solver() {}
    solver(model * m): solver() { setModel(m); }
    
    void setModel(model * m)
    {
        mdl = m;
    }
    
    model * getModel() { return mdl; }
    
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
