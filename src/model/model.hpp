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

    void setName(std::string name) { _name = name; }
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

#endif /* model_hpp */
