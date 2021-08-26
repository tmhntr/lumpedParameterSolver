//
//  RCR.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#include "RCR.hpp"

/*
The following is a basic implementation of an rcr windkessel compartment. By convention the shared input is flow out, and state inputs are upstream and present pressure. 
 */

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
    double q_in = 0;
    for (int i = 0; i < nInlets; i++)
        q_in += algebraic[i];
    
    // Constant Parameters
    double C = P[1];
    
    // load shared algebraic parameters
//    double q_out = *shared[0]; // shared[0] is outlet flow
    double q_out = 0;
    for (int i = 0; i < nOutlets; i++)
        q_out += getSharedVal(i);
    
    DY[0]   = (1.0 / C) * ( q_in - q_out ); // right atrial pressure
}
