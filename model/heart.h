//
//  heart.h
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#ifndef heart_h
#define heart_h

#include <stdio.h>
#include "modelBlock.h"
#include <math.h>
#include <vector>


class heart: public modelBlock {
protected:
    int nInputs = 2;
    int nAlgebraic = 10;
    int nP = 14;
public:
    heart(std::string n, int neq) : modelBlock(n, neq) {}
    heart(std::string n, int neq, int II[], double p[]) : heart(n, neq)
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
    
    void updateAlgebraic(double t, double y[]);
    void getDY(double t, double y[], double * DY);
};
#endif /* heart_h */
