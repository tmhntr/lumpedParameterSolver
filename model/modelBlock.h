//
//  modelBlock.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef modelBlock_h
#define modelBlock_h

#include <stdio.h>
#include "interface.h"
#include <math.h>
#include <string>
#include <map>
#include <vector>

class modelBlock: public model {
protected:
    int * inputIndex;
    std::vector<double> P;
    std::vector<double *> shared;
    int nInputs = 32;
    int nAlgebraic = 32;
    int nP;
public:
    double * algebraic;
    
    
    modelBlock(std::string n, int neq)
    {
        setName(n);
        NEQ = neq;
        inputIndex = (int *) calloc(nInputs, sizeof(int));
        algebraic = (double *) calloc(nAlgebraic, sizeof(int));
        P.assign(nP, 0.0);
    //    setDY((double *) NULL);
    }
    
    modelBlock(std::string n, int neq, int II[], double p[]) : modelBlock(n, neq)
    {
        int * IIptr = II;
        for (int i = 0; i < nInputs; i++)
        {
            setII(i, *IIptr);
            IIptr++;
        }
//        std::vector<double> parameters = p;
//        P = p;
        for (int i = 0; i < nP; i++)
        {
            P[i]=p[i];
        }
    }
    
    void setName(std::string name1)
    {
        name = name1;
    }
    void setII(int index, int value)
    {
        inputIndex[index] = value;
    }
    void addP(double param)
    {
        P.push_back(param);
    }
    void addShared(double * ptr)
    {
        shared.push_back(ptr);
    }
    
    double * getAlgebraicPtr(int index)
    {
        return (&algebraic[index]);
    }
    
};

#endif /* modelBlock_h */
