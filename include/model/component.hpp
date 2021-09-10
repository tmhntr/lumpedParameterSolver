//
//  component.hpp
//  component
//
//  Created by Tim  Hunter on 2021-09-08.
//

#ifndef component_hpp
#define component_hpp

#include <iostream>
#include <string>
#include <vector>

class component {
public:

    component()
    {
        _name = "name";
    }
    component(std::string name) { _name = name; }

    virtual ~component() {}

    virtual std::vector<component *> components() = 0;
    virtual int init(component * parent) = 0;

    virtual void updateDerived(double t, double y[]) = 0;
    virtual void getDY(double t, double y[], double * DY) = 0;

    virtual double getDerived(int index) = 0;
    virtual std::string getDerivedName(int index) = 0;
    virtual std::vector<std::string> getDerivedNameVec() = 0;

    void setY(double * y) { _y = y; }
    double * getY() { return _y; }
    double getY(int index) { return _y[index]; }

    void setName(std::string name) { _name = name; }
    std::string getName() { return _name; }
    void setNEQ(int value){ _neq = value; }
    int getNEQ(){ return _neq; }
    void setNDerived(int value){ _nDerived = value; }
    int getNDerived(){ return _nDerived; }

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
    int _nDerived;
private:
    int _neq = 0;
    std::string _name;
    double * _y;
};

#endif /* component_hpp */
