//
//  myogenic.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#include "myogenic.hpp"
#include <math.h>

/*
 Shared: outlet flow rate (from RCR)
 Input States: Upstream pressure (from other RCR), compartment pressure [nSegments], vessel diameter (from myogenic)
 Algebraic: outlet flow rate [nSegments]
 State variables: compartment pressure [nSegments]
 
 Note that [n] denotes multiple values
 Note that (module) denotes the probably source of such values
 */

void myogenic::updateAlgebraic(double t, double y[])
{

    bool isFlow = true;
    
    // State inputs
    double D, A, p;
    // Parameters
    double C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_rbc_meta, C_ATP_0, C_myo, C_shear, C_symp, C_tone_primeprime, tau_wall;
    
    // intermediate values
    double T, T_pass, T_act_max, T_total, S_loc, S_CR, S_tone, A_total;
    double q, mu, nParallel;
    
    
    for (int i = 0; i < nSegments; i++)
    {
//      Inputs:
        D = y[getInputIndex(0*nSegments+i)]; double r = D/2;
        A = y[getInputIndex(1*nSegments+i)];
        
        p = y[getInputIndex(2*nSegments+i)];


//      Parameters:
        C_pass = getP(0*nSegments+i);
        C_pass_prime = getP(1*nSegments+i);
        D_0 = getP(2*nSegments+i);
        C_act = getP(3*nSegments+i);
        C_act_prime = getP(4*nSegments+i);
        C_act_primeprime = getP(5*nSegments+i);
        C_rbc_meta = getP(6*nSegments+i);
        C_ATP_0 = getP(7*nSegments+i);
        C_myo = getP(8*nSegments+i);
        C_shear = getP(9*nSegments+i);
        C_symp = getP(10*nSegments+i);
        C_tone_primeprime = getP(11*nSegments+i);
        
        
        
        if (isFlow)
        {
            mu = getP(14*nSegments+i);
            nParallel = getP(15*nSegments+i);
            q = shared(i) / nParallel;
            tau_wall = (4.0 * mu * q) / (M_PI * pow(r, 4));
            
        } else
            tau_wall = 55.0;
            
        T = p*D/2.0;

//      Equation 2
        T_pass = C_pass * exp(C_pass_prime * (D/D_0 - 1));

//      Equation 3
        T_act_max = M_PI * pow(r, 2) * C_act * exp(-pow((D/D_0 - C_act_prime)/C_act_primeprime, 2));

//      Equation 1
        T_total = T_pass + A * T_act_max;

//      Equation 8
        S_loc = C_rbc_meta * C_ATP_0;
        
//      Equation 9
//        x = 0;
//        xend = sum(P.L(segment_all:end));
//        fun = @(y) exp(-(y - x)/P.L_met)*S_loc;
//        S_CR = integral(fun, x, xend);
        S_CR = 1.0;

//      Equation 5
        S_tone = C_myo * T - C_shear * tau_wall + C_symp - S_CR + C_tone_primeprime;

//      Equation 4
        A_total = 1/(1 + exp(-S_tone));

        setAlgebraic(0+i, T_total);
        setAlgebraic(nSegments+i, A_total);

    }

}
void myogenic::getDY(double t, double y[], double * DY)
{
//      Equation 6
    for (int i = 0; i < nSegments; i++)
    {
        // Parameters
//        double D_c = getP(0*nSegments+i);
        double tau_d = getP(12*nSegments+i);
        double tau_a = getP(13*nSegments+i);
        
        // State Inputs
        double D = y[getInputIndex(0*nSegments+i)];
        double A = y[getInputIndex(1*nSegments+i)];
        double p = y[getInputIndex(2*nSegments+i)];
        
        
        // Algebraic inputs
        double T_total = getAlgebraic(0*nSegments+i);
        double A_total = getAlgebraic(1*nSegments+i);
        
        
//        double T = p*D/2.0;
        
        

        DY[0+i] = (1/tau_d)*(2.0/p)*(p*D/2.0 - T_total);

    //  Equation 7
        DY[nSegments+i] = (1/tau_a)*(A_total - A);
    }
}

