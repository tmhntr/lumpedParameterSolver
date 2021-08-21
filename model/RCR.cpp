//
//  RCR.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#include "RCR.h"

void RCR::updateAlgebraic(double t, double y[])
{
    // Inputs
    double p_p = y[inputIndex[0]];
    double p_d = y[inputIndex[1]];
    
    // Parameters
    double R = P[0];
    
    double q;
    if (p_p > p_d || !hasValve)
    {
      q = (p_p - p_d) / R;
    }
    else
    {
      q = 0.0;
    }
    
    algebraic[0] = q;
}

void RCR::getDY(double t, double y[], double * DY)
{
    // load algebraic parameters
    double q_in = algebraic[0];
    
    // Constant Parameters
    double C = P[1];
    
    // load shared algebraic parameters
    double q_out = *shared[0]; // shared[0] is outlet flow
    
    DY[0]   = (1.0 / C) * ( q_in - q_out ); // right atrial pressure
}
