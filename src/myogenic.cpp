//
//  myogenic.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#include "myogenic.hpp"
#include <math.h>

/*
 Shared: segment bulk flow rate (from myoRC)
 Input States: D [nSegments], A [nSegments], p [nSegments]
 Algebraic: T_total [nSegments], A_total [nSegments], Tension [nSegments]
 State variables: Diameter [nSegments], Activation [nSegments]
 
 Parameters (all multiplied by nSegments): C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_rbc_meta, C_ATP_0, C_myo, C_shear, C_symp, C_tone_primeprime, tau_d, tau_a, mu, nParallel
 
 Note that [n] denotes multiple values
 Note that (module) denotes the probably source of such values
 */

void myogenic::updateDerived(double t, double y[])
{

    bool isFlow = true;
    
    // State inputs
    double D, A, p;
    // Parameters
    double C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_rbc_meta, C_ATP_0, C_myo, C_shear, C_symp, C_tone_primeprime;
    
    // intermediate values
    double T, T_pass, T_act_max, T_total, S_loc, S_CR, S_tone, A_total, tau_wall;
    double q, v, mu, nParallel;
    
    
    for (int i = 0; i < nSegments; i++)
    {
//      Inputs:
        D = input(0*nSegments+i); double r = D/2; // units: um
        A = input(1*nSegments+i);
        
        p = input(2*nSegments+i); // units mmhg
        p = p * 1333.22; // convert units from mmHg to dyn / cm^2


//      Parameters:
        C_pass = getP(0*nSegments+i);
        C_pass_prime = getP(1*nSegments+i);
        D_0 = getP(2*nSegments+i);
        C_act = getP(3*nSegments+i);
        C_act_prime = getP(4*nSegments+i);
        C_act_primeprime = getP(5*nSegments+i);
//        C_rbc_meta = getP(6*nSegments+i);
//        C_ATP_0 = getP(7*nSegments+i);
        C_myo = getP(6*nSegments+i);
        C_shear = getP(7*nSegments+i);
        
        C_symp = getP(10*nSegments);
        C_tone_primeprime = getP(10*nSegments+1);
        C_rbc_meta = getP(10*nSegments+2);
        C_ATP_0 = getP(10*nSegments+3);
        
        
        if (isFlow)
        {
            mu = getP(8*nSegments+i); // units: cP
            mu = mu * 1e-3; // units: N * s / m^2
            nParallel = getP(9*nSegments+i);
            q = input(3*nSegments+i) / nParallel;
            v = q / (M_PI * pow(r*1e-4, 2)); // in-line unit conversion of r from um to cm
            //units: cm/s
            tau_wall = (4.0 * mu * q) / (M_PI * pow(r*1e-4, 4)); // in-line unit conversion of r from um to cm
            
        } else
            tau_wall = 55.0;
            
        T = p*(D*1e-4)/2.0; // in-line unit conversion of D from um to cm
        // T units: dynes / cm

//      Equation 2
        T_pass = C_pass * exp(C_pass_prime * (D/D_0 - 1));

//      Equation 3
        T_act_max = M_PI * pow(r*1e-4, 2) * C_act * exp(-pow((D/D_0 - C_act_prime)/C_act_primeprime, 2)); // in-line unit conversion of r from um to cm

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

        setDerived(0*nSegments+i, T_total);
        setDerived(1*nSegments+i, A_total);
        setDerived(2*nSegments+i, T);


    }

}
void myogenic::getDY(double t, double y[], double * DY)
{
//      Equation 6
    for (int i = 0; i < nSegments; i++)
    {
        // Parameters
//        double D_c = getP(0*nSegments+i);
        double tau_d = getP(10*nSegments+4);
        double tau_a = getP(10*nSegments+5);
        
        // State Inputs
//        double D = input(0*nSegments+i);
        double A = input(1*nSegments+i);
        double p = input(2*nSegments+i);// units mmhg
        p = p * 1333.22; // convert units from mmHg to dyn / cm^2
        
        
        // Algebraic inputs
        double T_total = getDerived(0*nSegments+i);
        double A_total = getDerived(1*nSegments+i);
        double T = getDerived(2*nSegments+i);
                
        

        DY[0+i] = (1.0/tau_d)*(2.0/p)*(T - T_total);

    //  Equation 7
        DY[nSegments+i] = (1.0/tau_a)*(A_total - A);
    }
}

