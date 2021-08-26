//
//  sdsolver.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#include "sdsolver.hpp"

sdsolver::sdsolver(model * m) : solver(m)
{
    DY = (double *) calloc(getModel()->getNEQ(), sizeof(double));
    y = (double *) calloc(getModel()->getNEQ(), sizeof(double));
}


