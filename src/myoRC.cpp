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
 
 Parameters: length [nSegments], nparallel [nSegments], mu [nSegments],  E 
 
 Note that [n] denotes multiple values
 Note that (module) denotes the probably source of such values
 */

void myoRC::updateDerived(double t, double y[])
{
    // State inputs
    double p_up, p, D;
    
    // Single Parameters
    double mu;
    
    
    // Multi parameters
    double length, nParallel;
    
    // intermediate values
    double r, R, q;
    
    for (int i = 0; i < nSegments; i++)
    {
        length = getP(0*nSegments+i); // units (cm)
        nParallel = getP(1*nSegments+i);
        mu = getP(2*nSegments); // units: (cP)
        
        length = length * 1e-2; // convert units from (cm) to (m)
        mu = mu * 1e-3; // convert units from units from (cP) to  (N * s / m^2)
        
        // Inputs
        p_up = input(0*nSegments+i);
        p = input(0*nSegments+i+1);
        
        D = input(1*nSegments+i+1);
        D = D*1e-6; // from um to m
        r = D/2.0;
        
        R = (8.0*mu*length)/(M_PI*pow(r, 4)); // units: Pa * s * m^-3
        R = R*(1e-6/133.32); // from Pa * s * m^-3 to mmhg-s/ml

        R = R/nParallel;
        
        
        q = (p_up - p) / R;
    
        setDerived(i, q);
    }

}

void myoRC::getDY(double t, double y[], double * DY)
{
    // State inputs
    double D, r;
    
    // Algebraic terms
    double q_in, q_out;
    
    // Multi parameters
    double length, nParallel, E;
    
    //mIntermediate values
    double C;
    
    for (int i = 0; i < nSegments; i++)
    {
        // State inputs
        D = input(1*nSegments+i+1); // units: um
        r = D/2.0; // units: um
        r = r * 1e-6; // unit conversion from um to m
        
        // Parameters
        length = getP(0*nSegments+i); // units (cm)
        nParallel = getP(1*nSegments+i);
        E = getP(3*nSegments);
//        h = getP(4*nSegments+i);
        
        length = length * 1e-2; // convert units from (cm) to (m)

        // Algebraic terms
        q_in = input(2*nSegments+i+1);
        q_out = input(2*nSegments+i+2);
        
        C = (2*M_PI*pow(r, 2)*length)/(E*(0.0829*r + 0.0405*(1e-6))); // in-line conversion  (0.0405) from um to m
        C = C*nParallel;
        C = C*(133.32/1e-6);
        
        DY[i]   = (1.0 / C) * ( q_in - q_out );

    }
    
    
}
