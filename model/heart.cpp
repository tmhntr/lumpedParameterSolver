//
//  heart.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

#include "heart.h"

void heart::updateAlgebraic(double t, double y[])
{
    // now load parameters in
    double RR = 1.0;
    double Esys_lv = 1.0;
    double Edias_lv = 1.0;
    double Esys_rv = 1.0;
    double Edias_rv = 1.0;
    double Esys_la = 1.0;
    double Edias_la = 1.0;
    double Esys_ra = 1.0;
    double Edias_ra = 1.0;

    double R_ri = 1.0;
    double R_rav = 1.0;
    double R_li = 1.0;
    double R_lav = 1.0;

    // Inputs
    double p_ra = y[inputIndex[4]];
    double p_rv = y[inputIndex[5]];
    double p_la = y[inputIndex[6]];
    double p_lv = y[inputIndex[7]];
    
    double p_ri = y[inputIndex[8]];
    double p_li = y[inputIndex[9]]; // pressure at left inlet

    double  loc_t = fmod(t,RR);

    double  Ts     = 0.37 * sqrt(RR)     ;
    double  Tasys  = 0.25 * sqrt(RR)  ;
    double  Tav    = 0.19 * sqrt(RR)    ;

    double act_fxn_v, act_fxn_a;

    // ventricular activation function
    if (loc_t > Tav && loc_t <= Tav + Ts)
    {
        act_fxn_v   = (1.0 - cos(M_PI*(loc_t-Tav)/Ts));
    }
    else if (loc_t > Tav + Ts && loc_t <=Tav + 1.5*Ts)
    {
        act_fxn_v   = (1.0 + cos(2.0*M_PI*(loc_t-Tav-Ts)/Ts));
    }
    else
    {
        act_fxn_v   = 0.0;
    }

    // atrial activation function
    if ((0.0 < loc_t) && (loc_t<=Tasys))
    {
        act_fxn_a   = (1.0 - cos(M_PI*(loc_t/Tasys)));
    }
    else if ((Tasys <= loc_t) && (loc_t < 1.5*Tasys))
    {
        act_fxn_a   = (1.0 + cos(2.0 * M_PI * (loc_t - Tasys)/Tasys));
    }
    else
    {
        act_fxn_a   = 0.0;
    }
    
    // ventricular Elastance
    double Elv, Erv, Clv, Crv;
    Elv  = Edias_lv + 0.5 * (Esys_lv - Edias_lv) * act_fxn_v;
    Erv  = Edias_rv + 0.5 * (Esys_rv - Edias_rv) * act_fxn_v;

    // ventricular compliances
    Clv  = 1.0 / Elv;
    Crv  = 1.0 / Erv;


    // Atrial Elastance
    double Ela, Era, Cla, Cra;
    Ela   = Edias_la + 0.5 * (Esys_la-Edias_la) * act_fxn_a;
    Era   = Edias_ra + 0.5 * (Esys_ra-Edias_ra) * act_fxn_a;

    // Atrial compliances
    Cla   = 1.0 / Ela;
    Cra   = 1.0 / Era;
    
    // flows and pressures
    // right inlet flow
    double q_ri, q_rav, q_li, q_lav;
    if (p_ri > p_ra)
    {
      q_ri = (p_ri - p_ra) / R_ri;
    }
    else
    {
      q_ri = 0.0;
    }
    
    // right atrioventricular flow
    if (p_ra > p_rv)
    {
      q_rav = (p_ra - p_rv) / R_rav;
    }
    else
    {
      q_rav = 0.0;
    }

    // left inlet flow
    if (p_li > p_la)
    {
      q_li = (p_li - p_la) / R_li;
    }
    else
    {
      q_li = 0.0;
    }

    // left atrioventricular flow
    if (p_la > p_lv)
    {
      q_lav = (p_la - p_lv) / R_lav;
    }
    else
    {
      q_lav = 0.0;
    }
    
    // assign values to algebraic array
    algebraic[0] = Clv;
    algebraic[1] = Crv;
    algebraic[2] = Cla;
    algebraic[3] = Cra;
    
    algebraic[4] = q_ri;
    algebraic[5] = q_rav;
    algebraic[6] = q_li;
    algebraic[7] = q_lav;

}

void heart::getDY(double t, double y[], double * DY)
{
    // load input values
    double C_ra_0 = y[inputIndex[0]]; // Right atrial compliance from previous timestep
    double C_rv_0 = y[inputIndex[1]]; // Right ventricular compliance from previous timestep
    double C_la_0 = y[inputIndex[2]]; // left atrial compliance from previous timestep
    double C_lv_0 = y[inputIndex[3]]; // left venricular compliance from previous timestep
    
    double p_ra = y[inputIndex[4]];
    double p_rv = y[inputIndex[5]];
    double p_la = y[inputIndex[6]];
    double p_lv = y[inputIndex[7]];

    
    // load algebraic parameters
    double C_lv_1 = algebraic[0];
    double C_rv_1 = algebraic[1];
    double C_la_1 = algebraic[2];
    double C_ra_1 = algebraic[3];
    
    double q_ri = algebraic[4];
    double q_rav = algebraic[5];
    double q_li = algebraic[6];
    double q_lav = algebraic[7];
    
    double DELTAT = 0.01;
    
    // load shared algebraic values
    double q_ro = *shared[0];
    double q_lo = *shared[1];
    
    DY[0]   = (C_ra_1 - C_ra_0)/DELTAT;  // Right Atrial Compliance
    DY[1]   = (C_rv_1 - C_rv_0)/DELTAT;  // Right Ventricle Compliance
    DY[2]   = (C_la_1 - C_la_0)/DELTAT;  // Left Atrial Compliance
    DY[3]   = (C_lv_1 - C_lv_0)/DELTAT;    // Left Ventricle Compliance

    DY[4]   = (1.0 / C_ra_0) * (- p_ra * DY[0] + q_ri - q_rav ); // right atrial pressure
    DY[5]   = (1.0 / C_rv_0) * (- p_rv * DY[1] + q_rav - q_ro); // right ventricular pressure.
    DY[6]   = (1.0 / C_la_0) * (- p_la * DY[2] + q_li - q_lav);     // Left Atrial Pressure
    DY[7]   = (1.0 / C_lv_0) * (- p_lv * DY[3] + q_lav - q_lo); // Left Ventricle pressure
}
