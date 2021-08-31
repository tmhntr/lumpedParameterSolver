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
#include "interface.hpp"
//#include "submodule.hpp"
#include <math.h>
#include <string>
#include <vector>

class wrapper: public model
{
private:
    std::vector<model *> models;
public:
//    std::vector<std::string> stateNames;

    wrapper() : model("wrapper") {}
    wrapper(std::vector<model *> mdls) : wrapper()
    {
        for (std::vector<model *>::iterator it = mdls.begin() ; it != mdls.end(); ++it)
        {
            addModel(*it);
        }
    }
    
    void addModel(model * mdl)
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
    
    model * getModel(int index) { return models[index]; }
    model * getModel(std::string modelName)
    {
        std::vector<model *>::iterator it = models.begin();
        while (it != models.end())
        {
            if (modelName == (*it)->getName())
                return (*it);
        }
        return NULL;
    }
    std::vector<model *> getModelVec() { return models; }
    
    std::string * getStateNames()
    {
        return stateNames.data();
    }
    
//    void updateStateNames()
//    {
//        // reallocate statenames so that it fits all names.
////        if (stateNames)
////            free(stateNames);
////        stateNames = (std::string *) calloc(getNEQ(), sizeof(std::string));
//
//        std::vector<submodule *>::iterator it =  models.begin();
//        int c = 0;
//        while (c < getNEQ() && it != models.end())
//        {
//            for (int i = 0; i < (*it)->getNEQ(); i++)
//            {
//                stateNames.push_back((*it)->getStateName(i));
//                c++;
//            }
//            it++;
//        }
//    }
    
    void updateAlgebraic(double t, double y[])
    {
        for (int i = 0; i <  models.size(); i++)
        {
            models[i]->updateAlgebraic(t, y);
        }
    }
    
    void getDY(double t, double y[], double * DY)
    {
        model * mdlPtr;
        double * DYPtr = DY;
        for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            mdlPtr = *it;
//            std::cout << mdlPtr->getName() << std::endl;
            mdlPtr->getDY(t, y, DYPtr);
            DYPtr+=mdlPtr->getNEQ();
        }
    }
    
    int init()
    {
        return init(models, stateNames);
    }
    
    int init(std::vector<model *> modlist, std::vector<std::string> statevars)
    {
        int retval = 0;
        for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); it++)
        {
            try {
                if ((*it)->init(modlist, statevars) != 0)
                    throw 1;
            } catch (int e) {
                std::cout << "Initialization for model (" << (*it)->getName() << ") failed." << std::endl;
                retval++;
            }
        }
        return retval;
    }
};


#endif /* wrapper_hpp */
