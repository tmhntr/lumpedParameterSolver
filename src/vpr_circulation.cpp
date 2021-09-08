//
//  rat-heart.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#include "vpr_circulation.hpp"
#include <math.h>
#include <zero_rc.hpp>
/*
 adapted from beard et al, 2012
 Annals of Biomedical Engineering, Vol. 40, No. 11, November 2012 (Ó 2012) pp. 2365–2378 DOI: 10.1007/s10439-012-0611-7


 Shared: segment bulk flow rate (from myoRC)
 Input States: D [nSegments], A [nSegments], p [nSegments]
 Algebraic: T_total [nSegments], A_total [nSegments], Tension [nSegments]
 State variables: Diameter [nSegments], Activation [nSegments]

 Parameters (all multiplied by nSegments): C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_rbc_meta, C_ATP_0, C_myo, C_shear, C_symp, C_tone_primeprime, tau_d, tau_a, mu, nParallel

 Note that [n] denotes multiple values
 Note that (module) denotes the probably source of such values
 */

void vpr_circulation::updateDerived(double t, double y[])
{
    // add annotation to all equations and variables

    // State variables
    double A, C_ach, eps_1, eps_2, eps_3, Q_av, Q_mt, Q_pv, Q_tc, V_ao, V_lv, V_pa, V_pu, V_rv, V_vc, c_nor, delta_HR_pslow, delta_HR_s, Psi, V_spt;

    A = input(0);
    C_ach = input(1);
    eps_1 = input(2);
    eps_2 = input(3);
    eps_3 = input(4);
    Q_av = input(5);
    Q_mt = input(6);
    Q_pv = input(7);
    Q_tc = input(8);
    V_ao = input(9);
    V_lv = input(10);
    V_pa = input(11);
    V_pu = input(12);
    V_rv = input(13);
    V_vc = input(14);
    c_nor = input(15);
    delta_HR_pslow = input(16);
    delta_HR_s = input(17);
    Psi   = input(18);


//    V_spt = input(19)];

//    int status = 0;
//    V_spt = 0.016;
//    double value;
//    while (1)
//    {
//        value = ( e_t * E_es_spt * (V_spt-V_d_spt) + (1.0-e_t) * P_0_spt * (exp(lambda_spt * (V_spt-V_0_spt)) - 1.0)
//                 - e_t * E_es_lvf * (V_lv-V_spt) - (1.0-e_t) * P_0_lvf * (exp(lambda_lvf * (V_lv-V_spt)) - 1.0)
//                 + e_t * E_es_rvf * (V_rv+V_spt) + (1.0-e_t) * P_0_rvf * (exp(lambda_rvf * (V_rv+V_spt)) - 1.0) );
//        zero_rc ( 0.0, 0.1, 1e-4, &V_spt, &status, value );
//        if (status > 0)
//            break;
//    }


    // Parameters
    double B, C;
    B = 80.0;
    C = 0.375;
    // Volumes
    double V_d_ao, V_d_lvf, V_d_pa, V_d_pu, V_d_rvf, V_d_spt, V_d_vc, Delta_P;
    V_d_ao = 0.0;
    V_d_lvf = 0.0;
    V_d_pa = 0.0;
    V_d_pu = 0.0;
    V_d_rvf = 0.0;
    V_d_spt = 0.0083707;
    V_d_vc = 0.0;
    Delta_P = 0.0;
    double P_th = -4.0 + Delta_P;
    // Constitutive law lambda's
    double lambda_lvf, lambda_pcd, lambda_rvf, lambda_spt;
    lambda_lvf = 7.6155;
    lambda_pcd = 1.5009;
    lambda_rvf = 5.1312;
    lambda_spt = 53.897;
    // Constitutive law V_0's
    double V_0_lvf, V_0_pcd, V_0_rvf, V_0_spt;
    V_0_lvf = 0.0;
    V_0_pcd = 1.5093;
    V_0_rvf = 0.0;
    V_0_spt = 0.024019;
    // Constitutive law P_0's
    double P_0_lvf, P_0_pcd, P_0_rvf, P_0_spt;
    P_0_lvf = 0.56285;
    P_0_pcd = 3.307;
    P_0_rvf = 1.6159;
    P_0_spt = 14.609;
    // Inertances
    double L_av, L_mt, L_pv, L_tc;
    L_av = 0.0024479;
    L_mt = 0.0014814;
    L_pv = 0.0065968;
    L_tc = 0.0032383;
    // Resistances
    double R_mt, R_pul, R_pv, R_sys, R_tc, R_av;
    R_mt  = 2.26;
    R_pul = 22.17;
    R_pv  = 0.786;
    R_sys = 155.56;
    R_tc  = 3.386;
    R_av  = 2.57;

    // Baroreflex model parameters
    double Cwall, B1, B2, B3, Bwall, q_ach, q_nor, t_nor, t_ach, t_HR_ach, t_HR_nor, Beta, alpha_p0, alpha_s0, Zeta, K1, K2, K3, Kne, K_ach, K_nor, S, Tsmax, Tsmin, Tpmax, Tpmin, R0, HRmax, HRmin, HRo, Gamma, Gcns, Gp, Gs, delta_th;
    Cwall = .006; //Aortic Distensibility Constant
    B1 = 1.0;
    B2 = 10.0;
    B3 = 207.0;
    Bwall = 1.0; //Aortic Wall Viscous Constant
    q_ach = 5.0;
    q_nor = 0.1099;
    t_nor = 9.1; //Norepinephrine Reuptake Time Constant
    t_ach = 0.2; //Acetylcholine Hydrolysis Time Constant
    t_HR_ach = 2.5; //Slow Acetylcholine Heart Rate Response Time Constant
    t_HR_nor = 2.1; //Norepinephrine Heart Rate Response Time Constant
    Beta = 0.175;
    alpha_p0 = 76.0; //Parasympathetic Offset
    alpha_s0 = 59.0; //Sympathetic Offset
    Zeta = 0.9; //Threshold Frequency Scaling Factor
    K1 = 1.5; //Baroreceptor Connective Tissue Elastic Constant
    K2 = 3.75; //Baroreceptor Connective Tissue Elastic Constant
    K3 = 1.05; //Baroreceptor Connective Tissue Elastic Constant
    Kne = 1.0; //Baroreceptor Nerve Ending Elastic Constant
    K_ach = 0.65; //c_ach_half
    K_nor = 1.12; //c_nor_half
    S = 480.0;  //Strain Sensetivity
    Tsmax = 4.12; //Maximum Sympathetic Tone
    Tsmin = 0.5;  //Minimum Sympathetic Tone
    Tpmax = 3.0; //Maximum Parasympathetic Tone
    Tpmin = 0.5; //Minimum Parasympathetic Tone
    R0 = 1.6;    //Aortic Unstressed Radius
    HRmax = 483; //Maximum Heart Rate
    HRmin = 226; //Minimum Heart Rate
    HRo = 283; //Intrinsic Heart Rate
    Gamma = 0.75; //Fast Pathway Proportion
    Gcns = 1.0; //Gain Central Nervous System
    Gp = 0.492; //Gain Parasympathetic
    Gs = 0.178; //Gain Sympathetic
    double delta_HR_pmax = HRo-HRmin; //Maximum Parasympathetic Heart Rate Response
    double delta_HR_smax = HRmax-HRo; //Maximum Sympathetic Heart Rate Response
    delta_th = 0.0; //Strain Threshold




    // Model Equations

    double R = pow((A/3.14159),0.5);
    double eps_wall = (R-R0)/R0;
    double delta = eps_wall-eps_1; //Baroreceptor Nerve Ending Strain

    //Baroreceptor Firing Rate
    double n;
    if (delta < delta_th)
        n = 0.0;
    else
        n = S*(delta-Zeta*delta_th);

    double alpha_cns = Gcns*n;
    double Tp = Tpmin+(Tpmax-Tpmin)/(exp(-Gp*(alpha_cns-alpha_p0))+1);
    double Ts = Tsmin+(Tsmax-Tsmin)/(exp(Gs*(alpha_cns-alpha_s0))+1);
    double delta_HR_ps = delta_HR_pmax*pow(C_ach,2)/(pow(K_ach,2)+pow(C_ach,2));
    double delta_HR_ss = delta_HR_smax*pow(c_nor,2)/(pow(K_nor,2)+pow(c_nor,2));
    double delta_HR_pfast = Gamma*delta_HR_ps;
    double delta_HR_p = delta_HR_pfast+delta_HR_pslow;
    double HR_p = HRo-delta_HR_p;
    double HR_s = HRo+delta_HR_s;
    double HR = HR_p+(HR_s-HRo)*(HR_p-Beta*HRmin)/(HRo-Beta*HRmin);
    // Elastances
    double E_es_ao, E_es_lvf, E_es_pa, E_es_pu, E_es_rvf, E_es_spt, E_es_vc;
    E_es_ao  = 244.66;
    E_es_lvf = 1541.4*(1.0 + (HR-HRo)/HRo );
    E_es_pa  = 37.244;
    E_es_pu  = 0.49882;
    E_es_rvf = 56.823*(1.0 + (HR-HRo)/HRo );
    E_es_spt = 8708.7*(1.0 + (HR-HRo)/HRo );
    E_es_vc  = 0.59847;


    // Aortic pressure
    double P_ao = E_es_ao*(V_ao-V_d_ao);
    // Vessel wall mechanics
    double C1, C2, C3, C4, C5, C6;
    C1 = (Kne*(eps_wall - eps_1) - K1*(eps_1 - eps_2))/B1;
    C2 = (K1*(eps_1 - eps_2) - K2*(eps_2 - eps_3))/(B1 + B2);
    C3 = B1/(B1 + B2);
    C4 = B2/(B1 + B2);
    C5 = (K2*(eps_2 - eps_3) - K3*eps_3)/(B2 + B3);
    C6 = B2/(B2 + B3);



    double pmod = (HR/HRo);
    // pmod = 1;
    double theta = fmod(Psi,1.0); // fraction of beat (0,1)
    double e_t = exp(-B*pow(pmod,2.0)*pow((theta-C/pmod),2.0)); //Beat Driver Function

    int status = 0;
    V_spt = 0.016;
    double value;
    double tol = 1e-8;
    while (1)
    {
        value = ( e_t * E_es_spt * (V_spt-V_d_spt) + (1.0-e_t) * P_0_spt * (exp(lambda_spt * (V_spt-V_0_spt)) - 1.0)
                 - e_t * E_es_lvf * (V_lv-V_spt) - (1.0-e_t) * P_0_lvf * (exp(lambda_lvf * (V_lv-V_spt)) - 1.0)
                 + e_t * E_es_rvf * (V_rv+V_spt) + (1.0-e_t) * P_0_rvf * (exp(lambda_rvf * (V_rv+V_spt)) - 1.0) );
        zero_rc ( 0.0, 0.1, tol, V_spt, status, value );
        if (status == 0)
            break;
    }

    double V_lvf = V_lv-V_spt;
    double V_pcd = V_lv+V_rv;
    double V_rvf = V_rv+V_spt;

    double P_ed_lvf, P_ed_rvf, P_es_lvf, P_es_rvf, P_lvf, P_pa, P_pcd, Pi, P_lv, P_pu, P_rvf, P_rv, P_vc;
    P_ed_lvf = P_0_lvf*(exp(lambda_lvf*(V_lvf-V_0_lvf))-1.0);
    P_ed_rvf = P_0_rvf*(exp(lambda_rvf*(V_rvf-V_0_rvf))-1.0);
    P_es_lvf = E_es_lvf*(V_lvf-V_d_lvf);
    P_es_rvf = E_es_rvf*(V_rvf-V_d_rvf);
    P_lvf = e_t*P_es_lvf+(1-e_t)*P_ed_lvf;
    P_pa = (E_es_pa*(V_pa-V_d_pa)+P_th);
    P_pcd = P_0_pcd*(exp(lambda_pcd*(V_pcd-V_0_pcd))-1.0);
    Pi = P_pcd+P_th;
    P_lv = P_lvf+Pi;
    P_pu = (E_es_pu*(V_pu-V_d_pu)+P_th);
    P_rvf = e_t*P_es_rvf+(1.0-e_t)*P_ed_rvf;
    P_rv = P_rvf+Pi;
    P_vc = E_es_vc*(V_vc-V_d_vc);

    double Q_pul, Q_sys;
    Q_pul = (P_pa-P_pu)/(R_pul);
    Q_sys = (P_ao-P_vc)/(R_sys);

    int c = 0;
    // set baroreflex values of algebraic
    for (double val : {P_ao, Tp, C1, C2, C3, C4, C5, C6, Ts, delta_HR_ps, delta_HR_ss}) {
        setDerived(c, val);
        c++;
    }
    // heart mechanics
    for (double val : {HR, e_t, V_spt, E_es_spt, E_es_lvf, E_es_rvf}) {
        setDerived(c, val);
        c++;
    }
    // flow mechanics
    for (double val : {P_lv, P_pu, P_rv, P_pa, P_vc, Q_sys, Q_pul}) {
        setDerived(c, val);
        c++;
    }

}

void vpr_circulation::getDY(double t, double y[], double * DY)
{

    // Baroreflex
    // parameters
    double R0, Cwall, Bwall, t_ach, q_ach; // vessel wall mechanics
    double t_nor, q_nor, Gamma, t_HR_ach, t_HR_nor; // br integration
    R0 = 1.6;    //Aortic Unstressed Radius
    Cwall = .006; //Aortic Distensibility Constant
    Bwall = 1.0; //Aortic Wall Viscous Constant
    t_ach = 0.2; //Acetylcholine Hydrolysis Time Constant
    q_ach = 5.0;
    t_nor = 9.1; //Norepinephrine Reuptake Time Constant
    q_nor = 0.1099;
    Gamma = 0.75; //Fast Pathway Proportion
    t_HR_ach = 2.5; //Slow Acetylcholine Heart Rate Response Time Constant
    t_HR_nor = 2.1; //Norepinephrine Heart Rate Response Time Constant

    // State inputs
    double A, C_ach; // vessel wall mechanics
    double c_nor, delta_HR_pslow, delta_HR_s; // br integration
    A = input(0);
    C_ach = input(1);
    c_nor = input(15);
    delta_HR_pslow = input(16);
    delta_HR_s = input(17);

    // Derived algebraic vavlues
    double P_ao, Tp; // vessel wall mechanics
    double C1, C2, C3, C4, C5, C6; // vessel wall mechanics
    double Ts, delta_HR_ps, delta_HR_ss; // br integration
    P_ao = input(19+0);
    Tp = input(19+1);
    C1 = input(19+2);
    C2 = input(19+3);
    C3 = input(19+4);
    C4 = input(19+5);
    C5 = input(19+6);
    C6 = input(19+7);
    Ts = input(19+8);
    delta_HR_ps = input(19+9);
    delta_HR_ss = input(19+10);


    // heart mechanics
    // parameters
    double V_d_spt, P_0_spt, lambda_spt, V_0_spt, P_0_lvf, lambda_lvf, P_0_rvf, lambda_rvf;
    lambda_lvf = 7.6155;
//    lambda_pcd = 1.5009;
    lambda_rvf = 5.1312;
    lambda_spt = 53.897;
    P_0_lvf = 0.56285;
//    P_0_pcd = 3.307;
    P_0_rvf = 1.6159;
    P_0_spt = 14.609;
    V_d_spt = 0.0083707;
    V_0_spt = 0.024019;

    // State inputs
    double V_lv, V_rv;
    V_lv = input(10);
    V_rv = input(13);
//    V_spt = input(19)];

    // Derived algebraic vavlues
    double HR, e_t, V_spt, E_es_spt, E_es_lvf, E_es_rvf;
    HR = input(19+11);
    e_t = input(19+12);
    V_spt = input(19+13);
    E_es_spt = input(19+14);
    E_es_lvf = input(19+15);
    E_es_rvf = input(19+16);


    // flow mechanics
    // parameters
    double R_av, L_av, R_mt, L_mt, R_pv, L_pv, R_tc, L_tc;
    // Inertances
    L_av = 0.0024479;
    L_mt = 0.0014814;
    L_pv = 0.0065968;
    L_tc = 0.0032383;
    // Resistances
    R_mt  = 2.26;
    R_pv  = 0.786;
    R_tc  = 3.386;
    R_av  = 2.57;


    // State inputs
    double Q_av, Q_mt, Q_pv, Q_tc;
    Q_av = input(5);
    Q_mt = input(6);
    Q_pv = input(7);
    Q_tc = input(8);

    // Derived algebraic vavlues
    double P_lv, P_pu, P_rv, P_pa, P_vc, Q_sys, Q_pul;
    P_lv = input(19+17);
    P_pu = input(19+18);
    P_rv = input(19+19);
    P_pa = input(19+20);
    P_vc = input(19+21);
    Q_sys = input(19+22);
    Q_pul = input(19+23);


// Pressures, flows, resistances and inertances
    DY[0]= (-(sqrt(A/M_PI)-R0)/Cwall+P_ao)/Bwall ;
    DY[1]= (-C_ach/t_ach+q_ach*Tp);
    DY[2] = (C1*(1.0 - C4*C6) + C2 + C4*C5)/(1.0 - C3 - C4*C6);

    DY[3] = (C2 + C4*C5 + C3*DY[2])/(1.0 - C4*C6);

    DY[4] =  C5 + C6 * DY[3];


//    DY[19] = ( e_t * E_es_spt * (V_spt-V_d_spt) + (1.0-e_t) * P_0_spt * (exp(lambda_spt * (V_spt-V_0_spt)) - 1.0)
//              - e_t * E_es_lvf * (V_lv-V_spt) - (1.0-e_t) * P_0_lvf * (exp(lambda_lvf * (V_lv-V_spt)) - 1.0)
//              + e_t * E_es_rvf * (V_rv+V_spt) + (1.0-e_t) * P_0_rvf * (exp(lambda_rvf * (V_rv+V_spt)) - 1.0) );

//    if(t > 0.00844075)
//    {
//        std::cout << e_t * E_es_spt * (V_spt-V_d_spt) << " + " << (1.0-e_t) * P_0_spt * (exp(lambda_spt * (V_spt-V_0_spt)) - 1.0) << std::endl;
//        std::cout << e_t * E_es_lvf * (V_lv-V_spt) << " - " << (1.0-e_t) * P_0_lvf * (exp(lambda_lvf * (V_lv-V_spt)) - 1.0) << std::endl;
//        std::cout << e_t * E_es_rvf * (V_rv+V_spt) << " + " << (1.0-e_t) * P_0_rvf * (exp(lambda_rvf * (V_rv+V_spt)) - 1.0) << std::endl;
//
//    }


    //Valves
    if (((P_lv-P_ao)<0.0) && (Q_av<0.0))
        DY[5] = 0.0; //Q_av
    else {
        DY[5] = (P_lv-P_ao-Q_av*R_av)/L_av;
//        std::cout << (P_lv-P_ao-Q_av*R_av)/L_av << std::endl;
    }


    if (((P_pu-P_lv)<0.0) && (Q_mt<0.0))
        DY[6] = 0.0; //Q_mt
    else{
        DY[6] = (P_pu-P_lv-Q_mt*R_mt)/L_mt;
//        std::cout << DY[6] << std::endl;
    }

    if (((P_rv-P_pa)<0.0) && (Q_pv<0.0))
        DY[7] = 0.0; //Q_pv
    else
        DY[7] = (P_rv-P_pa-Q_pv*R_pv)/L_pv;

    if (((P_vc-P_rv)<0.0) && (Q_tc<0.0))
        DY[8] = 0.0; //Q_tc
    else
        DY[8] = (P_vc-P_rv-Q_tc*R_tc)/L_tc;


    //Chamber Volumes
    //V_ao
    DY[9] = (Q_av-Q_sys);

//    if (Q_av<0.0)
//        DY[9] = -Q_sys;
//    else
//        DY[9] = (Q_av-Q_sys);

    //V_lv
    DY[10] = (Q_mt-Q_av);

//    if ((Q_mt<0.0) && (Q_av<0.0))
//        DY[10] =  0.0;
//    else if (Q_mt<0.0)
//        DY[10] = -Q_av;
//    else if (Q_av<0.0)
//        DY[10] = Q_mt;
//    else
//        DY[10] = (Q_mt-Q_av);

    //V_pa
    DY[11] = (Q_pv-Q_pul);

//    if (Q_pv<0.0)
//        DY[11] = -Q_pul;
//    else
//        DY[11] = (Q_pv-Q_pul);

    //V_pu
    DY[12] = (Q_pul-Q_mt);

//    if (Q_mt<0.0)
//        DY[12] = Q_pul;
//    else
//        DY[12] = (Q_pul-Q_mt);

    //V_rv
    DY[13] = (Q_tc-Q_pv);

//    if ((Q_tc<0.0) && (Q_pv<0.0))
//        DY[13] = 0.0;
//    else if (Q_tc<0.0)
//        DY[13] = -Q_pv;
//    else if (Q_pv<0.0)
//        DY[13] = Q_tc;
//    else
//        DY[13] = (Q_tc-Q_pv);

    //V_vc
    DY[14] = (Q_sys-Q_tc);

//    if (Q_tc<0.0)
//        DY[14] = Q_sys; //V_vc
//    else
//        DY[14] = (Q_sys-Q_tc);


    DY[15]= (-c_nor/t_nor+q_nor*Ts);
    DY[16]= (-delta_HR_pslow+(1.0-Gamma)*delta_HR_ps)/t_HR_ach;
    DY[17]= (-delta_HR_s+delta_HR_ss)/t_HR_nor;

    DY[18] = HR/60.0;


}
