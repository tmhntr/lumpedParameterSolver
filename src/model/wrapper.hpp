//
//  wrapper.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef wrapper_hpp
#define wrapper_hpp

#include <stdio.h>
#include <iostream>
#include "solver/solver.hpp"
#include "model.hpp"
#include "component.hpp"
#include <math.h>
#include <string>
#include <vector>

class wrapper: public model
{
private:
    std::vector<model *> models;
public:
//    std::vector<std::string> stateNames;

    wrapper();
    wrapper(std::vector<model *> mdls);
    std::string getDerivedName(int index);
    double getDerived(int index);
    void addModel(model * mdl);

    model * getModel(int index);
    model * getModel(std::string modelName);
    std::vector<model *> getModelVec();

    std::string * getStateNames();

    void updateDerived(double t, double y[]);

    void getDY(double t, double y[], double * DY);

    int init(model * parent);

    std::vector<component *> flattenModList(std::vector<model *> modlist);

    std::vector<component *> flattenModList();

    std::vector<model *> components();
};


#endif /* wrapper_hpp */
