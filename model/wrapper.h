//
//  wrapper.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef wrapper_h
#define wrapper_h

#include <stdio.h>
#include "interface.h"
#include <math.h>
#include <string>
#include <vector>

class wrapper: public model
{
private:
    std::vector<model *> models;
public:
    void addModel(model * mdl)
    {
        models.push_back(mdl);
        NEQ += mdl->getNEQ();
    }
    void updateAlgebraic(double t, double y[])
    {
        model * mdlPtr;
        for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            mdlPtr = *it;
            mdlPtr->updateAlgebraic(t, y);
        }
    }
    void getDY(double t, double y[], double * DY)
    {
        model * mdlPtr;
        double * DYPtr = DY;
        for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); ++it)
        {
            mdlPtr = *it;
            std::cout<<"solving: "<<mdlPtr->getName()<<std::endl;
            mdlPtr->getDY(t, y, DYPtr);
            DYPtr+=mdlPtr->getNEQ();
        }
    }
};


#endif /* wrapper_h */
