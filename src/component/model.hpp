//
//  model.hpp
//  model
//
//  Created by Tim  Hunter on 2021-09-08.
//

#ifndef model_hpp
#define model_hpp

#include <iostream>
#include <string>
#include <vector>

class model {
public:

    model()
    {
        _name = "name";
    }
    model(std::string name) { _name = name; }

    virtual ~model() {}

    virtual std::vector<model *> components() = 0;
    virtual int init(model * parent) = 0;

    virtual void updateDerived(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;

    virtual double getDerived(int index) = 0;
    virtual std::string getDerivedName(int index) = 0;

    void setY(double * y) { _y = y; }
    double * getY() { return _y; }
    double getY(int index) { return _y[index]; }

    void setName(std::string name) { _name = name; }
    std::string getName() { return _name; }
    void setNEQ(int neq){ _neq = neq; }
    int getNEQ(){ return _neq; }

    void setStateName(int index, std::string stateName)
    {
        if (index < getNEQ())
            _stateNames[index] = stateName;
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
            return _stateNames[index];
    }
protected:
    std::vector<std::string> _stateNames;
private:
    int _neq = 0;
    std::string _name;
    double * _y;


    int _nDerived;
};

#endif /* model_hpp */
