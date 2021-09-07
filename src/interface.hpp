//
//  interface.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#ifndef interface_hpp
#define interface_hpp

#include <iostream>
#include <string>
#include <vector>

class solver;

class model {
private:
    int _neq = 0;
    std::string _name;
public:
    std::vector<std::string> stateNames;

    model()
    {
        _name = "name";
    }
    model(std::string name) { _name = name; }
    
    virtual ~model() {}
    
    virtual std::vector<model *> components() = 0;
    
    std::string getName() { return _name; }
    virtual void updateDerived(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;
    virtual int init(solver * solver) = 0;
    
    
    void setNEQ(int neq){ _neq = neq; }
    int getNEQ(){ return _neq; }

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

class printer {
public:
    printer() {}
    virtual ~printer() {}
    virtual void print() = 0;
    virtual void setSolver(void * slvr) = 0;
};

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


class config {
    double abstol, reltol, t_init, t_stop, print_interval, deltat;
    
};


#endif /* interface_hpp */
