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
public:
//    std::vector<std::string> stateNames;

    component_wrapper();
    component_wrapper(std::vector<component *> mdls);
    std::string getDerivedName(int index);
    double getDerived(int index);
    void addModel(component * mdl);

    component * getModel(int index);
    component * getModel(std::string modelName);
    std::vector<component *> getModelVec();

    std::string * getStateNames();

    void setY(double * y);

    void updateDerived(double t, double y[]);

    void getDY(double t, double y[], double * DY);

    int init(component * parent);

    std::vector<component_model *> flattenModList(std::vector<component *> modlist);

    std::vector<component_model *> flattenModList();

    std::vector<component *> components();
};


#endif /* component_wrapper_hpp */
