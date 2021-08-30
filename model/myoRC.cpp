//
//  RCR.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#include "myoRC.hpp"

/*
The following is a basic implementation of an rcr windkessel compartment. By convention the shared input is flow out, and state inputs are upstream and present pressure. 
 */

/*
 Shared: outlet flow rate (from RCR)
 Input States: Upstream pressure (from other RCR), compartment pressure [nSegments], vessel diameter (from myogenic)
 Algebraic: outlet flow rate [nSegments]
 State variables: compartment pressure [nSegments]
 
 Note that [n] denotes multiple values
 Note that (module) denotes the probably source of such values
 */

void myoRC::updateAlgebraic(double t, double y[])
{
    // State inputs
    double p_up, p, D;
    
    // Single Parameters
    double mu;
    mu = getP(3*nSegments+0);
    
    // Multi parameters
    double length, nParallel;
    
    // intermediate values
    double r, R, q;
    
    for (int i = 0; i < nSegments; i++)
    {
        length = getP(0*nSegments+i);
        nParallel = getP(1*nSegments+i);
        
        // Inputs
        p_up = y[getInputIndex(0*nSegments+i)];
        p = y[getInputIndex(0*nSegments+i+1)];
        
        D = y[getInputIndex(1*nSegments+i+1)]; r = D/2.0;
        
        R = (8.0*mu*length)/(M_PI*pow(r, 4)); // units: Pa * s * m^-3
        
        R = R/nParallel;
        
        R = R/(133.32/1e-6);
        
        q = (p_up - p) / R;
    
        setAlgebraic(i, q);
    }

}

void myoRC::getDY(double t, double y[], double * DY)
{
    // State inputs
    double D, r;
    
    // Algebraic terms
    double q_in, q_out;
    
    // Multi parameters
    double length, nParallel, E, h;
    
    //mIntermediate values
    double C;
    
    for (int i = 0; i < nSegments; i++)
    {
        // State inputs
        D = y[getInputIndex(1*nSegments+i+1)]; r = D/2.0;
        
        // Parameters
        length = getP(0*nSegments+i);
        nParallel = getP(1*nSegments+i);
        E = getP(3*nSegments+i);
        h = getP(4*nSegments+i);
        
        // Algebraic terms
        q_in = getAlgebraic(i);
        if (i < nSegments - 1)
        {
            q_out = getAlgebraic(i+1);
        } else
            q_out = shared(0);
        
        C = (2*M_PI*r*length)/(E*h);
        C = C*nParallel;
        C = C/(1e-6/133.32);
        
        DY[i]   = (1.0 / C) * ( q_in - q_out );

    }
    
    
}
