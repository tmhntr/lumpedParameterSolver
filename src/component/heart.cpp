//
//  heart.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-20.
//

//#include "submodule.hpp"
#include "heart.hpp"
#include <math.h>

//class heart: public submodule {
//    void updateAlgebraic(double t, double y[]);
//    void getDY(double t, double y[], double * DY);
//};

void heart::updateDerived(double t, double y[])
{
    // now load parameters in
    double RR = getP(0);
    double Esys_lv = getP(1);
    double Edias_lv = getP(2);
    double Esys_rv = getP(3);
    double Edias_rv = getP(4);
    double Esys_la = getP(5);
    double Edias_la = getP(6);
    double Esys_ra = getP(7);
    double Edias_ra = getP(8);

    double R_ri[nInlets];
    for (int i = 0; i < nInlets; i++)
        R_ri[i] = getP(9 + i);
    //    double R_ri = getP(9);
    double R_rav = getP(9 + nInlets);
    double R_li = getP(10 + nInlets);
    double R_lav = getP(11 + nInlets);

    // Inputs
    double p_ra = input(4);
    double p_rv = input(5);
    double p_la = input(6);
    double p_lv = input(7);

    double p_ri[nInlets];
    for (int i = 0; i < nInlets; i++)
        p_ri[i] = input(8 + i);

    double p_li = input(8 + nInlets); // pressure at left inlet

    double loc_t = fmod(t, RR);

    double Ts = 0.37 * sqrt(RR);
    double Tasys = 0.25 * sqrt(RR);
    double Tav = 0.19 * sqrt(RR);

    double act_fxn_v, act_fxn_a;

    // ventricular activation function
    if (loc_t > Tav && loc_t <= Tav + Ts) {
        act_fxn_v = (1.0 - cos(M_PI * (loc_t - Tav) / Ts));
    } else if (loc_t > Tav + Ts && loc_t <= Tav + 1.5 * Ts) {
        act_fxn_v = (1.0 + cos(2.0 * M_PI * (loc_t - Tav - Ts) / Ts));
    } else {
        act_fxn_v = 0.0;
    }

    // atrial activation function
    if ((0.0 < loc_t) && (loc_t <= Tasys)) {
        act_fxn_a = (1.0 - cos(M_PI * (loc_t / Tasys)));
    } else if ((Tasys <= loc_t) && (loc_t < 1.5 * Tasys)) {
        act_fxn_a = (1.0 + cos(2.0 * M_PI * (loc_t - Tasys) / Tasys));
    } else {
        act_fxn_a = 0.0;
    }

    // ventricular Elastance
    double Elv, Erv, Clv, Crv;
    Elv = Edias_lv + 0.5 * (Esys_lv - Edias_lv) * act_fxn_v;
    Erv = Edias_rv + 0.5 * (Esys_rv - Edias_rv) * act_fxn_v;

    // ventricular compliances
    Clv = 1.0 / Elv;
    Crv = 1.0 / Erv;

    // Atrial Elastance
    double Ela, Era, Cla, Cra;
    Ela = Edias_la + 0.5 * (Esys_la - Edias_la) * act_fxn_a;
    Era = Edias_ra + 0.5 * (Esys_ra - Edias_ra) * act_fxn_a;

    // Atrial compliances
    Cla = 1.0 / Ela;
    Cra = 1.0 / Era;

    // flows and pressures
    // right inlet flow
    double q_rav, q_li, q_lav;
    double q_ri[nInlets];
    for (int i = 0; i < nInlets; i++) {
        if (p_ri[i] > p_ra) {
            q_ri[i] = (p_ri[i] - p_ra) / R_ri[i];
        } else {
            q_ri[i] = 0.0;
        }
    }
    // right atrioventricular flow
    if (p_ra > p_rv) {
        q_rav = (p_ra - p_rv) / R_rav;
    } else {
        q_rav = 0.0;
    }

    // left inlet flow
    if (p_li > p_la) {
        q_li = (p_li - p_la) / R_li;
    } else {
        q_li = 0.0;
    }

    // left atrioventricular flow
    if (p_la > p_lv) {
        q_lav = (p_la - p_lv) / R_lav;
    } else {
        q_lav = 0.0;
    }

    // assign values to algebraic array
    setDerived(0, Clv);
    //    setAlgebraic(0, Clv);
    setDerived(1, Crv);
    setDerived(2, Cla);
    setDerived(3, Cra);
    for (int i = 0; i < nInlets; i++)
        setDerived(4 + i, q_ri[i]);
    setDerived(4 + nInlets, q_rav);
    setDerived(5 + nInlets, q_li);
    setDerived(6 + nInlets, q_lav);
}

void heart::getDY(double t, double y[], double* DY)
{
    // load input values
    double C_ra_0 = input(0); // Right atrial compliance from previous timestep
    double C_rv_0 = input(1); // Right ventricular compliance from previous timestep
    double C_la_0 = input(2); // left atrial compliance from previous timestep
    double C_lv_0 = input(3); // left venricular compliance from previous timestep

    double p_ra = input(4);
    double p_rv = input(5);
    double p_la = input(6);
    double p_lv = input(7);

    // load algebraic parameters
    double C_lv_1 = input(9 + nInlets);
    double C_rv_1 = input(10 + nInlets);
    double C_la_1 = input(11 + nInlets);
    double C_ra_1 = input(12 + nInlets);
    double q_ri = 0;
    for (int i = 0; i < nInlets; i++)
        q_ri += input(13 + nInlets);
    double q_rav = input(14 + nInlets);
    double q_li = input(15 + nInlets);
    double q_lav = input(16 + nInlets);

    //    double DELTAT = 0.01;

    // load shared algebraic values
    double q_ro = input(17 + nInlets);
    double q_lo = input(18 + nInlets);

    DY[0] = (C_ra_1 - C_ra_0) / DELTAT; // Right Atrial Compliance
    DY[1] = (C_rv_1 - C_rv_0) / DELTAT; // Right Ventricle Compliance
    DY[2] = (C_la_1 - C_la_0) / DELTAT; // Left Atrial Compliance
    DY[3] = (C_lv_1 - C_lv_0) / DELTAT; // Left Ventricle Compliance

    DY[4] = (1.0 / C_ra_0) * (-p_ra * DY[0] + q_ri - q_rav); // right atrial pressure
    DY[5] = (1.0 / C_rv_0) * (-p_rv * DY[1] + q_rav - q_ro); // right ventricular pressure.
    DY[6] = (1.0 / C_la_0) * (-p_la * DY[2] + q_li - q_lav); // Left Atrial Pressure
    DY[7] = (1.0 / C_lv_0) * (-p_lv * DY[3] + q_lav - q_lo); // Left Ventricle pressure
}
