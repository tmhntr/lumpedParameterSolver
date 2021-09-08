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
    
    void updateDerived(double t, double y[])
    {
        for (int i = 0; i <  models.size(); i++)
        {
            models[i]->updateDerived(t, y); 
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
    
    int init(solver * slvr)
    {
        slvr->setModel(this);
        return init(components(), stateNames, slvr);
    }
    
    int init(std::vector<model *> modlist, std::vector<std::string> stateVars, solver * slvr)
    {
        int retval = 0;
//        std::vector<component *> flatmodlist = flattenModList(modlist);
        for (std::vector<model *>::iterator it = modlist.begin() ; it != modlist.end(); it++)
        {
            try {
                if ((*it)->init(slvr) != 0)
                    throw 1;
            } catch (int e) {
                std::cout << "Initialization for model (" << (*it)->getName() << ") failed." << std::endl;
                retval++;
            }
        }
        return retval;
    }
    
    std::vector<component *> flattenModList(std::vector<model *> modlist)
    {
        std::vector<component *> newModList;
        wrapper * wrap;
        for (int i = 0; i < modlist.size(); i++)
        {
            
            if (typeid(wrapper) == typeid(*(modlist[i])))
            {
                wrap = (wrapper *) modlist[i];
                std::vector<component *> newNewModList = flattenModList(wrap->getModelVec());
                for (int j = 0; j  < newNewModList.size(); j++)
                {
                    newModList.push_back(newNewModList[j]);
                }
            } else
            {
                newModList.push_back((component *) modlist[i]);
            }
        }
        return newModList;
    }
    
    std::vector<component *> flattenModList() {return flattenModList(models); }
    
    std::vector<model *> components()
    {
        std::vector<model *> mlist;
        std::vector<component *> clist = flattenModList(models);
        for (int i = 0; i < clist.size(); i++) {
            mlist.push_back((model *) clist[i]);
        }
        return mlist;
    }
};


#endif /* wrapper_hpp */
