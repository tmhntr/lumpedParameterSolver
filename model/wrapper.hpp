//
//  wrapper.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef wrapper_hpp
#define wrapper_hpp

#include <stdio.h>
#include <iostream>
#include "interface.h"
#include "submodule.h"
#include <math.h>
#include <string>
#include <vector>

class wrapper: public model
{
private:
    std::vector<submodule *> models;
public:
//    std::vector<std::string> stateNames;

    wrapper() : model("wrapper") {}
    wrapper(std::vector<submodule *> mdls) : wrapper()
    {
        for (std::vector<submodule *>::iterator it = mdls.begin() ; it != mdls.end(); ++it)
        {
            addModel(*it);
        }
    }
    
    void addModel(submodule * mdl)
    {
        models.push_back(mdl);
        int oldNeq = getNEQ();
        setNEQ(oldNeq + mdl->getNEQ());
        stateNames.resize(getNEQ());
        for (int i = 0; i < mdl->getNEQ(); i++)
        {
            setStateName(i+oldNeq, mdl->getStateName(i));
        }
    }
    
    submodule * getModel(int index) { return models[index]; }
    submodule * getModel(std::string modelName)
    {
        std::vector<submodule *>::iterator it = models.begin();
        while (it != models.end())
        {
            if (modelName == (*it)->getName())
                return (*it);
        }
        return NULL;
    }
    std::vector<submodule *> getModelList() { return models; }
    
    std::string * getStateNames()
    {
        return stateNames.data();
    }
    
    void updateStateNames()
    {
        // reallocate statenames so that it fits all names.
//        if (stateNames)
//            free(stateNames);
//        stateNames = (std::string *) calloc(getNEQ(), sizeof(std::string));
        
        std::vector<submodule *>::iterator it =  models.begin();
        int c = 0;
        while (c < getNEQ() && it != models.end())
        {
            for (int i = 0; i < (*it)->getNEQ(); i++)
            {
                stateNames.push_back((*it)->getStateName(i));
                c++;
            }
            it++;
        }
    }
    
    void updateAlgebraic(double t, double y[])
    {
        for (std::vector<submodule *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            (*it)->updateAlgebraic(t, y);
        }
    }
    
    void getDY(double t, double y[], double * DY)
    {
        model * mdlPtr;
        double * DYPtr = DY;
        for (std::vector<submodule *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            mdlPtr = *it;
//            std::cout << mdlPtr->getName() << std::endl;
            mdlPtr->getDY(t, y, DYPtr);
            DYPtr+=mdlPtr->getNEQ();
        }
    }
};


#endif /* wrapper_hpp */
