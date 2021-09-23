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

void RCR::updateDerived(double t, double y[])
{
    // Inputs
    double p = input(nInlets);

    // This for loop calculates inlet q values for the number of nInlets specified
    for (int i = 0; i < nInlets; i++) {
        double p_up = input(i);

        double R = getP(i);

        double q;
        if (p_up > p || !hasValve) {
            q = (p_up - p) / R;
        } else {
            q = 0.0;
        }
        setDerived(i, q);
        //        algebraic[i] = q;
    }
}

void RCR::getDY(double t, double y[], double* DY)
{
    // load algebraic parameters
    double q_in = 0;
    for (int i = 0; i < nInlets; i++)
        q_in += input(1 * nInlets + 1 + i);

    // Constant Parameters
    double C = getP(nInlets);

    // load shared algebraic parameters
    //    double q_out = *shared[0]; // shared[0] is outlet flow
    double q_out = 0;
    for (int i = 0; i < nOutlets; i++)
        q_out += input(2 * nInlets + 1 + i);

    DY[0] = (1.0 / C) * (q_in - q_out); // right atrial pressure
}
