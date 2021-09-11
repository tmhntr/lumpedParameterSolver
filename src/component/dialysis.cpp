//
//  dialysis.cpp
//  model
//
//  Created by Tim  Hunter on 2021-09-03.
//

#include "dialysis.hpp"



void dialysis::updateDerived(double t, double y[])
{
    double M_eqic    = getP(0);
    double M_eqex    = getP(1);
    double Q_f        = getP(2);     // % Ultrafiltration rate. units:mL/s
    double Qinfused  = getP(3);     // % infusion rate (Qinfused is used in place of Qinf; which is already flow in inferior vena cava.) units:mL/s
    double Q_B       = getP(4);
    // %
    // % from Ursino 2000, p207
    // % Dialysate concentrations of solutes TEMPORARY (may be passed from p)
    double c_u_d       = getP(5); // % urea concentration in dialysate. placeholder. p207 of Ursino 2000.
    double c_na_d      = getP(6); // % sodium concentration in dialysate. placeholder. p207 of Ursino 2000.
    double c_k_d       = getP(7); // % potassium concentration in dialysate. placeholder. p207 of Ursino 2000.
    double c_cl_d      = getP(8); // % chlorine concentration in dialysate. placeholder. p207 of Ursino 2000.
    double c_hco3_d    = getP(9); // % carbonate concentration in dialysate. placeholder. p207 of Ursino 2000.
    // %
    // % from ursino 2008
    double c_u_inf     = getP(10); // % , units: mmol/L
    double c_na_inf    = getP(11); // % , units: mmol/L
    double c_k_inf     = getP(12); // % , units: mmol/L
    double c_cl_inf    = getP(13); // % TEMPORARY this may be a passed parameter

    // %%% ALL CONSTANT PARAMETERS ARE LISTED BELOW %%%
    // % Parameters are listed in the order they are called by the ODEs
    // %
    // % Fluid and Solute Kinetics
    // % Table 2, Ursino 2008 (unless stated otherwise).
    double D_s              = getP(14); // % Na, Cl, K clearance/dialysance. Table 2, Ursino 2008. units: mL/s.
    double D_u              = getP(15); // % urea clearance/dialysance. Table 2, Ursino 2008. units: mL/s.
    double D_hco3           = getP(16); // % carbonate clearance/dialysance. Table 1, Ursino 2000. units: L/s.
    double F_p                  = getP(17); // % F_p: plasma water fraction
    double F_R                  = getP(18); // % F_R: RBC water fraction
    double gamma_U          = getP(19); // % gamma_U: fraction of red blood cell water that participates in the transfer through the dialyzer
    double R_DU                 = getP(20); // % R_DU: Donnan ratio for Urea.
    double gamma_Na         = getP(21); // % gamma_Na: fraction of red blood cell water that participates in the transfer through the dialyzer
    double gamma_K          = getP(22); // % gamma_K: fraction of red blood cell water that participates in the transfer through the dialyzer
    double gamma_Cl         = getP(23); // % gamma_Cl: fraction of red blood cell water that participates in the transfer through the dialyzer
    double gamma_HCO3   = getP(24); // % gamma_HCO3: fraction of red blood cell water that participates in the transfer through the dialyzer
    double k_Na                 = getP(25); // % k_Na: mass transfer coefficient for Na; units (ml/s)
    double beta_Na          = getP(26); // % beta_Na: mass transfer coefficient for Na; units (n/a)
    double k_K                  = getP(27); // % k_K: mass transfer coefficient for K; units (ml/s)
    double beta_K               = getP(28); // % beta_K: mass transfer coefficient for K; units (n/a)
    double k_U                  = getP(29); // % k_U: mass transfer coefficient for U; units (ml/s)
    double beta_U               = getP(30); // % beta_U: mass transfer coefficient for U; units (n/a)
    double k_f                  = getP(31); // % k_f: water exchange coefficient; units (L^2 s^-1 mmol^-1)
    double E_is                 = getP(32); // % E_is: elastance of the interstitial space; units (mmHg/L)
    double V_isn                = getP(33); // % V_isn: basal volume of interstitial compartments; units (L)
    double V_pln                = getP(34); // % V_pln: basal volume of blood plasma; units (L)
    double c_ppln               = getP(35); // % c_ppln: basal protein concentration in plasma; units (g/dl)
    double c_pisn               = getP(36); // % c_pisn: basal protein concentration in interstitial compartment; units (g/dl)

    double alphaa               = getP(37); // % Gibbs Donnan ratio for anions. Ursino 2000, table 1.
    double alphac               = getP(38); // % Gibbs Donnan ratio for cations. Ursino 2000, table 1.
    // % Table 1 from Ursino 2000 p 205
    double eta_hco3     = getP(39); // % eta_hco3: bicarbonate mass transfer coefficient. table 1, Ursino 2000. units: L/s.
    double eta_h        = getP(40); // % eta_h: hydrogen ion mass transfer coefficient. table 1, Ursino 2000. units: L/s.
    double g_hco3       = getP(41); // % g_hco3: bicarbonate equilibrium ratio. table 1, Ursino 2000. units: dimensionless.
    double g_h          = getP(42); // % g_h: hydrogen ion equilibrium ratio. table 1, Ursino 2000. units: dimensionless.
    // % TEMPORARY insert references
    double etaprime_r          = getP(43); // % etaprime_r: reaction velocity, hco3 buffer. table 1, Ursino 2000. see reaction i. units: L^2/s/mmol
    double kprime_a          = getP(44); // % kprime_a: dissociation constant; p209 of Ursino 2000, right column 3rd para.
    double etaprimeprime_r  = getP(45); // % etaprimeprime_r: reaction velocity, protein buffer. table 1, Ursino 2000. see reaction ii. units: L^2/s/mmol
    double kprimeprime_a      = getP(46); // % kprimeprime_a:dissociation constant; p209 of Ursino 2000, right column 3rd para.
    double c_co2_ic              = getP(47); // % c_co2_ic: concentration of CO2 in ic compartment; (in eq 20) Ursino 2000, p209, para 3 on right column.
    double c_co2_ex              = getP(48); // % c_co2_ex: concentration of CO2 in ex compartment; (in eq 20) Ursino 2000, p209, para 3 on right column.
    double c_p_ic0            = getP(49); // % c_p_ic0: basal protein concentration in intracellular compartment; units mmol/L, Table 1 ursino 2000

    double M_u_ic = input(0);
    double M_na_ic = input(1);
    double M_k_ic = input(2);
    double M_cl_ic = input(3);
    double M_hco3_ic = input(4);
    double M_h_ic = input(5);
    double M_p_ic = input(6);

    double M_u_ex= input(7);
    double M_na_ex= input(8);
    double M_k_ex= input(9);
    double M_cl_ex= input(10);
    double M_hco3_ex= input(11);
    double M_h_ex= input(12);
    double M_p_ex = input(13);

    double V_ic = input(14);
    double V_is = input(15);

    // % Capillary pressure alues from Lim 2008 p584 paragraph 1
    // % units (mmHg)
    double pac1 = input(getNEQ()+0);
    double pac2 = input(getNEQ()+1);
    double pac3 = input(getNEQ()+2);
    double pvc1 = input(getNEQ()+0) - input(getNEQ()+3);
    double pvc2 = input(getNEQ()+1) - input(getNEQ()+4);
    double pvc3 = input(getNEQ()+2) - input(getNEQ()+4);
    // double pac1     = Y[3]        ; double pac2     = Y[5]       ;  double pac3      = Y[6];
    // double pvc1     = Y[3] - Y[13]; double pvc2     = Y[5] - Y[7];  double pvc3      = Y[6] - Y[7];

    double V_b = 0.0; // whole blood volume
    if (nCompartments > 0)
    {
      for (int i = 0; i < nCompartments; i++)
      {
        V_b += input(getNEQ()+5+i);
      }
    }
    else
    {
      V_b = input(16);
    }


    double V_rc     = 1.3;          // % red blood cell volume, units (L) ursino + innocenti 2008
    double V_pl        = V_b - V_rc;  // %  blood plasma volume
    double Hct      = V_rc / V_b;

    // % eq. 9 of Ursino 2000 p206.
    // % Volume of extracellular fluid (l)
    double V_ex                      = V_is + V_pl;  // % for equation 5 in Lim et al. Then Urisino 2000 gives formula eq. 9.




    double pis0         = getP(50); // % pis0: basal pressure in is compartment; units: mmHg; eq. 10, appendix 1. Ursino 2000. table 1.
    double La           = getP(51); // % La: arterial capillary permeability; units: mL/mmHg/s; table 1. Ursino 2008.
    double Lv           = getP(52); // % Lv: venous capillary permeability; units: mL/mmHg/s; table 1. Ursino 2008, table 1.
    // %
    // JJ TH NOV 3
    // double sigma_lv                     = data->p_ursino[94 + 19]; // % units: mmHg/ml
    // double sigma_R_v                     = data->p_ursino[95 + 19]; //  units: mmHg/ml
//    double delta_sigma_V                 = data->p_ursino[96 + 19]; //  units: ml
//    double V_gain                             = delta_sigma_V / 2785; //  This is the change in venous volume / total venous volume
//    double sigma_R                             = data->p_ursino[97 + 19]; //  units: mmHg/(ml/s)

    // %// %// % ALL ALGEBRAIC EQNS ARE LISTED BELOW // %// %// %
    // %
    // % eq. 16 of Ursino 2000 p207
    // % solute concentration in Intracellular compartment
    // % Check if abs(y(1)) < 10^(-16)
    double c_u_ic         = M_u_ic    / V_ic; // % urea concentration.                 Muic is a state variable. eq. 16 of Ursino 2000.
    double c_na_ic        = M_na_ic   / V_ic; // % sodium concentration.             Mnaic is a state variable. eq. 16 of Ursino 2000.
    double c_k_ic         = M_k_ic    / V_ic; // % potassium concentration.     Mkic is a state variable. eq. 16 of Ursino 2000.
    // double c_cl_ic        = M_cl_ic   / V_ic; // % chlorine concentration.         Mclic is a state variable. eq. 16 of Ursino 2000.
    double c_hco3_ic      = M_hco3_ic / V_ic; // % carbonate concentration.     Mhco3lic is a state variable. eq. 16 of Ursino 2000.
    double c_h_ic         = M_h_ic    / V_ic; // % hydrogen concentration.         Mhlic is a state variable. eq. 16 of Ursino 2000.
    double c_p_ic         = M_p_ic    / V_ic; // % protein concentration.         Mpic is a state variable. eq. 16 of Ursino 2000.



    // % This approximation (csex = cspl = csis) proposed by ursino and innocenti
    // % 2008 eq. 24
    double c_u_ex         = M_u_ex    / V_ex; /* double c_u_pl     = c_u_ex  ; */ double c_u_is      = c_u_ex;
    double c_na_ex        = M_na_ex   / V_ex; /* double c_na_pl    = c_na_ex ; */ double c_na_is     = c_na_ex;
    double c_k_ex         = M_k_ex    / V_ex; /* double c_k_pl     = c_k_ex  ; */ double c_k_is      = c_k_ex;
    double c_cl_ex        = M_cl_ex   / V_ex; // double c_cl_pl    = c_cl_ex ; double c_cl_is     = c_cl_ex;
    double c_hco3_ex      = M_hco3_ex / V_ex; double c_hco3_pl  = c_hco3_ex; double c_hco3_is  = c_hco3_ex;
    double c_h_ex         = M_h_ex    / V_ex; double c_h_pl     = c_h_ex  ; double c_h_is      = c_h_ex;
    double c_p_ex         = M_p_ex    / V_ex; double c_p_pl     = c_p_ex  ; double c_p_is      = c_p_ex;

    // % eq. 2-3 of Ursino 2000 p205.
    // % Osmotic activity of fluid compartment (mmol/l)
    double c_ic          = (M_u_ic + M_na_ic + M_k_ic + M_eqic) / V_ic ;         // % eq 2, page 205 Ursino 2000; Intracellular osmotic activity
    double c_is          = (M_u_ex + M_na_ex + M_k_ex + M_eqex) / V_ex ;         // % eq 3, page 205 Ursino 2000; Interstitial osmotic activity

    double c_ppl  = c_ppln * (V_pln / V_pl); // % Ursino & Innocenti 2008 eq. 39
    double c_pis  = c_pisn * (V_isn / V_is); // % Ursino & Innocenti 2008 eq. 38

    // % eq. 10, appendix 1 of Lim 2008 p 584
    // % pressure in the interstitial compartment (mmHg)
    double pis          = E_is * ( input(1) - V_isn ) + pis0;

    // % eq. 11, appendix 1 of Lim 2008 p 584
    // % osmotic pressure in the blood plasma (mmHg)
    double pipl          = 2.1 * c_ppl + 0.16 * c_ppl*c_ppl + 0.009 * c_ppl * c_ppl * c_ppl;         // % eq. 11, appendix 1.

    // % eq. 12, appendix 1 of Lim 2008 p 584
    // % osmotic pressure of the interstitial space
    double piis          = 2.8 * c_pis + 0.18 * c_pis*c_pis + 0.012 * c_pis * c_pis * c_pis;         // % eq. 12, appendix 1.

    // % eq. 8, appendix 1 of Lim 2008 p 584
    double F_a1          = La * (pac1 - pis - pipl + piis );     // % eq. 8, appendix 1, for n = 1.
    double F_a2          = La * (pac2 - pis - pipl + piis );     // % eq. 8, appendix 1, for n = 2.
    double F_a3          = La * (pac3 - pis - pipl + piis );     // % eq. 8, appendix 1, for n = 3.

    // % eq. 9, appendix 1 of Lim 2008 p 584
    double R_v1          = Lv * (pis - pvc1 + pipl - piis );     // % eq. 9, appendix 1, for n = 1.
    double R_v2          = Lv * (pis - pvc2 + pipl - piis );     // % eq. 9, appendix 1, for n = 2.
    double R_v3          = Lv * (pis - pvc3 + pipl - piis );     // % eq. 9, appendix 1, for n = 3.

    // % eq. 6, appendix 1. of Ursino 2000.
    // % F_a is the rate at which fluid is filtered at arterial capillaries.
    double F_a             = F_a1 + F_a2 + F_a3;                                     // % units (ml/s), eq. 6, appendix 1. of Ursino 2000.

    // % eq. 7, appendix 1. of Ursino 2000.
    // % R_v is the rate at which fluid is filtered at venous capillaries.
    double R_v             = R_v1 + R_v2 + R_v3;                                     // % units (ml/s), eq 7, appendix 1. of Ursino 2000.


    // % eq. 12 from Ursino 2000 p206 (unless stated otherwise)
    // % mass transfer rate from ic compartment to is compartment
    double phi_k    = - k_K       * ( c_k_ic    - beta_K  * c_k_is ) ; // % Ursino 2008, eq 26. amount of solute exchanged at cellular membrane per unit time. units:
    double phi_na   = - k_Na      * ( c_na_ic   - beta_Na * c_na_is ) ; // % Ursino 2008, eq 26. amount of solute exchanged at cellular membrane per unit time. units:
    double phi_u    = - k_U       * ( c_u_ic    - beta_U  * c_u_is ) ; // % Ursino 2008, eq 26. amount of solute exchanged at cellular membrane per unit time. units:
    double phi_hco3 = - eta_hco3  * ( c_hco3_ic - g_hco3  * c_hco3_is ) ; // % Ursino 2000, eq 12. amount of solute exchanged at cellular membrane per unit time. units:
    double phi_h    = - eta_h     * ( c_h_ic    - g_h     * c_h_is ) ; // % Ursino 2000, eq 12. amount of solute exchanged at cellular membrane per unit time. units:
    double phi_p    = 0.0; // % Ursino 2000. p207, second para.
    double phi_cl   = phi_na + phi_k + phi_h - phi_hco3; // % Ursino 2000, eq 13.
    // %
    // % eq 11 from Ursino 2000. These are eq 20 to 22 p207 of Ursino 2000. Algebriac equations.
    double R_hco3_ic  = etaprime_r * ( kprime_a * c_co2_ic - c_hco3_ic * c_h_ic ); // % eq 20
    // % for c_h_pic in eq 21, see bottom para p207 of Ursino 2000.
    double c_h_pic    = c_p_ic0 - c_p_ic; // % ref ursino 2000 p. 207 last paragraph
    double R_p_ic     = etaprimeprime_r * (kprimeprime_a * c_h_pic - c_p_ic * c_h_ic ); // % eq 21. Dont know c_h_pic yet (4 June 2020).
    double R_h_ic     = R_hco3_ic + R_p_ic; // % eq 22.
    // %
    // % Rsex values are analogous to Rsic values. Ursino 2000 p.
    double R_hco3_is  = etaprime_r * ( kprime_a * c_co2_ex - c_hco3_is * c_h_is ); // % eq 20
    // %
    // % for c_h_pis in eq 21, see bottom para p207 of Ursino 2000.
    // % Protein activity is assumed to be mainly in plasma
    double c_h_pis    = c_pis - c_p_is; // % ref ursino 2000 p. 207 last paragraph
    double R_p_is     = etaprimeprime_r * (kprimeprime_a * c_h_pis - c_p_is * c_h_is ); // % etaprimeprime_r * (kprimeprime_a * c_h_pis - c_p_ex * c_h_ex ); // % eq 21. Dont know c_h_pic yet (4 June 2020).
    double R_h_is     = R_hco3_is + R_p_is; // % eq 22.
    double R_hco3_pl  = etaprime_r * ( kprime_a * c_co2_ex - c_hco3_pl * c_h_pl ); // % eq 20
    // %
    // % for c_h_pic in eq 21, see bottom para p207 of Ursino 2000.
    double c_h_ppl    = c_ppl - c_p_pl; // % ref ursino 2000 p. 207 last paragraph
    double R_p_pl     = etaprimeprime_r * (kprimeprime_a * c_h_ppl - c_p_pl * c_h_pl ); // % eq 21. Dont know c_h_pic yet (4 June 2020).
    double R_h_pl     = R_hco3_pl + R_p_pl; // % eq 22.
    // %
    // % TEMPORARY reaction rate was entered as the sum of rates in pl and is
    // % compartments. Find reference or revise
    double R_hco3_ex  = R_hco3_pl + R_hco3_is; double R_p_ex = R_p_pl + R_p_is; double R_h_ex = R_h_pl + R_h_is;
    // %


    // % eq 29 from Ursino and Innocenti 2008 p888
    double Q_eK     = Q_B*(F_p * (1 - Hct) + F_R * gamma_K     * alphac);
    double Q_eNa    = Q_B*(F_p * (1 - Hct) + F_R * gamma_Na     * alphac);
    double Q_eU     = Q_B*(F_p * (1 - Hct) + F_R * gamma_U     * R_DU);
    double Q_eHCO3  = Q_B*(F_p * (1 - Hct) + F_R * gamma_HCO3 * alphac);
    double Q_eCl    = Q_B*(F_p * (1 - Hct) + F_R * gamma_Cl     * alphaa);

    // % eq 28 from Ursino and Innocenti 2008 p888
    // % convective and diffusive transport to dialyzer.
    // J is solute removal rate across the dialyser.
    double J_k    = (D_s     * (1.0 - Q_f / Q_eK)     + Q_f) * c_k_ex      - D_s     *   (1.0 - Q_f / Q_eK)     * c_k_d ;
    double J_na   = (D_s     * (1.0 - Q_f / Q_eNa)    + Q_f) * c_na_ex     - D_s     *   (1.0 - Q_f / Q_eNa)    * c_na_d ;
    double J_u    = (D_u     * (1.0 - Q_f / Q_eU)     + Q_f) * c_u_ex      - D_u     *   (1.0 - Q_f / Q_eU)     * c_u_d ;
    double J_hco3 = (D_hco3  * (1.0 - Q_f / Q_eHCO3)  + Q_f) * c_hco3_ex   - D_hco3  *   (1.0 - Q_f / Q_eHCO3)  * c_hco3_d ;
    double J_cl   = (D_s     * (1.0 - Q_f / Q_eCl)    + Q_f) * c_cl_ex     - D_s     *   (1.0 - Q_f / Q_eCl)    * c_cl_d ;
    double J_h    = 0.0; // % for hydrogen J is 0.
    double J_p    = 0.0; // % for proteins, J is 0.



    int count = 0;
    for (double val : {c_ic, c_is, F_a, R_v, phi_u, phi_na, phi_k, phi_cl, phi_hco3, phi_h, phi_p, J_u, J_na, J_k, J_cl, J_hco3, J_h, J_p, R_hco3_ic, R_h_ic, R_p_ic, R_hco3_ex, R_h_ex, R_p_ex})
    {
        setDerived(count, val);
        count++;
    }
}

void dialysis::getDY(double t, double y[], double * DY)
{

    // Get derived values

    double c_ic = getDerived(0);
    double c_is = getDerived(1);
    double F_a = getDerived(2);
    double R_v = getDerived(3);

    double phi_u = getDerived(4);
    double phi_na = getDerived(5);
    double phi_k = getDerived(6);
    double phi_cl = getDerived(7);
    double phi_hco3 = getDerived(8);
    double phi_h = getDerived(9);
    double phi_p = getDerived(10);
    double J_u = getDerived(11);
    double J_na = getDerived(12);
    double J_k = getDerived(13);
    double J_cl = getDerived(14);
    double J_hco3 = getDerived(15);
    double J_h = getDerived(16);
    double J_p = getDerived(17);
    double R_hco3_ic = getDerived(18);
    double R_h_ic = getDerived(19);
    double R_p_ic = getDerived(20);
    double R_hco3_ex = getDerived(21);
    double R_h_ex = getDerived(22);
    double R_p_ex = getDerived(23);


    double Q_f        = getP(2);     // % Ultrafiltration rate. units:mL/s
    double Qinfused = getP(3);
    double c_u_inf = getP(5);
    double c_na_inf = getP(6);
    double c_k_inf = getP(7);
    double c_cl_inf = getP(8);

    double k_f = getP(34);



    // % in Urisino eq 2, Oic is the intracellular osmotic activity. units: probably mEq/L or mmol/L unless the 0.93 has
    // % Ois is the interstitial osmotic activity.

    DY[0] = phi_u;                   // % urea            eq 26 Ursino 2000.
    DY[1] = phi_na;                  // % sodium        eq 26 Ursino 2000.
    DY[2] = phi_k;                   // % potassium   eq 26 Ursino 2000.
    DY[3] = phi_cl;                  // % chlorine      eq 11 Ursino 2000.
    DY[4] = phi_hco3 + R_hco3_ic;      // % bicarbonate eq 11 Ursino 2000.
    DY[5] = phi_h         + R_h_ic;         // % hydrogen      eq 11 Ursino 2000.
    DY[6] = phi_p         + R_p_ic;         // % urea            eq 11 Ursino 2000.

    // % for eq 14, Ursino 2000
    // % State variables 28-34 are Extracellular Solute mass
    DY[7]   = -phi_u     - J_u        + Qinfused*c_u_inf; // % urea eq 14, p207, Ursino 2000. combined with eqn 27, Ursino 2008.
    DY[8]   = -phi_na    - J_na       + Qinfused*c_na_inf; // % sodium     eq 14, p207, Ursino 2000.combined with eqn 27, Ursino 2008.
    DY[9]   = -phi_k     - J_k        + Qinfused*c_k_inf; // % potassium  eq 14, p207, Ursino 2000.combined with eqn 27, Ursino 2008.
    DY[10]   = -phi_cl    - J_cl       + Qinfused*c_cl_inf; // % chlorine     eq 14, p207, Ursino 2000.
    DY[11]   = -phi_hco3  - J_hco3     + R_hco3_ex; // % bicarbonate eq 14, p207, Ursino 2000.
    DY[12]   = -phi_h     - J_h        + R_h_ex    ; // % hydrogen eq 14, p207, Ursino 2000.
    DY[13]   = -phi_p     - J_p        + R_p_ex    ; // % protein eq 14, p207, Ursino 2000.

    DY[14]     =   k_f*(c_ic - c_is); // % equation 1, appendix 1, Vic, intracellular fluid volume. Ursino 2000 units: L (liters).
    // is the 1000 conversion F_actor applied consistently throughout the RHS?
    DY[15]     = - k_f*(c_ic - c_is) + (F_a - R_v); // % eq. 2, appendix 1, Vis. Vis is interstial fluid volume. Ursino 2000. units: L.

    if (nCompartments > 0)
      DY[16]     = -(F_a - R_v) - Q_f + Qinfused;     // % eq. 3, appedix 1, Vpl. Vpl is plasma volume. Ursino 2000. unit: L.


}
