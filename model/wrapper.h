//
//  wrapper.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef wrapper_h
#define wrapper_h

#include <stdio.h>
#include <iostream>
#include "interface.h"
#include <math.h>
#include <string>
#include <vector>

class wrapper: public model
{
private:
    std::vector<model *> models;
public:
    std::string * stateNames;

    
    void addModel(model * mdl)
    {
        models.push_back(mdl);
        setNEQ(getNEQ() + mdl->getNEQ());
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
    
    void updateStateNames()
    {
        // reallocate statenames so that it fits all names.
        if (stateNames != NULL)
            free(stateNames);
        stateNames = (std::string *) calloc(getNEQ(), sizeof(std::string));
        
        std::vector<model *>::iterator it =  models.begin();
        int c = 0;
        while (c < getNEQ() && it != models.end())
        {
            for (int i = 0; i < (*it)->getNEQ(); i++)
            {
                stateNames[c] = (*it)->getStateName(i);
                c++;
            }
            it++;
        }
    }
    
    void updateAlgebraic(double t, double y[])
    {
        for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            (*it)->updateAlgebraic(t, y);
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
};


#endif /* wrapper_h */
