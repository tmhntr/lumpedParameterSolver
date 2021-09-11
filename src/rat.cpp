#include "component/vpr_circulation.hpp"
#include "component/component_wrapper.hpp"
#include <iostream>




component * rat()
{
    vpr_circulation * ratty = new vpr_circulation(
      "Rat",
      {"F_a", "R_v", "Q_f"},
      {"P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi"},
      {});
    ratty->setHasDialysis(true);
    std::vector<double> y0 = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};
    return ratty;
}
