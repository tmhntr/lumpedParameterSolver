//
//  component_wrapper.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef component_wrapper_hpp
#define component_wrapper_hpp

#include <stdio.h>
#include <iostream>
#include <model/solver.hpp>
#include <model/component.hpp>
#include "component_model.hpp"
#include <math.h>
#include <string>
#include <vector>

class component_wrapper: public component
{
private:
    std::vector<component *> models;
    int _nDerived = 0;
public:
//    std::vector<std::string> stateNames;

    virtual ~component_wrapper() {}

    double getDerived(int index);
    std::string getDerivedName(int index);
    std::vector<std::string> getDerivedNameVec();
    void addModel(component * mdl);

    component * getModel(int index);
    component * getModel(std::string modelName);
    std::vector<component *> getModelVec();

    std::string * getStateNames();

    void setY(std::vector<double> y);

    void updateDerived(double t, double y[]);

    void getDY(double t, double y[], double * DY);

    int init(component * parent);

    std::vector<component *> flattenModList(std::vector<component *> modlist);
    std::vector<component *> flattenModList();

    std::vector<component *> components();

    component_wrapper() : component("wrapper") {  }
    component_wrapper(std::vector<component *> mdls) : component_wrapper()
    {
        for (std::vector<component *>::iterator it = mdls.begin() ; it != mdls.end(); ++it)
        {
            this->addModel(*it);
        }
    }

};


#endif /* component_wrapper_hpp */
